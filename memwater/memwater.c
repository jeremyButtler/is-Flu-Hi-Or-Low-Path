/*########################################################
# Name memWater
# Use:
#  o Holds functions doing a memory efficent Smith
#    Waterman pairwise alignments.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of Functions
'   o header:
'     - Included libraries
'   o .c fun01: macroMax
'     - Find the maximum value (branchless)
'   o .c fun02: macroIfMax
'     - Set a value (ret) to a value based on which value
'       is greater.
'   o .c fun03: indexToQry
'     - Gets the query coordinates of the query sequence
'       in an matrix.
'   o .c fun04: indexToRef
'     - Gets the coordinates of the reference sequence in
'       in an matrix.
'   o .c fun05: indexToCoord_memwater
'     - Gets the coordinates of the reference and query
'       sequence in an matrix.
'   o .c fun06: getHiScore
'     - Selects the max score and direction selected for
'       the max score.
'   o .c fun07: getIndelScore
'     - Gets the indel score for a memwater alignment
'   o .c fun08: scoreGt0
'     - Checks to see if the score is greater then zero.
'       If not, this resets the input values
'   o .c fun06: waterMaxScore
'     - Maximizes the score for a single base pair in an
'       waterman query reference scan
'   o .c fun07: waterMaxEndRowScore
'     - Maximizes the score for a single waterman
'       alignment when at the end of an row
'   o .c fun08: scoreGt0
'     - Checks to see if the score is greater then zero.
'       if not, this resets the input values
'   o fun09 memWater:
'     - Run a memory efficent Waterman Smith alignment on
'       input sequences
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "memwater.h"

#include "alnSetST.h"
#include "seqST.h"

/*.h files only*/
#include "../generalLib/dataTypeShortHand.h"
#include "alnDefaults.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o .c #include <stdio.h>
!   o .h #include "alnSeqDefaults.h"
!   o .h #include "../generalLib/base10StrToNum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: macroMax
|  - Find the maximum value (branchless)
| Input:
|  o ret:
|    - Value to hold the maximum value
|  o x:
|    - First value to compare, ret is set to x if x >= y
|  o y:
|    - second value to compare, ret is set to y if y > x
| Output:
|  - Sets:
|    - Sets ret to x if x is greater than or equal to y
|    - Sets ret to y if y is greather than x
\-------------------------------------------------------*/
#define \
macroMax(\
   ret,\
   x,\
   y\
)(\
   (ret) = (x) ^ (((x) ^ (y)) & (-((x) < (y))))\
   /*Logic:
   `  x < y:
   `    if x < y I get 0 (x > y); else I get 1
   `  -(x < y):
   `    If x < y I get -1 (111...)
   `    If x >= y I get 0
   `  x ^ ((x ^ y) & 111...): (only when x < y)
   `    This gives me x
   `  x ^ (x ^ y) & 0: (only when x >= y)
   `    This gives me y
   */\
)

/*-------------------------------------------------------\
| Fun02: macroIfMax
|  - Set a value (ret) to a value based on which value
|    is greater.
| Input:
|  o ret:
|    - This will hold the return value
|  o x:
|    - First value to compare, (if x >= y)
|  o y:
|    - second value to compare, (if y > x)
|  o xRet:
|    - Value to set ret of x is >= y
|  o yRet:
|    - Value to set ret of y is > x
| Output:
|  - Sets:
|    - ret to xRet if x is greater than or equal to y
|    - ret to yRet if y is greater than x
\-------------------------------------------------------*/
#define \
macroIfMax(\
   ret,\
   x,\
   y,\
   xRet,\
   yRet\
)(\
   (ret) = (xRet) ^ (((xRet) ^ (yRet)) & (-((x) < (y))))\
   /*This follows the same logic as macroMax(ret, x, y),
   ' except instead of setting ret to the highest value, I
   ' set ret to xRet if x is >= to y or yRet if y > x.
   */ \
)

