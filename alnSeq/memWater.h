/*########################################################
# Name memWater
# Use:
#  o Holds functions doing a memory efficent Smith Waterman
#    pairwise alignments. These aligners return positions,
#    which can be then run through a global/local aligner
#    used to find the actual alignment.
# Includes:
#   - "alnSetStruct.h"    (No .c file)
#   - "alnSeqDefaults.h"  (No .c file)
#   o "cStrToNumberFun.h" (No .c file)
#   o "seqStruct.h"       (No .c file)
# C Standard libraries:
#   o <stdlib.h>
#   o <stdint.h>
#   o <stdio.h>
########################################################*/

#ifndef MEMWATER_H
#define MEMWATER_H

#include "seqStruct.h"     
#include "alnSetStruct.h"  
#include "alnSeqDefaults.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' memWater SOH: Start Of Header
' o fun-01 max:
'   - Get the maximum of two values
' o fun-02 charMaxScore:
'    - This finds the max score preferes deletions,
'      insertions, and then snps (if scores are equal)
' o fun-03 updateStartPos:
'   - Updates the start position of the last score in the
'     memory efficent Waterman
' o fun-04 memWaterAln:
'   - Run a memory efficent Waterman Smith alignment on
'     input sequences
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Fun-01: max
| Use:
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
| From:
|  - https://graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax
\--------------------------------------------------------*/
#define max(ret, x, y){\
   (ret) = (x) ^ (((x) ^ (y)) & (-((x) < (y)))); \
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
   */ \
}

/*--------------------------------------------------------\
| Name: charMaxScore (Fun-02:)
| Use:
|  - This finds the max score preferes deletions,
|    insertions, and then snps (if scores are equal)
| Input:
|  - maxScore:
|    o This will hold the maximum score
|  - maxDir:
|    o This will hold the direction of the max score
|  - snp:
|    o Score for making an snp/match move
|  - ins:
|    o Score for making an insertion move
|  - del:
|    o Score for making an deletion move
|   - pref:
|     o Direction combination that is prefered when scores
|       are equal. This can be hardcoded in.
| Output:
|  - Modifies
|    o maxScore to hold the maxium score
|    o maxDir to hold the direction of the max score
\--------------------------------------------------------*/
#define charMaxScore(maxScore,maxDir,snpSc,insSc,delSc){\
   max((maxScore), (delSc), (insSc));         /*5 Op*/\
   maxDir = (snpSc) > (maxScore);             /*1 Op*/\
   max((maxScore), (maxScore), (snpSc));      /*5 Op*/\
   (maxDir) +=                                /*3 Op*/\
      ( (delSc) != (maxScore) ) + defMvDel;\
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


