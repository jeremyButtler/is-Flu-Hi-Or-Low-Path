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
'    - included libraries
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
|  - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "alnSetST.h"

#include <stdio.h>

#include "alnDefaults.h"
#include "../generalLib/base10str.h"
#include "../generalLib/dataTypeShortHand.h"

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
){alnSetSTPtr = alnSetSTPtr; /*quites error message*/}

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
){
   freeStack_alnSetST(alnSetSTPtr);
   free(alnSetSTPtr);
} /*freeHeap_alnSetST*/

/*-------------------------------------------------------\
| Fun07: readScoreFile_alnSetST
|  - Reads in a file of scores for a scoring matrix
| Input:
|  - alnSetSTPtr:
|    o pointer to an alnSetST (settings) structure with
|      the score matrix to modify
|  - scoreFILE:
|    o File to get scores from
| Output:
|  - Modifies:
|    o Score matrix in alngSetST to hold the scores from
|      the file (scoreFILE)
|    o scoreFILE to point to the end of the file
|  - Returns:
|    o 0 for no errors
|    o position of error in file
\-------------------------------------------------------*/
unsigned long
readScoreFile_alnSetST(
    struct alnSet *alnSetSTPtr, /*score matrix to change*/
    FILE *scoreFILE  /*File of scores for scoring matrix*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC: readScoreFile_alnSetST
   '  o fun07 sec-01:
   '    - Variable declerations & set up
   '  o fun07 sec-02:
   '    - Blank the scoring matrix
   '  o fun07 sec-03:
   '    - Read in line and check if comment
   '  o fun07 sec-04:
   '    - Convert score & add to matrix
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec-01:
   ^  - Variable declerations and set up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ushort lenBuffUS = 1024;
   char buffCStr[lenBuffUS];
   char *tmpCStr = 0;
   char scoreC = 0;

   uchar colUC = 0;
   uchar rowUC = 0;

   buffCStr[lenBuffUS - 1] = '\0';
   buffCStr[lenBuffUS - 2] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec-02:
   ^  - Blank the scoring matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(colUC = 0; colUC < defMatrixCol; ++colUC)
   { /*Loop: blank all values in the scoring matrix*/
       for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
           alnSetSTPtr->scoreMatrixC[colUC][rowUC] = 0;
   } /*Loop: blank all values in the scoring matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec-03:
   ^  - Read in line and check if comment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(fgets(buffCStr, 1024, scoreFILE))
   { /*While I have scores to read in*/
       
       if(buffCStr[0] == '/' && buffCStr[1] == '/')
       { /*On a comment, move onto the next line*/
           while(
               buffCStr[lenBuffUS - 2] != '\0' &&
               buffCStr[lenBuffUS - 2] != '\n'
           ) { /*While have more buffer to read in*/
               buffCStr[lenBuffUS - 2] = '\0';
               fgets(buffCStr, 1024, scoreFILE);
           } /*While have more buffer to read in*/

           /*Reset the buffer*/
           buffCStr[lenBuffUS - 2] = '\0';

           continue;
       } /*On a comment, move onto the next line*/

       /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
       ^ Fun07 Sec-04:
       ^  - Convert score & add to matrix
       \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

       if(buffCStr[0] == '\n')
           continue;                        /*Blank line*/

       if(buffCStr[0] < 64 && buffCStr[2] < 64)
           return ftell(scoreFILE);  /*Invalid character*/
       
       tmpCStr = strToSC_base10str(&buffCStr[4], scoreC);

       setScore_alnSetST(
         buffCStr[0],
         buffCStr[2],
         scoreC,
         alnSetSTPtr
       ); /*Add the score to the matrix*/

       if(tmpCStr == &buffCStr[3])
           return ftell(scoreFILE);         /*No score*/

       while(
           buffCStr[lenBuffUS - 2] != '\0' &&
           buffCStr[lenBuffUS - 2] != '\n'
       ){ /*While have more buffer to read in*/
           buffCStr[lenBuffUS - 2] = '\0';
           fgets(buffCStr, 1024, scoreFILE);
       } /*While have more buffer to read in*/

       /*Reset the buffer*/
       buffCStr[lenBuffUS - 2] = '\0';
   } /*While I have scores to read in*/

   return 0;
} /*readScoreFile_alnSetST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC: readMatchFile_alnSetST
   '  o fun08 sec-01:
   '    - Variable declerations & set up
   '  o fun08 sec-02:
   '    - Blank the match matrix
   '  o fun08 sec-03:
   '    - Read in line and check if comment
   '  o fun08 sec-04:
   '    - Add match/snp (mismatch) to match matrix
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec-01:
   ^  - Variable declerations and set up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ushort lenBuffUS = 1024;
   char buffCStr[lenBuffUS];

   uchar colUC = 0;
   uchar rowUC = 0;

   buffCStr[lenBuffUS - 1] = '\0';
   buffCStr[lenBuffUS - 2] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec-02:
   ^  - Blank the scoring matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(colUC = 0; colUC < defMatrixCol; ++colUC)
   { /*Loop: blank all values in the scoring matrix*/
       for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
           alnSetSTPtr->matchMatrixBl[colUC][rowUC] = 0;
   } /*Loop: blank all values in the scoring matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec-03:
   ^  - Read in line and check if comment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(fgets(buffCStr, 1024, (FILE *) matchFILE))
   { /*While I have matchs to read in*/
       
       if(buffCStr[0] == '/' && buffCStr[1] == '/')
       { /*On a comment, move onto the next line*/
           while(
               buffCStr[lenBuffUS - 2] != '\0' &&
               buffCStr[lenBuffUS - 2] != '\n'
           ) { /*While have more buffer to read in*/
               buffCStr[lenBuffUS - 2] = '\0';
               fgets(buffCStr, 1024, (FILE *) matchFILE);
           } /*While have more buffer to read in*/

           /*Reset the buffer*/
           buffCStr[lenBuffUS - 2] = '\0';

           continue;
       } /*On a comment, move onto the next line*/

       /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
       ^ Fun08 Sec-04:
       ^  - Convert match & add to matrix
       \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

       if(buffCStr[0] == '\n')
           continue;                        /*Blank line*/

       if(buffCStr[4] != '1' && buffCStr[4] != '0')
           return ftell((FILE *) matchFILE);
           /*This error means I do not know if match/snp*/

       setMatch_alnSetST(
         buffCStr[0],
         buffCStr[2],
         buffCStr[4],
         alnSetSTPtr
       ); /*Add the match to the matrix*/

       while(
           buffCStr[lenBuffUS - 2] != '\0' &&
           buffCStr[lenBuffUS - 2] != '\n'
       ){ /*While have more buffer to read in*/
           buffCStr[lenBuffUS - 2] = '\0';
           fgets(buffCStr, 1024, (FILE *) matchFILE);
       } /*While have more buffer to read in*/

       /*Reset the buffer*/
       buffCStr[lenBuffUS - 2] = '\0';
   } /*While I have matchs to read in*/

   return 0;
} /*REAdMatchFile_alnSetST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC: init_alnSetST
   '  - Set values in altSet (alingment settings)
   '    structure to defaults
   '  o fun11 sec-01:
   '    - Set non-matrix variables
   '  o fun11 sec-02:
   '    - Initialize scoring matrix
   '  o fun11 sec-03:
   '    - Initialize match matrix
   '  o fun11 sec-04:
   '    - Handle special DNA scoring cases
   '  o fun11 sec-05:
   '    - Handle special DNA match cases
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec-01:
   ^  - Set non-matrix variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Variables for my for loop*/
   uchar colUC = 0;
   uchar rowUC = 0;

   alnSetST->gapOpenC = defGapOpen;
   alnSetST->gapExtendC = defGapExtend;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec-02:
   ^  - Initialize scoring matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(colUC = 0; colUC < defMatrixCol; ++colUC)
   { /*loop for all columns in the comparison matrix*/
       for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
           alnSetST->scoreMatrixC[colUC][rowUC] = 0;
           /*Most of these cells will never be used*/
           /*But are needed to build the table*/
   } /*loop for all columns in the comparison matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec-03:
   ^  - Initialize match matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*Both words, DNA, and AA all are the same when both
    ` characters/bases/amino acids are the same.
    */
    for(colUC = 0; colUC < defMatrixCol; ++colUC)
    { /*loop for all columns in the comparison matrix*/
        for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
        { /*Loop: Fill in the entire matrix*/ 
           if(colUC == rowUC)
             alnSetST->matchMatrixBl[colUC][rowUC] =
                defBaseMatch;
           else
             alnSetST->matchMatrixBl[colUC][rowUC] =
                defBaseSnp;
        } /*Loop: Fill in the entire matrix*/ 
    } /*loop for all columns in the comparison matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec-04:
   ^  - Handle special DNA scoring cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Set up scores for non-anonmyous base pairs*/
   setScore_alnSetST('a', 'a', defAToA, alnSetST);
   setScore_alnSetST('a', 't', defAToT, alnSetST);
   setScore_alnSetST('a', 'u', defAToT, alnSetST);
   setScore_alnSetST('a', 'g', defAToG, alnSetST);
   setScore_alnSetST('a', 'c', defAToC, alnSetST);

   setScore_alnSetST('t', 'a', defTToA, alnSetST);
   setScore_alnSetST('t', 't', defTToT, alnSetST);
   setScore_alnSetST('t', 'u', defTToT, alnSetST);
   setScore_alnSetST('t', 'g', defTToG, alnSetST);
   setScore_alnSetST('t', 'c', defTToC, alnSetST);

   setScore_alnSetST('u', 'a', defTToA, alnSetST);
   setScore_alnSetST('u', 'u', defTToT, alnSetST);
   setScore_alnSetST('u', 'g', defTToG, alnSetST);
   setScore_alnSetST('u', 'c', defTToC, alnSetST);
   setScore_alnSetST('u', 't', defTToT, alnSetST);

   setScore_alnSetST('g', 'a', defGToA, alnSetST);
   setScore_alnSetST('g', 't', defGToT, alnSetST);
   setScore_alnSetST('g', 'u', defGToT, alnSetST);
   setScore_alnSetST('g', 'g', defGToG, alnSetST);
   setScore_alnSetST('g', 'c', defGToC, alnSetST);

   setScore_alnSetST('c', 'a', defCToA, alnSetST);
   setScore_alnSetST('c', 't', defCToT, alnSetST);
   setScore_alnSetST('c', 'u', defCToT, alnSetST);
   setScore_alnSetST('c', 'g', defCToG, alnSetST);
   setScore_alnSetST('c', 'c', defCToC, alnSetST);

   /*non-anonymous base and anonymous base pairs*/
   setScore_alnSetST('a', 'w', defAToW, alnSetST);
   setScore_alnSetST('a', 's', defAToS, alnSetST);
   setScore_alnSetST('a', 'm', defAToM, alnSetST);
   setScore_alnSetST('a', 'k', defAToK, alnSetST);
   setScore_alnSetST('a', 'r', defAToR, alnSetST);
   setScore_alnSetST('a', 'y', defAToY, alnSetST);
   setScore_alnSetST('a', 'b', defAToB, alnSetST);
   setScore_alnSetST('a', 'd', defAToD, alnSetST);
   setScore_alnSetST('a', 'h', defAToH, alnSetST);
   setScore_alnSetST('a', 'v', defAToV, alnSetST);
   setScore_alnSetST('a', 'n', defAToN, alnSetST);
   setScore_alnSetST('a', 'x', defAToX, alnSetST);

   setScore_alnSetST('w', 'a', defWToA, alnSetST);
   setScore_alnSetST('s', 'a', defSToA, alnSetST);
   setScore_alnSetST('m', 'a', defMToA, alnSetST);
   setScore_alnSetST('k', 'a', defKToA, alnSetST);
   setScore_alnSetST('r', 'a', defRToA, alnSetST);
   setScore_alnSetST('y', 'a', defYToA, alnSetST);
   setScore_alnSetST('b', 'a', defBToA, alnSetST);
   setScore_alnSetST('d', 'a', defDToA, alnSetST);
   setScore_alnSetST('h', 'a', defHToA, alnSetST);
   setScore_alnSetST('v', 'a', defVToA, alnSetST);
   setScore_alnSetST('n', 'a', defNToA, alnSetST);
   setScore_alnSetST('x', 'a', defXToA, alnSetST);

   setScore_alnSetST('c', 'w', defCToW, alnSetST);
   setScore_alnSetST('c', 's', defCToS, alnSetST);
   setScore_alnSetST('c', 'm', defCToM, alnSetST);
   setScore_alnSetST('c', 'k', defCToK, alnSetST);
   setScore_alnSetST('c', 'r', defCToR, alnSetST);
   setScore_alnSetST('c', 'y', defCToY, alnSetST);
   setScore_alnSetST('c', 'b', defCToB, alnSetST);
   setScore_alnSetST('c', 'd', defCToD, alnSetST);
   setScore_alnSetST('c', 'h', defCToH, alnSetST);
   setScore_alnSetST('c', 'v', defCToV, alnSetST);
   setScore_alnSetST('c', 'n', defCToN, alnSetST);
   setScore_alnSetST('c', 'x', defCToX, alnSetST);

   setScore_alnSetST('w', 'c', defWToC, alnSetST);
   setScore_alnSetST('s', 'c', defSToC, alnSetST);
   setScore_alnSetST('m', 'c', defMToC, alnSetST);
   setScore_alnSetST('k', 'c', defKToC, alnSetST);
   setScore_alnSetST('r', 'c', defRToC, alnSetST);
   setScore_alnSetST('y', 'c', defYToC, alnSetST);
   setScore_alnSetST('b', 'c', defBToC, alnSetST);
   setScore_alnSetST('d', 'c', defDToC, alnSetST);
   setScore_alnSetST('h', 'c', defHToC, alnSetST);
   setScore_alnSetST('v', 'c', defVToC, alnSetST);
   setScore_alnSetST('n', 'c', defNToC, alnSetST);
   setScore_alnSetST('x', 'c', defXToC, alnSetST);

   setScore_alnSetST('g', 'w', defGToW, alnSetST);
   setScore_alnSetST('g', 's', defGToS, alnSetST);
   setScore_alnSetST('g', 'm', defGToM, alnSetST);
   setScore_alnSetST('g', 'k', defGToK, alnSetST);
   setScore_alnSetST('g', 'r', defGToR, alnSetST);
   setScore_alnSetST('g', 'y', defGToY, alnSetST);
   setScore_alnSetST('g', 'b', defGToB, alnSetST);
   setScore_alnSetST('g', 'd', defGToD, alnSetST);
   setScore_alnSetST('g', 'h', defGToH, alnSetST);
   setScore_alnSetST('g', 'v', defGToV, alnSetST);
   setScore_alnSetST('g', 'n', defGToN, alnSetST);
   setScore_alnSetST('g', 'x', defGToX, alnSetST);

   setScore_alnSetST('w', 'g', defWToG, alnSetST);
   setScore_alnSetST('s', 'g', defSToG, alnSetST);
   setScore_alnSetST('m', 'g', defMToG, alnSetST);
   setScore_alnSetST('k', 'g', defKToG, alnSetST);
   setScore_alnSetST('r', 'g', defRToG, alnSetST);
   setScore_alnSetST('y', 'g', defYToG, alnSetST);
   setScore_alnSetST('b', 'g', defBToG, alnSetST);
   setScore_alnSetST('d', 'g', defDToG, alnSetST);
   setScore_alnSetST('h', 'g', defHToG, alnSetST);
   setScore_alnSetST('v', 'g', defVToG, alnSetST);
   setScore_alnSetST('n', 'g', defNToG, alnSetST);
   setScore_alnSetST('x', 'g', defXToG, alnSetST);

   setScore_alnSetST('t', 'w', defTToW, alnSetST);
   setScore_alnSetST('t', 's', defTToS, alnSetST);
   setScore_alnSetST('t', 'm', defTToM, alnSetST);
   setScore_alnSetST('t', 'k', defTToK, alnSetST);
   setScore_alnSetST('t', 'r', defTToR, alnSetST);
   setScore_alnSetST('t', 'y', defTToY, alnSetST);
   setScore_alnSetST('t', 'b', defTToB, alnSetST);
   setScore_alnSetST('t', 'd', defTToD, alnSetST);
   setScore_alnSetST('t', 'h', defTToH, alnSetST);
   setScore_alnSetST('t', 'v', defTToV, alnSetST);
   setScore_alnSetST('t', 'n', defTToN, alnSetST);
   setScore_alnSetST('t', 'x', defTToX, alnSetST);

   setScore_alnSetST('w', 't', defWToT, alnSetST);
   setScore_alnSetST('s', 't', defSToT, alnSetST);
   setScore_alnSetST('m', 't', defMToT, alnSetST);
   setScore_alnSetST('k', 't', defKToT, alnSetST);
   setScore_alnSetST('r', 't', defRToT, alnSetST);
   setScore_alnSetST('y', 't', defYToT, alnSetST);
   setScore_alnSetST('b', 't', defBToT, alnSetST);
   setScore_alnSetST('d', 't', defDToT, alnSetST);
   setScore_alnSetST('h', 't', defHToT, alnSetST);
   setScore_alnSetST('v', 't', defVToT, alnSetST);
   setScore_alnSetST('n', 't', defNToT, alnSetST);
   setScore_alnSetST('x', 't', defXToT, alnSetST);

   /*Set u & t to same scores (U is RNA version of T)*/
   setScore_alnSetST('u', 'w', defTToW, alnSetST);
   setScore_alnSetST('u', 's', defTToS, alnSetST);
   setScore_alnSetST('u', 'm', defTToM, alnSetST);
   setScore_alnSetST('u', 'k', defTToK, alnSetST);
   setScore_alnSetST('u', 'r', defTToR, alnSetST);
   setScore_alnSetST('u', 'y', defTToY, alnSetST);
   setScore_alnSetST('u', 'b', defTToB, alnSetST);
   setScore_alnSetST('u', 'd', defTToD, alnSetST);
   setScore_alnSetST('u', 'h', defTToH, alnSetST);
   setScore_alnSetST('u', 'v', defTToV, alnSetST);
   setScore_alnSetST('u', 'n', defTToN, alnSetST);
   setScore_alnSetST('u', 'x', defTToX, alnSetST);

   setScore_alnSetST('w', 'u', defWToT, alnSetST);
   setScore_alnSetST('s', 'u', defSToT, alnSetST);
   setScore_alnSetST('m', 'u', defMToT, alnSetST);
   setScore_alnSetST('k', 'u', defKToT, alnSetST);
   setScore_alnSetST('r', 'u', defRToT, alnSetST);
   setScore_alnSetST('y', 'u', defYToT, alnSetST);
   setScore_alnSetST('b', 'u', defBToT, alnSetST);
   setScore_alnSetST('d', 'u', defDToT, alnSetST);
   setScore_alnSetST('h', 'u', defHToT, alnSetST);
   setScore_alnSetST('v', 'u', defVToT, alnSetST);
   setScore_alnSetST('n', 'u', defNToT, alnSetST);
   setScore_alnSetST('x', 'u', defXToT, alnSetST);

   /*anonymous base and anonymous base pairs*/
   setScore_alnSetST('w', 'w', defWToW, alnSetST);
   setScore_alnSetST('w', 's', defWToS, alnSetST);
   setScore_alnSetST('w', 'm', defWToM, alnSetST);
   setScore_alnSetST('w', 'k', defWToK, alnSetST);
   setScore_alnSetST('w', 'r', defWToR, alnSetST);
   setScore_alnSetST('w', 'y', defWToY, alnSetST);
   setScore_alnSetST('w', 'b', defWToB, alnSetST);
   setScore_alnSetST('w', 'd', defWToD, alnSetST);
   setScore_alnSetST('w', 'h', defWToH, alnSetST);
   setScore_alnSetST('w', 'v', defWToV, alnSetST);
   setScore_alnSetST('w', 'n', defWToN, alnSetST);
   setScore_alnSetST('w', 'x', defWToX, alnSetST);

   setScore_alnSetST('s', 'w', defSToW, alnSetST);
   setScore_alnSetST('s', 's', defSToS, alnSetST);
   setScore_alnSetST('s', 'm', defSToM, alnSetST);
   setScore_alnSetST('s', 'k', defSToK, alnSetST);
   setScore_alnSetST('s', 'r', defSToR, alnSetST);
   setScore_alnSetST('s', 'y', defSToY, alnSetST);
   setScore_alnSetST('s', 'b', defSToB, alnSetST);
   setScore_alnSetST('s', 'd', defSToD, alnSetST);
   setScore_alnSetST('s', 'h', defSToH, alnSetST);
   setScore_alnSetST('s', 'v', defSToV, alnSetST);
   setScore_alnSetST('s', 'n', defSToN, alnSetST);
   setScore_alnSetST('s', 'x', defSToX, alnSetST);

   setScore_alnSetST('m', 'w', defMToW, alnSetST);
   setScore_alnSetST('m', 's', defMToS, alnSetST);
   setScore_alnSetST('m', 'm', defMToM, alnSetST);
   setScore_alnSetST('m', 'k', defMToK, alnSetST);
   setScore_alnSetST('m', 'r', defMToR, alnSetST);
   setScore_alnSetST('m', 'y', defMToY, alnSetST);
   setScore_alnSetST('m', 'b', defMToB, alnSetST);
   setScore_alnSetST('m', 'd', defMToD, alnSetST);
   setScore_alnSetST('m', 'h', defMToH, alnSetST);
   setScore_alnSetST('m', 'v', defMToV, alnSetST);
   setScore_alnSetST('m', 'n', defMToN, alnSetST);
   setScore_alnSetST('m', 'x', defMToX, alnSetST);

   setScore_alnSetST('k', 'w', defKToW, alnSetST);
   setScore_alnSetST('k', 's', defKToS, alnSetST);
   setScore_alnSetST('k', 'm', defKToM, alnSetST);
   setScore_alnSetST('k', 'k', defKToK, alnSetST);
   setScore_alnSetST('k', 'r', defKToR, alnSetST);
   setScore_alnSetST('k', 'y', defKToY, alnSetST);
   setScore_alnSetST('k', 'b', defKToB, alnSetST);
   setScore_alnSetST('k', 'd', defKToD, alnSetST);
   setScore_alnSetST('k', 'h', defKToH, alnSetST);
   setScore_alnSetST('k', 'v', defKToV, alnSetST);
   setScore_alnSetST('k', 'n', defKToN, alnSetST);
   setScore_alnSetST('k', 'x', defKToX, alnSetST);

   setScore_alnSetST('r', 'w', defRToW, alnSetST);
   setScore_alnSetST('r', 's', defRToS, alnSetST);
   setScore_alnSetST('r', 'm', defRToM, alnSetST);
   setScore_alnSetST('r', 'k', defRToK, alnSetST);
   setScore_alnSetST('r', 'r', defRToR, alnSetST);
   setScore_alnSetST('r', 'y', defRToY, alnSetST);
   setScore_alnSetST('r', 'b', defRToB, alnSetST);
   setScore_alnSetST('r', 'd', defRToD, alnSetST);
   setScore_alnSetST('r', 'h', defRToH, alnSetST);
   setScore_alnSetST('r', 'v', defRToV, alnSetST);
   setScore_alnSetST('r', 'n', defRToN, alnSetST);
   setScore_alnSetST('r', 'x', defRToX, alnSetST);

   setScore_alnSetST('y', 'w', defYToW, alnSetST);
   setScore_alnSetST('y', 's', defYToS, alnSetST);
   setScore_alnSetST('y', 'm', defYToM, alnSetST);
   setScore_alnSetST('y', 'k', defYToK, alnSetST);
   setScore_alnSetST('y', 'r', defYToR, alnSetST);
   setScore_alnSetST('y', 'y', defYToY, alnSetST);
   setScore_alnSetST('y', 'b', defYToB, alnSetST);
   setScore_alnSetST('y', 'd', defYToD, alnSetST);
   setScore_alnSetST('y', 'h', defYToH, alnSetST);
   setScore_alnSetST('y', 'v', defYToV, alnSetST);
   setScore_alnSetST('y', 'n', defYToN, alnSetST);
   setScore_alnSetST('y', 'x', defYToX, alnSetST);

   setScore_alnSetST('b', 'w', defBToW, alnSetST);
   setScore_alnSetST('b', 's', defBToS, alnSetST);
   setScore_alnSetST('b', 'm', defBToM, alnSetST);
   setScore_alnSetST('b', 'k', defBToK, alnSetST);
   setScore_alnSetST('b', 'r', defBToR, alnSetST);
   setScore_alnSetST('b', 'y', defBToY, alnSetST);
   setScore_alnSetST('b', 'b', defBToB, alnSetST);
   setScore_alnSetST('b', 'd', defBToD, alnSetST);
   setScore_alnSetST('b', 'h', defBToH, alnSetST);
   setScore_alnSetST('b', 'v', defBToV, alnSetST);
   setScore_alnSetST('b', 'n', defBToN, alnSetST);
   setScore_alnSetST('b', 'x', defBToX, alnSetST);

   setScore_alnSetST('d', 'w', defDToW, alnSetST);
   setScore_alnSetST('d', 's', defDToS, alnSetST);
   setScore_alnSetST('d', 'm', defDToM, alnSetST);
   setScore_alnSetST('d', 'k', defDToK, alnSetST);
   setScore_alnSetST('d', 'r', defDToR, alnSetST);
   setScore_alnSetST('d', 'y', defDToY, alnSetST);
   setScore_alnSetST('d', 'b', defDToB, alnSetST);
   setScore_alnSetST('d', 'd', defDToD, alnSetST);
   setScore_alnSetST('d', 'h', defDToH, alnSetST);
   setScore_alnSetST('d', 'v', defDToV, alnSetST);
   setScore_alnSetST('d', 'n', defDToN, alnSetST);
   setScore_alnSetST('d', 'x', defDToX, alnSetST);

   setScore_alnSetST('h', 'w', defHToW, alnSetST);
   setScore_alnSetST('h', 's', defHToS, alnSetST);
   setScore_alnSetST('h', 'm', defHToM, alnSetST);
   setScore_alnSetST('h', 'k', defHToK, alnSetST);
   setScore_alnSetST('h', 'r', defHToR, alnSetST);
   setScore_alnSetST('h', 'y', defHToY, alnSetST);
   setScore_alnSetST('h', 'b', defHToB, alnSetST);
   setScore_alnSetST('h', 'd', defHToD, alnSetST);
   setScore_alnSetST('h', 'h', defHToH, alnSetST);
   setScore_alnSetST('h', 'v', defHToV, alnSetST);
   setScore_alnSetST('h', 'n', defHToN, alnSetST);
   setScore_alnSetST('h', 'x', defHToX, alnSetST);

   setScore_alnSetST('v', 'w', defVToW, alnSetST);
   setScore_alnSetST('v', 's', defVToS, alnSetST);
   setScore_alnSetST('v', 'm', defVToM, alnSetST);
   setScore_alnSetST('v', 'k', defVToK, alnSetST);
   setScore_alnSetST('v', 'r', defVToR, alnSetST);
   setScore_alnSetST('v', 'y', defVToY, alnSetST);
   setScore_alnSetST('v', 'b', defVToB, alnSetST);
   setScore_alnSetST('v', 'd', defVToD, alnSetST);
   setScore_alnSetST('v', 'h', defVToH, alnSetST);
   setScore_alnSetST('v', 'v', defVToV, alnSetST);
   setScore_alnSetST('v', 'n', defVToN, alnSetST);
   setScore_alnSetST('v', 'x', defVToX, alnSetST);

   setScore_alnSetST('n', 'w', defNToW, alnSetST);
   setScore_alnSetST('n', 's', defNToS, alnSetST);
   setScore_alnSetST('n', 'm', defNToM, alnSetST);
   setScore_alnSetST('n', 'k', defNToK, alnSetST);
   setScore_alnSetST('n', 'r', defNToR, alnSetST);
   setScore_alnSetST('n', 'y', defNToY, alnSetST);
   setScore_alnSetST('n', 'b', defNToB, alnSetST);
   setScore_alnSetST('n', 'd', defNToD, alnSetST);
   setScore_alnSetST('n', 'h', defNToH, alnSetST);
   setScore_alnSetST('n', 'v', defNToV, alnSetST);
   setScore_alnSetST('n', 'n', defNToN, alnSetST);
   setScore_alnSetST('n', 'x', defNToX, alnSetST);

   setScore_alnSetST('x', 'w', defXToW, alnSetST);
   setScore_alnSetST('x', 's', defXToS, alnSetST);
   setScore_alnSetST('x', 'm', defXToM, alnSetST);
   setScore_alnSetST('x', 'k', defXToK, alnSetST);
   setScore_alnSetST('x', 'r', defXToR, alnSetST);
   setScore_alnSetST('x', 'y', defXToY, alnSetST);
   setScore_alnSetST('x', 'b', defXToB, alnSetST);
   setScore_alnSetST('x', 'd', defXToD, alnSetST);
   setScore_alnSetST('x', 'h', defXToH, alnSetST);
   setScore_alnSetST('x', 'v', defXToV, alnSetST);
   setScore_alnSetST('x', 'n', defXToN, alnSetST);
   setScore_alnSetST('x', 'x', defXToX, alnSetST);

   return;
} /*init_alnSetST*/

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