/*-------------------------------------------------------\
| Fun03: indexToQry
|   - Gets the query coordinates of the query sequence in
|     an matrix.
| Input
|   - refLen:
|     o Length of the reference sequence
|   - index:
|     o Index to convert to cooridnates
|   - qryCoord:
|     o Will hold the coordinate of the query sequence
| Output:
|  - Returns:
|    o The query coordinate that was in the index
\-------------------------------------------------------*/
#define \
indexToQry(\
   refLen,\
   index\
)({\
   ((index) / ((refLen) + 1)); \
}) /*indexToQry*/

/*-------------------------------------------------------\
| Fun04: indexToRef
|   - Gets the coordinates of the reference sequence in
|     an matrix.
| Input
|   - refLen:
|     o Length of the reference sequence
|   - index:
|     o Index to convert to cooridnates
|   - refCoord:
|     o Will hold the coordinate of the reference sequence
| Output:
|  - Returns
|    o The reference coordinate that was in the index
\-------------------------------------------------------*/
#define \
indexToRef(\
   refLen,\
   index\
)({\
   ((index) % ((refLen) + 1)); \
}) /*indexToRef*/

/*-------------------------------------------------------\
| Fun05: indexToCoord_memwater
|   - Gets the coordinates of the reference and query
|     sequence in an matrix.
| Input
|   - refLen:
|     o Length of the reference sequence
|   - index:
|     o Index to convert to cooridnates
|   - refCoord:
|     o Will hold the coordinate of the reference sequence
|   - qryCoord:
|     o Will hold the coordinate of the query sequence
| Output:
|  - Sets
|    o refCoord to the reference coordinate in index
|    o qryCoord to the query coordinate in index
\-------------------------------------------------------*/
#define \
indexToCoord_memwater(\
   refLen,\
   index,\
   refCoord,\
   qryCoord\
){\
   (refCoord) = indexToRef((refLen), (index));\
   (qryCoord) = indexToQry((refLen), (index));\
} /*indexToCoord_memwater*/

/*-------------------------------------------------------\
| Fun06: getHiScore
|  - Selects the max score and direction selected for the
|    max score.
| Input:
|  - maxSc:
|    o This will hold the max score
|  - maxDir
|    o This will hold the direction of the max score
|  - maxPos:
|    o Index to store
|  - insSc:
|    o Score for having an insertion at this position
|  - snpSc
|    o Score for having an SNP/match at this position
|  - delSc:
|    o Score for having an deletion at this position
|  - snpPos:
|    o Index for and snp
|  - insPos:
|    o Index for a insertion
|  - delPos:
|    o Index for an deletion
| Variations:
|  - fun06 var-a: insDelSnp
|    o Insertions over deleitons over snps/matches
|  - fun06 var-b: delInsSnp
|    o deletions over Insertions over snps/matches
|  - fun06 var-c: insSnpDel
|    o Insertions over snps/matches over deletions
|  - fun06 var-d: delSnpIns
|    o Deletions over snps/matches over insertions
|  - fun06 var-e: snpInsDel
|    o Snps/matches over insertions over deletions
|  - fun06 var-f: snpDelIns
|    o Snps/matches over deletions over insertions
|  - fun06 var-g: Default (insDelSnp)
|    o Default option (Ins over del over snps/matches)
| Output:
|  - Sets:
|    o Sets maxDir to the direction of the max score
|    - Sets maxSc to the max score
\-------------------------------------------------------*/

/*_______________________________________________________\
@ Fun06 Var-A:
@   - Insertions over deleitons over snps/matches
\_______________________________________________________*/
#ifdef INSDELSNP
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax((maxSc), (insSc), (delSc)); /*5 Op*/\
      (maxDir) = (snpSc) > (maxSc);        /*1 Op*/\
      macroIfMax(\
         (maxPos),\
         (insSc),\
         (delSc),\
         (insPos),\
         (delPos)\
      );\
      \
      macroIfMax(\
         (maxPos),\
         (maxSc),\
         (snpSc),\
         (maxPos),\
         (snpPos)\
      );\
      macroMax((maxSc), (maxSc), (snpSc)); /*5 Op*/\
      (maxDir) +=                          /*4 Op*/\
        ( ( (insSc) == (maxSc) ) | (maxDir) ) + defMvDel;\
      /*Logic:
      ` maxDir = snp > max
      `   1 if snp was selected, else 0 (ins/del)
      ` defMvDel = 1
      ` (ins == max) | maxDir
      `   1 if insertion or snp was selected, else 0 (del)
      ` For an snp 1 += (0 | 1) + 1 = 3
      ` For an ins 0 += (1 | 0) + 1 = 2
      ` For an del 0 += (0 | 0) + 1 = 1
      */\
   }