/*--------------------------------------------------------\
| Name: updateStartePos (Fun-01:)
| Use:
|   - Updates the start position of the last score in the
|     memory efficent Waterman
| Input:
|   - dir:
|     o Direction the current cell points to
|   - lastSnpDir:
|     o The previous direction a snp would move to
|     o This is here to allow stops to update
|   - lastRefStart:
|     o Starting point of the previous reference base
|   - lastQryStart:
|     o Starting point of the previous query base
|   - refStartPtr:
|     o Pointer to the reference position to update
|   - qryStartPtr:
|     o Pointer to the query position to update
|   - refBaseStr
|     o Base on in the reference sequence
|   - refStr
|     o First base in the reference sequence
|     o refBaseStr - refStr = reference position (index 0)
|   - qryBaseStr
|      o Base on in the query sequence
|   - qryStr 
|     o First base in the query sequence
|     o qryBaseStr - qryStr = query position (index 0)
| Output:
|  - Modifies:
|    o lastRefStart, lastQryStart to hold the previous
|      starting values for this cell (this is for SNPs)
|    o refStartPtr and qryStartPtr to hold the first base
|      in the alignment.
| Note:
|  - This assumes you are only using one array to keep
|    track of each query and reference starting position.
\--------------------------------------------------------*/
#define updateStartPos( \
   dir, \
   lastSnpDir,   /*Direction of previous snp*/\
   lastRefStart, \
   lastQryStart, \
   refStartPtr, \
   qryStartPtr, \
   refBaseStr, /*Base on in the reference sequence*/ \
   refStr,     /*First base in the reference sequence*/ \
   qryBaseStr, /*Base on in the query sequence*/ \
   qryStr      /*First base in the query sequence*/ \
){\
   unsigned long swapUL = 0; \
   switch(dir) \
   { /*Switch: Find the starting index*/ \
      case defMvStop: \
      /*Case: Last alignment ends (reset start)*/ \
         (lastRefStart) = *(refStartPtr); \
         (lastQryStart) = *(qryStartPtr); \
         break; \
      /*Case: Last alignment ends (reset start)*/ \
      case defMvDel: \
      /*Case: Added an deletion*/ \
         (lastRefStart) = *(refStartPtr); \
         (lastQryStart) = *(qryStartPtr); \
         \
         *(refStartPtr) = *((refStartPtr) - 1); \
         *(qryStartPtr) = *((qryStartPtr) - 1); \
         break; \
      /*Case: Added an deletion*/ \
      case defMvIns: \
      /*Case: Added an insertion*/ \
         (lastQryStart) = *(qryStartPtr); \
         (lastRefStart) = *(refStartPtr); \
         break;  /*Already have starting bases*/ \
      /*Case: Added an insertion*/ \
      case defMvSnp: \
      /*Case: Added an snp/match*/ \
         if(lastSnpDir == defMvStop)\
         { /*If: A snp/match would move to a stop*/\
            (lastRefStart) = *(refStartPtr); \
            (lastQryStart) = *(qryStartPtr); \
            *(refStartPtr) = (refBaseStr) - (refStr); \
            *(qryStartPtr) = (qryBaseStr) - (qryStr); \
            break; \
         } /*If: A snp/match would move to a stop*/\
         \
         swapUL = (lastRefStart); \
         (lastRefStart) = *(refStartPtr); \
         *(refStartPtr) = swapUL; \
         \
         swapUL = (lastQryStart); \
         (lastQryStart) = *(qryStartPtr); \
         *(qryStartPtr) = swapUL; \
         break; \
      /*Case: Added an snp/match*/ \
   } /*Switch: Find the starting index*/ \
} /*updateStartPos*/


