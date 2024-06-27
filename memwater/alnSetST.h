/*########################################################
# Name alnSetStruct
# Use:
#  o Holds the settings structures and supporting
#    functions for setting structures for alnSeq's
#    pairwise aligners.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - Header guards and defined variables
'  o .h st01 alnSet:
'     o Holds settings for my alignment program
'  o .h fun01 setScore_alnSetST:
'    - Sets the score for a base pair (reference/query)
'  o .h fun02 setMatch_alnSetST:
'    - Sets if two bases are a match or mismtach
'  o fun03 freeStack_alnSetST:
'    o Frees variables inside alnSet
'  o fun04 freeHeap_alnSetST:
'    o Frees an alnSet structure (and sets to 0)
'  o .h fun05 getScore_alnSetST:
'    - Get the score for a pair of bases from an alignment
'  o .h fun06 getMatch_alnSetST:
'    - Check if two bases were a match or mismatch
'  o fun07 readScoreFile_alnSetST
'     - Reads in a file of scores for a scoring matrix
'  o fun08 readMatchFile_alnSetST:
'    - Reads in a file of matches
'  o .h fun09 seqToIndex_alnSetST:
'    - Converts a sequence to a look up table index
'  o .h fun10 indexToSeq_alnSetST:
'    - Converts a sequence of lookup indexs back into
'      uppercase characters (a-z)
'  o fun11 init_alnSetST:
'    - Set all values in altSet (alingment settings)
'      structure to defaults
'  o license:
'    - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - header guards and defined variables
\-------------------------------------------------------*/

#ifndef ALIGNMENT_SETTINGS_STRUCT_H
#define ALIGNMENT_SETTINGS_STRUCT_H

/*Aligment matrix variables*/
#define defMoveStop 0    /*stop (end of alignment)*/
#define defMoveLeft 1    /*Move left (deletion)*/
#define defMoveUp 2      /*Move up (insertion)*/
#define defMoveDiagnol 3 /*Move on a diagnol (snp/match)*/

/*Flags for match/mismatch*/
#define defBaseMatch 1
#define defBaseSnp 0
#define defMatrixCol 27

/*For seq conversion to index functions*/
#define defClearNonAlph (1 | 2 | 4 | 8 | 16)
  /*clear 64 bit and case*/
#define defToUper (1 | 2 | 4 | 8 | 16 | 64)
    /*Clear 32nd bit (marks lower case)*/

/*-------------------------------------------------------\
| ST01: alnSet
| Use: Holds settings for my alignment program
\-------------------------------------------------------*/
typedef struct alnSet
{ /*alnSet*/
   /*Line wrap for printing out an alignment*/
   unsigned int lineWrapUS;
   unsigned short lenFileNameUS;
   char pBasePosBl; /*1 Print out base numbers*/
   char pFullAlnBl;
     /*1: Print out the full alignmnet
     ` 0: Print out the aligned region
     */
   char formatFlag;
     /*defExpandCig: is default format (S D I = X)
     ` defEMBOSS: is EMBOSS format (| space)
     ` defClustal: is clustal format (* space)
     */
   char justScoresBl;
     /*1: print coordiantes & scores only (query/ref scan)
     ` 0: Print the alignment
     */

   /*Preference for alignment algorithim used*/
   char useNeedleBl;
   char useWaterBl;
   char useHirschBl;
   char memWaterBl;
   char useVectBl;    /*Use vectors (hirschberg only)*/
   char twoBitBl;     /*1: use two bit arrays; 0 do not*/

   /*Directional priorities (see alnSeqDefualts.h for
   ` options)
   */
   char bestDirC;

   /*General alignment variables*/
   char noGapBl;     /*1: use gapExtendC; 0 do not*/
   signed char gapOpenC;  /*Penalty for starting indel*/
   signed char gapExtendC;/* Penalty for extending indel*/

   /*Waterman smith specific variables*/
   char refQueryScanBl;  /*Keep best score for ref/query*/
     /* If set to 1: Recored a best score for each base
     `  in the reference and query in a Waterman alignment
     */
   char filtQryBl; /*1: filter by query position*/
      /*1: Remove alignments with overlaps in the query
      `    positions
      ` 0: Do not apply this filter
      */
   char filtRefBl; /*1: filter by reference position*/
      /*1: Remove alignments with overlaps in reference
      `    positions
      ` 0: Do not apply this filter
      */
   char filtQryRefBl;
      /*1: Remove alignments with overlaps in both the
      `    query and reference positions
      ` 0: Do not apply this filter
      */
   char pAltAlns;  /*1: Print out alternative alignments*/
   long minScoreL; /*Min score to keep alignment*/

   signed char scoreMatrixC[defMatrixCol][defMatrixCol];
   signed char matchMatrixBl[defMatrixCol][defMatrixCol];
     /* Size of both non-WORDS matrixies (27 or 26) is due
     ` to wanting a look up table that can handle
     ` anonymous bases.  Most cells will be set to 0. 
     `  How to get score
     `  score =
     `   scoreMatrixC[(uchar) (base1 & defClearNonAlph)-1]
     `               [(uchar) (base2 & defClearNonAlph)-1]
     */
}alnSet;