#endif

/*_______________________________________________________\
@ Fun06 Var-B: delInsSnp
@   - deletions over Insertions over snps/matches
\_______________________________________________________*/
#ifdef DELINSSNP
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax((maxSc), (delSc), (insSc));       /*5 Op*/\
      maxDir = (snpSc) > (maxSc);                /*1 Op*/\
      macroIfMax(\
         (maxPos),\
         (delSc),\
         (insSc),\
         (delPos),\
         (insPos)\
      );\
      \
      macroIfMax(\
         (maxPos),\
         (maxSc),\
         (snpSc),\
         (maxPos),\
         (snpPos)\
      );\
      \
      macroMax((maxSc), (maxSc), (snpSc));       /*5 Op*/\
      (maxDir) += ( (delSc) != (maxSc) ) +defMvDel; /*3*/\
      /*Logic:
      ` maxDir = snp > max
      `   maxDir is 1 if snp was selected, else 0
      ` defMvDel = 1
      ` del != maxSc
      `   1 if a deletion (ins/snp) was selected
      ` For an snp 1 += (1) + 1 = 3
      ` For an ins 0 += (1) + 1 = 2
      ` For an del 0 += 0 + 1 = 1
      */\
   }
#endif

/*_______________________________________________________\
@ Fun06 Var-C: insSnpDel
@   - Insertions over snps/matches over deletions
\_______________________________________________________*/
#ifdef INSSNPDEL
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax(maxSc, (insSc), (snpSc));         /*5 Op*/\
      (maxDir) = (delSc) <= (maxSc);             /*1 Op*/\
        /*1 if kept score is not a deletion, else 0*/\
      macroIfMax(\
         (maxPos),\
         (insSc),\
         (snpSc),\
         (insPos),\
         (snpPos)\
      );\
      \
      macroIfMax(\
         (maxPos),\
         (maxSc),\
         (delSc),\
         (maxPos),\
         (delPos)\
      );\
      \
      macroMax((maxSc), (maxSc), (delSc));       /*5 Op*/\
      (maxDir)+= (((snpSc) > (insSc)) & maxDir)+defMvDel;\
         /*4 Op*/\
      /*Logic:
      ` maxDir del <= max
      `   1 if an insertion or snp, else 0 (deletion)
      ` defMvDel = 1
      ` (snp > ins) & maxDir
      `   1 if an snp is selected, else 0 (ins/del)
      ` For an snp 1 += (1 & 1) + 1 = 3
      ` For an ins 1 += (0 & 1) + 1 = 2
      ` For an del 0 += (0/1 & 0) + 1 = 1
      */\
   }
#endif

/*_______________________________________________________\
@ Fun06 Var-D: delSnpIns
@   _ Deletions over snps/matches over insertions
\_______________________________________________________*/
#ifdef DELSNPINS
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax((maxSc), (snpSc), (insSc));       /*5 Op*/\
      (maxDir) = (maxSc) <= (delSc);             /*1 Op*/\
      macroIfMax(\
         (maxPos),\
         (snpSc),\
         (insSc),\
         (snpPos),\
         (insPos)\
      );\
      macroIfMax(\
         (maxPos),\
         (delSc),\
         (maxSc),\
         (delPos),\
         (maxPos)\
      );\
      macroMax((maxSc), (delSc), (maxSc));       /*5 Op*/\
      (maxDir) =                                 /*4 Op*/\
          defMvSnp\
        - ( ( (insSc) > (snpSc) ) | (maxDir) )\
        - (maxDir);\
      /*Logic:
      `  maxDir is 1 if a deletion is selected (max <= del)
      `  defMvSnp = 3
      `  (ins > snp) | (maxDir)
      `     1 if insertion or deletion selected, else 0
      `  -maxDir
      `     Is 1 if deletion was selected
      ` For an del I get 3 - (0/1 | 1) - 1 = 1
      ` For an ins I get 3 - (1 | 0) - 0 = 2
      ` For an snp I get 3 - (0 | 0) - 0 = 3
      */\
   }