/*--------------------------------------------------------\
| Name: memWaterAln
| Use:
|   - Performs a memory efficent Smith Waterman alignment
|     on a pair of sequences
| Input;
|   - qryST:
|     o SeqStruct with the query sequence and index 0
|       coordinates to start (offsetUL)/end (endAlnUL) the
|       alignment.
|   - refST:
|     o SeqStruct with the reference sequence and index 0
|       coordinates to start (offsetUL)/end (endAlnUL) the
|       alignment
|   - settings:
|     o alnSet structure with the setttings, such as
|       gap open, gap extend, scoring matrix, and preffered
|       direction.
|   - retQryStartUL:
|     o This will be changed to hold the first query base
|       in the alignment.
|   - retRefStartUL:
|     o This will be changed to hold the first reference
|       base in the alignment.
| Output:
|  - Returns:
|    o The best score (as long)
|    o -1 if had an error or no alignment
\--------------------------------------------------------*/
static long memWaterAln(
    char *qrySeqStr,        /*query sequence*/
    unsigned long lenQryUL, /*Length of query (index 1)*/
    char *refSeqStr,        /*Reference sequence*/
    unsigned long lenRefUL, /*Length of reference index 1*/
    struct alnSet *settings, /*Settings for alignment*/
    unsigned long *retQryStartUL, /*Will hold query start*/
    unsigned long *retRefStartUL  /*Will hold ref start*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC: memWaterAln
   '  - Run a memory efficent Waterman Smith alignment on
   '    input sequences
   '  o fun-02 sec-01:
   '    - Variable declerations
   '  o fun-02 sec-02:
   '    - Allocate memory for alignment
   '  o fun-02 sec-03:
   '    - Fill in initial negatives for ref
   '  o fun-02 sec-04:
   '    - Fill the matrix with scores
   '  o fun-02 sec-05:
   '    - Set up for returing the matrix (clean up/wrap up)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01: Variable declerations
   ^  o fun-02 sec-01 sub-01:
   ^    - Variables dealing with the query and reference
   ^      starting positions
   ^  o fun-02 sec-01 sub-02:
   ^    - Variables holding the scores (only two rows)
   ^  o fun-02 sec-01 sub-03:
   ^    - Directinol matrix variables
   ^  o fun-02 sec-01 sub-04:
   ^    - Variables for building returend alignment array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /******************************************************\
   * Fun-02 Sec-01 Sub-01:
   *  - Variables dealing with the query and reference
   *    starting positions
   \******************************************************/

   /*Get start & end of the query and reference sequences*/
   char *refStartStr = refSeqStr;
   char *refEndStr = refSeqStr + lenRefUL - 1;

   char *qryStartStr = qrySeqStr;
   char *qryEndStr = qrySeqStr + lenQryUL - 1;

   char *qryIterStr = 0;
   char *refIterStr = 0;

   /*Set up counters for the query and reference base
   `  index
   */
   /******************************************************\
   * Fun-02 Sec-01 Sub-02:
   *  - Variables holding the scores (only two rows)
   \******************************************************/

   long insScoreL = 0;   /*Score for doing an insertion*/
   long snpScoreL = 0;   /*Score for doing an match/snp*/
   long delScoreL = 0;   /*Score for doing an deletion*/
   long nextSnpSL = 0;   /*Score for the next match/snp*/

   /*Marks when to reset score buffer (every second row)*/
   long *scoreRowLP = 0; /*matrix to use in alignment*/
   long *scoreOnLP = 0;  /*Score I am working on*/
   long bestScoreL = 0;
   unsigned long maskUL = 0; /*used to find if score > 0*/

   /*Gap penalities*/
   short gapExtendS = settings->gapExtendS;

   /******************************************************\
   * Fun-02 Sec-01 Sub-03:
   *  - Directinol matrix variables
   \******************************************************/

   /*Direction matrix (one cell holds a single direction)*/
   char *dirRow = 0;  /*Holds directions*/
   char *firstDir = 0; /*Holds directions*/
   char lastDirC = 0;
   char lastLastDirC = 0;
   char dirUC = 0;

   /*For recording the start position*/
   unsigned long *refStartUL = 0;
   unsigned long *qryStartUL = 0;
   unsigned long *refStartFirstIndexUL = 0;
   unsigned long *qryStartFirstIndexUL = 0;
   unsigned long lastRefStartUL = 0;
   unsigned long lastQryStartUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^  - Allocate memory for alignment
   ^  o fun-02 sec-02 sub-01:
   ^    - Allocate memory for the alignment
   ^  o fun-02 sec-02 sub-02:
   ^    - Allocate memory for alternative alignments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /******************************************************\
   * Fun-02 Sec-02 Sub-01:
   *  - Allocate memory for the alignment
   \******************************************************/

   /*Make struct array for every base in reference*/
   scoreRowLP = calloc((lenRefUL + 1), sizeof(long));
      /*+ 1 is for the indel column*/

   if(scoreRowLP == 0) return -1;

   dirRow = calloc(lenRefUL + 1 ,sizeof(char));
   firstDir = dirRow;

   if(dirRow == 0)
   { /*If I do not have a direction matrix for each cell*/
     free(scoreRowLP);
     return -1;
   } /*If I do not have a direction matrix for each cell*/

   /******************************************************\
   * Fun-02 Sec-02 Sub-02:
   *  - Allocate memory for alternative alignments
   \******************************************************/

   refStartUL = calloc(lenRefUL,sizeof(unsigned long));
   refStartFirstIndexUL = refStartUL;
   if(refStartUL == 0)
   { /*If had a memory error*/
      free(scoreRowLP);
      return -1;
   } /*If had a memory error*/

   /*One query position recoreded per refference position*/
   qryStartUL = calloc(lenRefUL, sizeof(unsigned long));
   qryStartFirstIndexUL = qryStartUL;
   if(qryStartUL == 0)
   { /*If had a memory error*/
      free(scoreRowLP);
      free(refStartUL);
      return -1;
   } /*If had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^  - Fill in initial negatives for reference
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *dirRow = defMvStop;
   ++dirRow;

   refIterStr = refStartStr;
   while(refIterStr <= refEndStr)
   { /*loop; till have initalized the first row*/
     *dirRow = defMvStop;
     ++dirRow;

     ++scoreOnLP; /*Already set to 0 by calloc*/
     ++refIterStr; /*Move though the next base*/
   } /*loop; till have initalized the first row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-04:
   ^  - Fill the matrix with scores
   ^  o fun-02 sec-04 sub-01:
   ^    - Get the initial scores
   ^  o fun-02 sec-04 sub-02:
   ^    - Do the first move
   ^  o fun-02 sec-04 sub-03:
   ^    - Fill out the matrix
   ^  o fun-02 sec-04 sub-04:
   ^    - Find the next matches score
   ^  o fun-02 sec-04 sub-05:
   ^    - Find the best score for the last round
   ^  o fun-02 sec-04 sub-06:
   ^    - Find the score for the next deletion
   ^  o fun-02 sec-04 sub-07:
   ^    - Check if is an alternative base best score
   *  o fun-02 sec-04 sub-08:
   *    - Check if is an alternative base best score
   ^  o fun-02 sec-4 sub-09:
   ^    - Move to the next reference base
   ^  o fun-02 sec-04 sub-10:
   ^    - Find the scores for the next insertion
   ^  o fun-02 sec-04 sub-11:
   ^    - Find the best score for the last base
   ^  o fun-02 sec-04 sub-12:
   ^    - Is the last base in row an alternative alignment?
   ^  o fun-02 sec-04 sub-13:
   ^    - Move to the indel column
   ^  o fun-02 sec-04 sub-14:
   ^    - Find the scores for the first base in the row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /******************************************************\
   * Fun-02 Sec-04 Sub-01:
   *  - Get the initial scores
   \******************************************************/

   dirRow = firstDir;

   qryIterStr = qryStartStr;
   refIterStr = refStartStr;
   scoreOnLP = scoreRowLP;
   refStartUL = refStartFirstIndexUL;
   qryStartUL = qryStartFirstIndexUL;

   nextSnpSL =
        getBaseScore(qryIterStr, refIterStr, settings)
      + *scoreOnLP;

   /*These are always negative*/
   *scoreOnLP = 0;
   delScoreL = 0;
   insScoreL = 0;
   lastLastDirC = 0;
   ++scoreOnLP;

   /******************************************************\
   * Fun-02 Sec-04 Sub-01:
   *  - Do the first move
   \******************************************************/

   *dirRow = defMvStop;
   ++dirRow;

   /******************************************************\
   * Fun-02 Sec-04 Sub-03:
   *  - Fill out the matrix
   \******************************************************/

   /*Starting on the first sequence row*/
   while(qryIterStr <= qryEndStr)
   { /*loop; compare query base against all ref bases*/

     refIterStr = refStartStr + 1;

     /*First reference bases column*/
     while(refIterStr <= refEndStr)
     { /* loop; compare one query to one reference base*/

       /**************************************************\
       * Fun-02 Sec-04 Sub-04:
       *  - Find the next matches score
       \**************************************************/

       snpScoreL = nextSnpSL;

       nextSnpSL =
          getBaseScore(
              qryIterStr,
              refIterStr,
              settings
       ); /*Find the score for the two base pairs*/

       nextSnpSL += *scoreOnLP;

       /**************************************************\
       * Fun-02 Sec-04 Sub-05:
       *  - Find the best score for the last round
       \**************************************************/

       lastDirC = lastLastDirC;

       lastLastDirC = *dirRow;

       charMaxScore(
          *scoreOnLP,
          dirUC,
          snpScoreL,
          insScoreL,
          delScoreL
       );

       /*Check if keeping the score*/
       maskUL = -(*scoreOnLP > 0);
       dirUC &= maskUL;
       *scoreOnLP &= maskUL;
       *dirRow = dirUC;

       /**************************************************\
       * Fun-02 Sec-04 Sub-06:
       *  - Find the the next deletion score
       \**************************************************/

       /*Need to move the direction here, so I have
       ` the previous bases direction.
       */
       delScoreL = *scoreOnLP + settings->gapExtendS;

       /**************************************************\
       * Fun-02 Sec-04 Sub-07:
       *  - Determine if is a best score (keep as primary)
       \**************************************************/

       updateStartPos(
          *dirRow,
          lastDirC,
          lastRefStartUL,
          lastQryStartUL,
          refStartUL,
          qryStartUL,
          refIterStr - 1,
          refSeqStr,
          qryIterStr,
          qrySeqStr
       ); /*macro in water.h*/
          
       if(*scoreOnLP > bestScoreL)
       { /*If this is the current best score*/
          bestScoreL = *scoreOnLP;
          *retQryStartUL = *qryStartUL;
          *retRefStartUL = *refStartUL;
       } /*If this is the current best score*/

       /***********************************************\
       * Fun-02 Sec-04 Sub-09:
       *  - Move to next reference base
       \***********************************************/
          
       ++refStartUL;
       ++qryStartUL;
       ++refIterStr; /*Move to next reference base*/
       ++scoreOnLP;  /*Move to the next score*/
       ++dirRow;

       /**************************************************\
       * Fun-02 Sec-04 Sub-10:
       *  - Find the the next insertion score
       \**************************************************/

       /*Find the next insertion score (Is one score
       ` ahead of the just filled score).
       */

       /*Get the new last insertion direction*/
       insScoreL = *scoreOnLP + settings->gapExtendS;
     } /*loop; compare one query to one reference base*/

       /**************************************************\
       * Fun-02 Sec-04 Sub-11:
       *  - Find the best score for the last base
       \**************************************************/

       /*Find the best score for the last base. In this
       ` case, this score can only apply to indels. So,
       ` I need to move off it to avoid overwirting it
       */
       lastDirC = lastLastDirC;

       charMaxScore(
          *scoreOnLP,
          dirUC,
          nextSnpSL,
          insScoreL,
          delScoreL
       );

       /*Check if keeping the score*/
       maskUL = -(*scoreOnLP > 0);
       dirUC &= maskUL;
       *scoreOnLP &= maskUL;
       *dirRow = dirUC;

     /****************************************************\
     * Fun-02 Sec-04 Sub-12:
     *  - Is the last base in row an alternative alignment?
     \****************************************************/

     updateStartPos(
        *dirRow,
        lastDirC,
        lastRefStartUL,
        lastQryStartUL,
        refStartUL,
        qryStartUL,
        refIterStr - 1,
        refSeqStr,
        qryIterStr,
        qrySeqStr
     );

     if(*scoreOnLP > bestScoreL)
     { /*If this is the current best score*/
        bestScoreL = *scoreOnLP;
        *retQryStartUL = *qryStartUL;
        *retRefStartUL = *refStartUL;
     } /*If this is the current best score*/

     refStartUL = refStartFirstIndexUL;
     qryStartUL = qryStartFirstIndexUL;

     /**************************************************\
     *  Fun-02 Sec-04 Sub-13:
     *   - Move to the indel column
     \**************************************************/

      /*I need to move dirRow and insDir to the first
      ` first base in the next row (skip indel column).
      ` The score for the indel column is updated in the
      ` next subsection. I need to find the score for a
      ` deletion before the update.
      */
      dirRow = firstDir;
      *dirRow = defMvStop;
      ++dirRow;

      /**************************************************\
      * Fun-02 Sec-04 Sub-14:
      *  - Find the scores for the first base in the row
      \**************************************************/

      /*Move to indel column and apply gap extension*/
      scoreOnLP = scoreRowLP;
      ++qryIterStr; /*Move to the next query base*/

      /*Find the next score for an snp/match*/
      nextSnpSL = 
           getBaseScore(qryIterStr,refStartStr,settings)
         + *scoreOnLP;

      /*Update the indel column and find next deletion*/
      lastLastDirC = 0;
      *scoreOnLP = 0; /*First column is always insertion*/
      delScoreL = *scoreOnLP + gapExtendS;
      ++scoreOnLP; /*Move to the first base pair*/

      /*At this point insDir is on the first base*/
      insScoreL = *scoreOnLP + settings->gapExtendS;
   } /*loop; compare query base against all ref bases*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-05:
   ^  - Set up for returing the matrix (clean up/wrap up)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   free(firstDir);
   free(scoreRowLP);
   free(qryStartUL);
   free(refStartUL);
   
   scoreRowLP = 0;
   qryStartUL = 0;
   refStartUL = 0;

   return bestScoreL;
} /*memWaterAln*/

#endif