/*-------------------------------------------------------\
| Fun01: setScore_alnSetST
|  - Sets the score for a base pair (reference/query)
| Input:
|  - qryBase:
|    o query base to set score fore
|  - refBase:
|    o reference base to set score fore
|  - score:
|    o Score to have for this query/reference combination
|      (max is a short)
|  - alnSetSTPtr:
|    o pointer to alnSet (settings) structer with matrix
|      to add the new query reference comparison score to.
| Output:
|  o Modifies:
|    - one score in an snp/match scoring matrix
|  - default
|    o This assumes that the sequences are converted to
|      indexes
\-------------------------------------------------------*/
#define \
setScore_alnSetST(\
   qryBase,\
   refBase,\
   score,\
   alnSetSTPtr\
){\
   (alnSetSTPtr)->scoreMatrixC\
        [(uchar) ((qryBase) & defClearNonAlph)]\
        [(uchar) ((refBase) & defClearNonAlph)]\
      = (score);\
} /*setScore_alnSetST, sequences will be lookup indexes*/

/*-------------------------------------------------------\
| Fun02: setMatch_alnSetST
|  - Sets if two bases are a match or mismtach
| Input:
|  - qryBase:
|    o query base to set score fore
|  - refBase:
|    o reference base to set score fore
|  - match:
|    o 1: the query and reference bases are matches
|    o 0: the query and reference bases are mismatches
|  - alnSetSTPtr:
|    o pointer to alnSet (settings) structer with matrix
|      to add the new query reference comparison score to.
| Output:
|  - Modifies:
|    o one match in the matchMatrix
| Variations:
|  - default
|    o This assumes that the sequences are converted to
|      indexes
\-------------------------------------------------------*/
/*The user will not be providing converted bases*/
#define \
setMatch_alnSetST(\
   qryBase,\
   refBase,\
   match,\
   alnSetSTPtr\
){\
   (alnSetSTPtr)->matchMatrixBl\
        [(uchar) ((qryBase) & defClearNonAlph)]\
        [(uchar) ((refBase) & defClearNonAlph)]\
      = (match);\
} /*setBasePairMatch, sequences will be lookup indexes*/


/*-------------------------------------------------------\
| Fun03: freeStack_alnSetST
|  - Does a stack free of an alnSet structer
| Input:
|  - alnSetSTPtr:
|    o alnSetSTPtr to free internal variables in
| Output:
|  - Free:
|    o Nothing, there are no heap allocated variables.
|      Is here in case I decide to have heap allocated
|      variables on day.
\-------------------------------------------------------*/
void
freeStack_alnSetST(
   struct alnSet *alnSetSTPtr
);