#endif

/*_______________________________________________________\
@ Fun06 Var-E: snpInsDel
@   - Snps/matches over insertions over deletions
\_______________________________________________________*/
#ifdef SNPINSDEL
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax((maxSc), (snpSc), (insSc));     /*5 Op*/\
      (maxDir) = ((delSc) <= (maxSc));         /*1 Op*/\
        /*1 if deletion not choosen, else 0*/\
      macroIfMax(\
         (maxPos),\
         (snpSc),\
         (insSc),\
         (snpPos),\
         (insPos)\
      );\
      macroIfMax(\
         (maxPos),\
         (maxSc),\
         (delSc),\
         (maxPos),\
         (delPos)\
      );\
      macroMax((maxSc), (maxSc), (delSc));      /*5 Op*/\
      (maxDir) += ((snpSc) == (maxSc)) +defMvDel;/*3 Op*/\
      /*Logic
      ` maxDir = delSc <= maxSc
      `   1 if an insertion or snp was selected, else 0 (del)
      ` snp == max
      `   1 if the snp was selected.
      ` For an snp 1 += (1) + 1 = 3
      ` For an ins 1 += (0) + 1 = 2
      ` For an del 0 += (0) + 1 = 1
      */\
   }
#endif

/*_______________________________________________________\
@ Fun06 Var-F: snpDelIns
@   - Snps/matches over deletions over insertions
\_______________________________________________________*/
#ifdef SNPDELINS
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax((maxSc), (delSc), (insSc));       /*5 Op*/\
      maxDir = (snpSc) < (maxSc);                /*1 Op*/\
      macroIfMax(\
         (maxPos),\
         (delSc),\
         (insSc),\
         (delPos),\
         (insPos)\
      );\
      macoIfMax(\
         (maxPos),\
         (maxSc),\
         (snpSc),\
         (maxPos),\
         (snpPos)\
      );\
      macroMax((maxSc), (snpSc), (maxSc));       /*5 Op*/\
      (maxDir) =                                 /*4 Op*/\
           defMvSnp /*Always 3*/\
         - (maxDir) \
         - ( ( (delSc) == (maxSc) ) & (maxDir) );\
        /*Logic
        ` maxDir = snp < max
        `   1 if deletion/insertion selected, else 0 (snp)
        ` defMvSnp is always 3
        ` (del == max) & maxDir
        `   1 if a deletin was selected, else 0 (snp/ins)
        ` For an snp (over ins/snp)   3 - 0 - (0/1 & 0) = 3
        ` For an ins (ignore if eqal) 3 - 1 - (0 & 0) = 2
        ` For an del (over ins)       3 - 1 - (1 & 1) = 1
        */\
   }
#endif

/*_______________________________________________________\
@ Fun06 Var-g: default
@   - Default option (del over ins over snps/matches)
\_______________________________________________________*/
#ifndef INSDELSNP
 #ifndef INSSNPDEL
  #ifndef SNPINSDEL
    #ifndef SNPDELINS
      #ifndef DELINSNP
        #ifndef DELSNPINS
   #define \
   getHiScore(\
      maxSc,\
      maxDir,\
      maxPos,\
      snpSc,\
      insSc,\
      delSc,\
      snpPos,\
      insPos,\
      delPos\
   ){\
      macroMax((maxSc), (delSc), (insSc));       /*5 Op*/\
      maxDir = (snpSc) > (maxSc);                /*1 Op*/\
      macroIfMax(\
         (maxPos),\
         (delSc),\
         (insSc),\
         (delPos),\
         (insPos)\
      );\
      \
      macroIfMax(\
         (maxPos),\
         (maxSc),\
         (snpSc),\
         (maxPos),\
         (snpPos)\
      );\
      \
      macroMax((maxSc), (maxSc), (snpSc));       /*5 Op*/\
      (maxDir) += ( (delSc) != (maxSc) ) +defMvDel; /*3*/\
      /*Logic:
      ` maxDir = snp > max
      `   maxDir is 1 if snp was selected, else 0
      ` defMvDel = 1
      ` del != maxSc
      `   1 if a deletion (ins/snp) was selected
      ` For an snp 1 += (1) + 1 = 3
      ` For an ins 0 += (1) + 1 = 2
      ` For an del 0 += 0 + 1 = 1
      */\
   }
     #endif
    #endif
   #endif
  #endif
 #endif
#endif

/*-------------------------------------------------------\
| Fun07: getIndelScore
|   - Gets the indel score for a memwater alignment
| Input:
|   - lastScoreSL:
|     o Previous score that I am using to find this score
|   - gapOpenSC:
|     o Gap opening penalty
|   - gapDiffSS:
|     o The gap_open_penalty - gap_extend_penalty. Used to
|       get the gap extension from the gap opening penalty
|   - dirSC:
|     o Direction moved (indel or snp) for last score
| Variations:
|   - fun07 var-a: nogapextend
|     o Does not use the gap extension penatly
|   - fun07 var-b: default
|     o Applies the gap extension penalty
| Output:
|   - Returns the calcualted score for an indel
\-------------------------------------------------------*/

/*_______________________________________________________\
@ Fun07 Var-A: nogapextend
@   - Does not use the gap extension penatly
\_______________________________________________________*/
#ifdef NOGAPEXTEND
   #define \
   getIndelScore(\
      lastScoreSL,\
      gapOpenSC,\
      gapDiffSS,\
      dirSC\
   )( (lastScoreSL) + (gapOpenSC) )

/*_______________________________________________________\
@ Fun07 Var-B: default
@   - Applies the gap extension penalty
\_______________________________________________________*/
#else
   #define \
   getIndelScore(\
      lastScoreSL,\
      gapOpenSC,\
      gapDiffSS,\
      dirSC\
   )(\
        (lastScoreSL) + (gapOpenSC)\
      + ((gapDiffSS) & (long) ( -((dirSC) != defMvSnp)))\
   )
   /*Logic:
   `   - dirSC != defMvSnp:
   `     o Is 0 if direction is snp
   `     o Is 1 if direction is indel
   `   - -(dirSC != defMvSnp):
   `     o Converts 1 to -1 (indel), but keeps 0 as 0
   `   - gapDiffSS & -(dirSC != defMvSnp):
   `     o -1: gapDiffSS is kept
   `     o 0: gapDiffSS is set to 0
   `   - gapDiffSS + gapOpenSC:
   `     o Gives the gap extension penalty
   `   - 0 + gapOpenSC:
   `     o Gives the gap open penalty
   `   - lastScoreSL + gapOpenSC + gapDiffSS:
   `     o Current indel score with gap exentsion penalty
   `   - lastScoreSL + gapOpenSC + 0:
   `     o Current indel score with gap open penalty
   */
#endif

/*-------------------------------------------------------\
| Fun08: scoreGt0
|   - Checks to see if the score is greater then zero. If
|     not, this resets the input values
| Input:
|   - scoreSL:
|     o Score to check if greater than zero
|   - dirOnSC:
|     o Direction for the score
|   - indexUL:
|     o Index at in the memwater alignment
|   - curIndexUL:
|     o Current index (applied if score is <= zero)
| Output:
|   - Modifies:
|     o Sets scoreSL to 0 if scoreSL <= 0
|     o Sets dirOnSC to 0 if scoreSL <= 0
|     o sets indexUL to curIndexUL if scoreSL <= 0
\-------------------------------------------------------*/
#define \
scoreGt0(\
   scoreSL,\
   dirOnSC,\
   indexUL,\
   curIndexUL\
){\
   long keepDirSL = (long) -((scoreSL) > 0);\
   (dirOnSC) &= keepDirSL;\
   (scoreSL) &= keepDirSL;\
   (indexUL) &= keepDirSL;\
   (indexUL) |= ( (curIndexUL) & (~keepDirSL) );\
} /*scoreGt0*/