/*-------------------------------------------------------\
| Fun04: freeHeap_alnSetST
|  - Frees and alnSet (alignment settings) structure
| Input:
|  - alnSetSTPtr:
|    o Pionter to alnSetST to free
| Output:
|  - Free:
|    o alnSetSTPtr
|  - Set:
|    o alnSetSTPtr to 0 (NULL)
\-------------------------------------------------------*/
void
freeHeap_alnSetST(
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun05: getScore_alnSetST:
|  - Get the score for a pair of bases from an alignment
| Input:
|  - qryBase:
|    o character with query base to get score for
|  - refBase:
|    o character with reference base to get score for
|  - alnSetPtr:
|    o alnSet (settings) structer pionter with scoring
|      matrix
| Output:
|  - Returns:
|    o score of the input base pair
|  - default
|    o This assumes that the sequences are converted to
|      indexes
\-------------------------------------------------------*/
#define \
getScore_alnSetST(\
   qryBase,\
   refBase,\
   alnSetSTPtr\
)(\
  (alnSetSTPtr)->scoreMatrixC\
     [(uchar) (qryBase)]\
     [(uchar) (refBase)]\
)/*getBasePairScore, with sequence converted to index*/

/*-------------------------------------------------------\
| Fun06: getMatch_alnSetST
|  - Check if two bases were a match or mismatch
| Input:
|  - qryBase:
|    o character with query base to compare
|  - refBase:
|    o character with reference base to compare to
|  - alnSetPtr:
|    o alnSet (settings) structer pionter with scoring
|      matrix
| Output:
|  - Returns:
|    o defBaseMatch if the bases matched (same)
|    o defBaseSnp if bases were not a match (different)
|  - default
|    o This assumes that the sequences are converted to
|      indexes
\-------------------------------------------------------*/
#define \
getMatch_alnSetST(\
   qryBase,\
   refBase,\
   alnSetSTPtr\
)(\
  (alnSetSTPtr)->matchMatrixBl\
     [(uchar) (qryBase)]\
     [(uchar) (refBase)]\
)/*getBasePairMatch, with sequence converted to index*/

/*-------------------------------------------------------\
| Fun08: readMatchFile_alnSetST
|  - Reads in a file of matches
| Input:
|  - alnSetSTPtr:
|    o pointer to an alnSetST (settings) structure with
|      the match matrix to modify
|  - matchFILE:
|    o File to get matchs from
| Output:
|  - Modifies:
|    o Match matrix in alngSetST to hold the matchs from
|      the file (matchFILE)
|    o matchFILE to point to the end of the file
|  - Returns:
|    o 0 for no errors
|    o position of error in file
\-------------------------------------------------------*/
unsigned long
readMatchFile_alnSetST(
    struct alnSet *alnSetSTPtr,
    void *matchFILE  /*File of matchs for scoring matrix*/
);

/*-------------------------------------------------------\
| Fun09: seqToIndex_alnSetST
|  - Converts a sequence to a look up table index
|    (table is in alnSetStruct.c/h)
| Input:
|  - seqStr:
|    o pointer c-string with the sequence to convert
| Output:
|  - Modifies:
|    o seqST->seqCStr to have look up table indexs (1-27, 
|      with null as 0) instead of bases
| Variations:
|  - default:
|    - converts sequence in seqStructPtr to indexes
\-------------------------------------------------------*/
#define \
seqToIndex_alnSetST(\
   seqStr\
){\
   signed char *tmpStr = (signed char *)(seqStr);\
   \
   while(*tmpStr != '\0')\
   { /*Loop: convert bases to lookup table values*/\
      *tmpStr &= defClearNonAlph;\
      ++tmpStr;\
   } /*Loop: convert bases to lookup table values*/\
   \
} /*seqToIndex_alnSetST*/

/*-------------------------------------------------------\
| Fun10: indexToSeq_alnSetST
|  - Converts a sequence of lookup indexs back into
|    uppercase characters (a-z)
| Input:
|  - seqStructPtr:
|    o Pointer to sequence structer with converte sequence
|      to deconvert (lookup index to base)
| Output:
|  - Modifies:
|    o seqST->seqCStr to have bases instead of look up
|      table indexs
|  - default:
|    - converts sequence in seqStructPtr from index to a
|      sequence (human readable)
\-------------------------------------------------------*/
#define \
indexToSeq_alnSetST(\
   seqStr\
){\
   signed char *tmpStr = (signed char *) (seqStr);\
   \
   while(*tmpStr != '\0')\
   { /*Loop: convert bases to lookup table values*/\
      *tmpStr |= 64;\
      ++tmpStr;\
   } /*Loop: convert bases to lookup table values*/\
} /*indexToSeq_alnSetST*/

/*-------------------------------------------------------\
| Fun11: init_alnSetST
|  - Set values in altSet (alingment settings) structure
|    to default values
| Input:
|  - alnSetSTPtr:
|    o poineter to an alnSet (settings) structure to
|      initialize
| Output:
|  o Modifies:
|    - alnSetST to have default alignment settings values
\-------------------------------------------------------*/
void
init_alnSetST(
    struct alnSet *alnSetST /*Has settings to initialize*/
);

#endif

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