/*-------------------------------------------------------\
| Fun09: memWater
|   - Performs a memory efficent Smith Waterman alignment
|     on a pair of sequences
| Input;
|   - qrySeqSTVoidPtr:
|     o Point to an seqStruct with the query sequence and
|       index 0 coordinates to start (offsetUL)/end
|       (endAlnUL) the alignment.
|   - refSeqSTVoidPtr:
|     o Point to an seqStruct with the reference sequence
|       and index 0 coordinates to start (offsetUL)/end
|       (endAlnUL) the alignment.
|   - refStartUL:
|     o Pointer to unsigned long to hold the frist
|       reference base in the alignment
|   - refEndUL:
|     o Pointer to unsigned long to hold the last
|       reference base in the alignment
|   - qryStartUL:
|     o Pointer to unsigned long to hold the frist query 
|       base in the alignment
|   - qryEndUL:
|     o Pointer to unsigned long to hold the last query
|       base in the alignment
|   - alnSetVoidPtr:
|     o Pointer to an alnSet structure with the gap open,
|       gap extend, and scoring matrix for the alingment
| Output:
|  - Modifies:
|    o refStartUL to have 1st reference base in alignment
|    o refEndUL to have last reference base in alignment
|    o qryStartUL to have first query base in alignment
|    o qryEndUL to have last query base in alignment
|  - Returns:
|    o Score for aligment
|    o 0 for memory errors
\-------------------------------------------------------*/
long
memWater(
    void *qrySeqSTVoidPtr,
    void *refSeqSTVoidPtr,
    unsigned long *refStartUL,
    unsigned long *refEndUL,
    unsigned long *qryStartUL,
    unsigned long *qryEndUL,
    void *alnSetVoidPtr      /*Settings for alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC: memWaterAln
   '  - Run a memory efficent Waterman Smith alignment on
   '    input sequences
   '  o fun09 sec01:
   '    - Variable declerations
   '  o fun09 sec02:
   '    - Allocate memory for alignment
   '  o fun09 sec03:
   '    - Fill in initial negatives for ref
   '  o fun0 sec04:
   '    - Fill the matrix with scores
   '  o fun09 sec05:
   '    - Set up for returing matrix (clean up/wrap up)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01: Variable declerations
   ^  o fun09 sec01 sub01:
   ^    - Variables dealing with the query and reference
   ^      starting positions
   ^  o fun09 sec01 sub02:
   ^    - Variables holding the scores (only two rows)
   ^  o fun09 sec01 sub03:
   ^    - Directinol matrix variables
   ^  o fun09 sec01 sub04:
   ^    - Variables for building returend alignment array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec01 Sub01:
   *  - Variables dealing with the query and reference
   *    starting positions
   \*****************************************************/

   /*Set up the void pointers*/
   struct seqStruct *qryST =
      (struct seqStruct *) qrySeqSTVoidPtr;

   struct seqStruct *refST =
      (struct seqStruct *) refSeqSTVoidPtr;

   struct alnSet *settings =
      (struct alnSet *) alnSetVoidPtr;

   long scoreSL = 0; /*Score to return*/
   ulong bestStartUL = 0; /*Records best starting index*/
   ulong bestEndUL = 0;   /*Records best ending index*/

   /*Get start & end of query and reference sequences*/
   char *refSeqStr = 0;
   char *qrySeqStr = 0;

   ulong lenRefUL = refST->endAlnUL - refST->offsetUL + 1;
   ulong lenQryUL = qryST->endAlnUL - qryST->offsetUL + 1;

   /*Iterators for loops*/
   ulong ulRefBase = 0;
   ulong ulQryBase = 0;

   /*****************************************************\
   * Fun09 Sec01 Sub02:
   *  - Variables holding the scores (only two rows)
   \*****************************************************/

   long snpScoreSL = 0;
   long insScoreSL = 0;
   long delScoreSL = 0;    /*Score for doing an deletion*/
   long nextSnpScoreSL = 0;/*Score for next match/snp*/
   long *scoreHeapSL = 0;  /*matrix to use in alignment*/

   #ifndef NOGAPEXTEND
      /*Used in finding if useing gap extension*/
      short gapDiffS =
         settings->gapExtendC - settings->gapOpenC;
   #endif

   /*****************************************************\
   * Fun09 Sec01 Sub03:
   *  - Directional matrix variables
   \*****************************************************/

   /*Direction matrix (1 cell holds a single direction)*/
   char *dirRowHeapSC = 0;  /*Holds directions*/

   /*Keeping track of alignment starting positions*/
   ulong indexUL = 0;      /*Index I am at in the matrix*/
   ulong *indexHeapUL=0;   /*Row of starting indexes*/
   ulong *oldIndexHeapUL=0;/*Last round starting indexes*/
   ulong *swapPtrUL = 0;   /*For swapping ulongs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^  - Allocate memory for alignment
   ^  o fun09 sec02 sub01:
   ^    - Allocate memory for the alignment
   ^  o fun09 sec02 sub02:
   ^    - Allocate memory for keeping track of indexes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec02 Sub01:
   *  - Allocate memory for the alignment
   \****************************************************/

   dirRowHeapSC = malloc((lenRefUL + 1) * sizeof(char));

   if(dirRowHeapSC == 0)
      goto memErr_fun09_sec05_sub01;

   scoreHeapSL = malloc((lenRefUL + 1) * sizeof(long));
   /*+ 1 is for the indel column*/

   if(scoreHeapSL == 0)
      goto memErr_fun09_sec05_sub01;

   /*****************************************************\
   * Fun09 Sec02 Sub02:
   *  - Get memory for keeping track of starting indexes
   \*****************************************************/

   /*Set up the first row of starting indexes*/
   indexHeapUL = malloc((lenRefUL + 1) * sizeof(ulong));

   if(indexHeapUL == 0)
      goto memErr_fun09_sec05_sub01;

   /*Set up the second row of indexs (so have two rows)*/
   oldIndexHeapUL = malloc((lenRefUL +1) * sizeof(ulong));

   if(oldIndexHeapUL == 0)
      goto memErr_fun09_sec05_sub01;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^  - Fill in initial negatives for reference
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(indexUL = 0; indexUL <= lenRefUL; ++indexUL)
   { /*loop; till have initalized the first row*/
      dirRowHeapSC[indexUL] = defMvStop;
      indexHeapUL[indexUL] = indexUL;
      scoreHeapSL[indexUL] = 0;
   } /*loop; till have initalized the first row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^  - Fill the matrix with scores
   ^  o fun09 sec04 sub01:
   ^    - Final set up before scoring the matrix
   ^  o fun09 sec04 sub02:
   ^    - Start loops and get each score
   ^  o fun09 sec04 sub03:
   ^    - Check if is an alternative base best score
   ^  o fun09 sec04 sub04:
   ^    - Find the best score for the last base
   ^  o fun09 sec04 sub05:
   ^    - Is last base in row an alternative alignment?
   ^  o fun09 sec04 sub06:
   ^    - Prepare to score the next row in the matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec04 Sub01:
   *  - Final set up before scoring the matrix
   \*****************************************************/

   /*Move the row of starting indexes to the last row*/
   swapPtrUL = indexHeapUL;
   indexHeapUL = oldIndexHeapUL;
   oldIndexHeapUL = swapPtrUL;

   indexHeapUL[0] = indexUL;

   nextSnpScoreSL = 0;

   /*These are always negative*/
   delScoreSL = 0;
   indexHeapUL[0] = indexUL;
   /*dirRowHeapSC is already set to stop*/
   /*scoreHeapSL is already set to 0*/

   /*Incurment to the frist base*/
   ++indexUL;
   qrySeqStr = qryST->seqStr + qryST->offsetUL;
   refSeqStr = refST->seqStr + refST->offsetUL - 1;
      /*Offseting reference by 1 to account for the gap
      `  column
      */

   /*****************************************************\
   * Fun09 Sec04 Sub02:
   *  - Start loops and get each score
   \*****************************************************/

   /*Starting on the first sequence row*/
   for(
      ulQryBase = 0;
      ulQryBase < lenQryUL;
      ++ulQryBase
   ){ /*loop; compare query base against all ref bases*/

      for(
         ulRefBase = 1;
         ulRefBase < lenRefUL + 1;
         ++ulRefBase
      ){ /*loop; compare one query to one reference base*/

         snpScoreSL =
            getScore_alnSetST(
               qrySeqStr[ulQryBase],
               refSeqStr[ulRefBase],
               settings
            ); /*Find the score for the base pairs*/

         snpScoreSL += nextSnpScoreSL;
         nextSnpScoreSL = scoreHeapSL[ulRefBase];

         insScoreSL =
            getIndelScore(
               scoreHeapSL[ulRefBase],
               settings->gapOpenC,
               gapDiffS,
               dirRowHeapSC[ulRefBase]
            );

         getHiScore(
            scoreHeapSL[ulRefBase],
            dirRowHeapSC[ulRefBase],
            indexHeapUL[ulRefBase],
            snpScoreSL,
            insScoreSL,
            delScoreSL,
            oldIndexHeapUL[ulRefBase - 1], /*snp index*/
            oldIndexHeapUL[ulRefBase], /*insertion index*/
            indexHeapUL[ulRefBase - 1] /*Deletion index*/
         );

         scoreGt0(
            scoreHeapSL[ulRefBase],
            dirRowHeapSC[ulRefBase],
            indexHeapUL[ulRefBase],
            indexUL
         );

         delScoreSL =
            getIndelScore(
               scoreHeapSL[ulRefBase],
               settings->gapOpenC,
               gapDiffS,
               dirRowHeapSC[ulRefBase]
            );

         /***********************************************\
         * Fun09 Sec04 Sub03:
         *  - Determine if is best score (keep as primary)
         \***********************************************/

         if(scoreSL < scoreHeapSL[ulRefBase])
         { /*If: this is the best score*/
            scoreSL = scoreHeapSL[ulRefBase];
            bestStartUL = indexHeapUL[ulRefBase];
            bestEndUL = indexUL;
         } /*If: this was an snp or match*/

         ++indexUL;
      } /*loop; compare one query to one reference base*/

     /***************************************************\
     *  Fun09 Sec04 Sub06:
     *   - Prepare for the next round
     \***************************************************/

     /*Get scores set up for the gap column*/
	  nextSnpScoreSL = 0;
     delScoreSL = 0;

     /*Swap index arrays so the current is last*/
     swapPtrUL = indexHeapUL;
     indexHeapUL = oldIndexHeapUL;
     oldIndexHeapUL = swapPtrUL;

     indexHeapUL[0] = indexUL;

     ++indexUL; /*Set index for the next base pair*/
   } /*loop; compare query base against all ref bases*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^  - Set up for returing the matrix (clean up/wrap up)
   ^  o fun09 sec05 sub01:
   ^    - clean up
   ^  o fun09 sec05 sub02:
   ^    - find the best score
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec05 Sub01:
   *  - clean up
   \*****************************************************/

   indexToCoord_memwater(
      lenRefUL,
      bestStartUL,
      *refStartUL,
      *qryStartUL
   ); /*Convert the starting index to coordinates*/

   *refStartUL += refST->offsetUL;
   *qryStartUL += qryST->offsetUL;

   indexToCoord_memwater(
      lenRefUL,
      bestEndUL,
      *refEndUL,
      *qryEndUL
   ); /*Convert the ending index to coordinates*/

   *refEndUL += refST->offsetUL;
   *qryEndUL += qryST->offsetUL;

   /*my ends and starts seem to be one off*/
   *qryEndUL -= (*qryEndUL > 0);
   *refEndUL -= (*refEndUL > 0);

   free(dirRowHeapSC);
   dirRowHeapSC = 0;

   free(scoreHeapSL);
   scoreHeapSL = 0;

   free(indexHeapUL);
   indexHeapUL = 0;

   free(oldIndexHeapUL);
   oldIndexHeapUL = 0;

   return scoreSL;

   memErr_fun09_sec05_sub01:;

   free(dirRowHeapSC);
   dirRowHeapSC = 0;

   free(scoreHeapSL);
   scoreHeapSL = 0;

   free(indexHeapUL);
   indexHeapUL = 0;

   free(oldIndexHeapUL);
   oldIndexHeapUL = 0;

   return 0;
} /*memWaterAln*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
