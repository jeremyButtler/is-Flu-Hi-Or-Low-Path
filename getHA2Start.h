/*#########################################################
# Name: getHa2Start
# Use:
#  - Holds the function to get the HA2 starting position
#    from a feature table
# Libraries:
#  - "alnSeq/memWater.h"        (No .c file)
#  o "dataTypeShortHand.h"      (No .c file)
#  o "alnSeq/seqStruct.h"       (No .c file)
#  o "alnSeq/generalAlnFun.h"   (No .c file)
#  o "alnSeq/alnSetStruct.h"    (No .c file)
#  o "alnSeq/alnSeqDefaults.h"  (No .c file)
#  o "alnSeq/cStrToNumberFun.h" (No .c file)
# C Standard Libraries:
#  - <string.h>
#  o <stdlib.h>
#  o <stdio.h>
#  o <stdint.h>
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getHa2Start SOF: Start Of File
'  - Functions for getting the HA2 start position and HA
'    sequence from a fasta file.
'  o Header:
'    - Has the libraries and definitions for this file
'  o fun-01 getHA2Start:
'    - Find the first H2 base in the feature table
'  o fun-02 getHaSeq:
'    - Get the HA sequence from a fasta file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Has the libraries and definitions for this file
\--------------------------------------------------------*/

#ifndef GET_HA_START_H
#define GET_HA_START_H

#include <string.h>
#include "alnSeq/memWater.h"

/*--------------------------------------------------------\
| Name: getHA2Start
| Use:
|  - Gets the starting position of the HA2 gene
| Input:
|  - featureTblPathStr:
|    o Path to the feature table to get HA2 gene from
|      (idealy from NCBI flu).
|    o https://www.ncbi.nlm.nih.gov/genomes/FLU/annotation/
| Output:
|  - Returns:
|    o The starting position (index 0) of the feature
|    o 0 if no feature was found
\--------------------------------------------------------*/
static inline ulong getHA2Start(
   char *featureTblPathStr /*Path to the feature table*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: getHA2Start
   '  - Find the first H2 base in the feature table
   '  o fun-01 sec-01:
   '    - Variable declerations
   '  o fun-01 sec-02:
   '    - Check if have valid input/get firt line
   '  o fun-01 sec-03:
   '    - Find the HA feature
   '  o fun-01 sec-04:
   '    - Handle invalid entries and EOFS
   '  o fun-01 sec-05:
   '    - Move past the CDS entry (if present)
   '  o fun-01 sec-06:
   '    - Find the HA2 entry if present
   '  o fun-01 sec-07:
   '    - Get the starting position from the HA2 entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   short buffSizeS = 1 << 10; /*Is 1024*/
   short usCharOn = 0;

   char lastLineStr[buffSizeS];
   char thisLineStr[buffSizeS];
   char *buffStr = lastLineStr;
   char *errStr = 0; /*Reports errors from fgets*/

   ulong startUL = 0;
   FILE *tblFILE = fopen(featureTblPathStr, "r");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^  - Check if have valid input/get firt line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(tblFILE == 0) return 0; /*No file*/

   /*fgets makes my life easier, but is less efficent*/
   errStr = fgets(buffStr, buffSizeS, tblFILE);
   if(errStr == 0) goto invalidEntry;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^  - Find the HA feature
   ^  o fun-01 sec-03 sub-01:
   ^    - Check if new feature (if so move to first entry)
   ^  o fun-01 sec-03 sub-02:
   ^    - Check if the feature entry is a gene entry
   ^  o fun-01 sec-03 sub-03:
   ^    - Check if the gene entry is an HA entry
   ^  o fun-01 sec-03 sub-04:
   ^    - Move to next line if no HA entry in this feature
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /******************************************************\
   * Fun-01 Sec-03 Sub-01:
   *  - Check if new feature (if so move to first entry)
   \******************************************************/

   while(errStr != 0)
   { /*Loop: Find the HA gene entry*/
      if(buffStr[0] == '>')
      { /*If: If I have a header*/
         /*Is the first entry*/
         errStr = fgets(buffStr, buffSizeS, tblFILE);
         if(errStr == 0) goto invalidEntry;
         if(buffStr[0] == '>') continue;

         checkIfGene:

         usCharOn = 0;

         /*Find the first alphebetical character in entry*/
         while(buffStr[usCharOn] < 64) ++usCharOn;

         if(
               buffStr[usCharOn] != 'g'
            || buffStr[usCharOn + 1] != 'e'
            || buffStr[usCharOn + 2] != 'n'
            || buffStr[usCharOn + 3] != 'e'
         ){ /*If: not a gene entry, then find next entry*/
            
            /*Find the next entry (does not start with
            ` white space
            */
            while(buffStr[0] < 33)
            { /*Loop: Get past non-numeric entries*/
               errStr = fgets(buffStr, buffSizeS, tblFILE);
               if(errStr == 0) goto invalidEntry;
            } /*Loop: Get past non-numeric entries*/

            if(buffStr[0] == '>') continue;
            else goto checkIfGene;
         } /*If: not a gene entry, then find next entry*/

         /************************************************\
         * Fun-01 Sec-03 Sub-02:
         *  - Check if the feature is an HA feature
         \************************************************/

         /*Has the gene name of the first entry*/
         errStr = fgets(buffStr, buffSizeS, tblFILE);
         if(errStr == 0) goto invalidEntry;
         if(buffStr[0] == '>') continue;

         usCharOn = 8;
         /*3 tabs + 4 chars in "gene" + 1 tab = 8*/

         /*& ~32 here to make sure is upper case*/
         if(
              ((buffStr[usCharOn] & (~32)) == 'H')
           && ((buffStr[usCharOn] & (~32)) == 'A')
           == 0
         ) break;
      } /*If: If I have a header*/

      /***************************************************\
      * Fun-01 Sec-03 Sub-04:
      *  - Move to next line if no HA entry in this feature
      \***************************************************/

      /*Get the next line*/
      errStr = fgets(buffStr, buffSizeS, tblFILE);
      if(errStr == 0) goto invalidEntry;
   } /*Loop: Find the HA gene entry*/

   /*At this point I am on the HA feature, but I still
   ` need to to find the HA2 product
   */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^  - Handle invalid entries and EOFS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(!errStr)
   { /*If: at end of file*/
      invalidEntry:

      fclose(tblFILE);
      return 0;
   } /*If: at end of file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-06:
   ^  - Find the HA2 entry if present
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Starts out on CDS entry*/
   errStr = fgets(buffStr, buffSizeS, tblFILE);
   if(errStr == 0) goto invalidEntry;
   if(buffStr[0] == '>') goto invalidEntry;

   while(errStr > 0) 
   { /*Loop: find the HA2 entry*/
      /*See if next entry is HA2 (not at start)*/
      if(buffStr == thisLineStr) buffStr = lastLineStr;
      else buffStr = thisLineStr;

      errStr = fgets(buffStr, buffSizeS, tblFILE);

      if(errStr == 0) goto invalidEntry;
      if(buffStr[0] == '>') goto invalidEntry;

      /*Check if there is a product entry, if not, then
      ` I am on the next entry (this was a one line
      ` entry). Just restart the loop.
      */
      if(buffStr[0] > 33) continue;

      usCharOn = 3 + 7 + 1; /*3 tabs + "product" + 1 tab*/

      if(
           buffStr[usCharOn] == 'H'
        && buffStr[usCharOn + 1] == 'A'
        && buffStr[usCharOn + 2] == '2'
      ) break; /*Found the HA2 entry*/

      /*I need to get the next entry (no white space)*/
      while(buffStr[0] < 33)
         errStr = fgets(buffStr, buffSizeS, tblFILE);
   } /*Loop: find the HA2 entry*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-07:
   ^  - Get the starting position from the HA2 entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fclose(tblFILE);

   if(errStr == 0) return 0;

   /*Find out which buffer has the starting position*/
   if(buffStr == thisLineStr) buffStr = lastLineStr;
   else buffStr = thisLineStr;
   strToULBase10(buffStr, &startUL);

   return startUL - 1; /*Convert to index 0*/
} /*getHA2Start*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Name: getHaSeq
' Use:
'  - Finds the HA sequence in a fasta file
' Input:
'  - pathToHaFastaStr:
'    o Fasta file to search for the HA sequence in
' Output:
'  - Returns:
'    o seqStruct with the fasta sequence (seqST->seqCStr).
'    o 0 if no HA sequence was found
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static inline seqStruct * getHaSeq(
   char *pathToHaFastaStr/*Path to fasta with HA sequence*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC: getHaSeq
   '  - Get the HA sequence from a fasta file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   FILE *faFILE = 0;
   struct seqStruct *seqST = malloc(sizeof(seqStruct));

   if(seqST == 0) return 0; /*Memory error*/
   initSeqST(seqST);

   faFILE = fopen(pathToHaFastaStr, "r");

   if(faFILE == 0)
   { /*If: the reference file could not be opened*/
      freeSeqST(seqST, 1); /*1 = Struct on head*/
      seqST = 0;
      return 0; 
   } /*If: the reference file could not be opened*/

   while(readFaSeq(faFILE, seqST) != 0)
   { /*Loop: Till I have found the HA sequence*/
      if(strstr(seqST->idCStr, "HA") != 0)
      { /*If: I found the HA sequence*/ 
         fclose(faFILE);
         return seqST;
      } /*If: I found the HA sequence*/ 
   } /*Loop: Till I have found the HA sequence*/

   /*No HA sequence found*/
   fclose(faFILE);
   freeSeqST(seqST, 1); /*1 = Struct on head*/
   return 0;
} /*getHaSeq*/

/*--------------------------------------------------------\
| Name: findHA2Start
| Use:
|  - Finds the starting position of the HA2 gene in a
|    sequence (this assumes it is an HA sequence)
| Input:
|  - seqStr:
|    o c-string with sequence to find HA2 start in.
|  - lenSeqUL:
|    o length of seqStr (index 1)
|  - retHA2StartUL:
|    o this will hold the start position of the HA2 gene
|      in seqStr (index 0) (The starting position is a
|      best guess. However, it could be wrong.
|  - retConStartUL:
|    o This will report the frist mapped base in the
|      consensus (index 0; first three bases are P1)
| Output:
|  - Modifies:
|    o retHA2StartUL to hold the found HA2 position in
|      seqStr (index 0)
|    o retConStartUL to hold the first mapped base in the
|      consensus (index 0) 
|  - Returns:
|    o Score for the alignment
|    o 0 if the alignment is beneath the min score
|    o -1 if had a memory error
\--------------------------------------------------------*/
static inline long findHA2Start(
   char *seqStr,         /*Sequence to find HA2 position*/
   ulong lenSeqUL,       /*Length of seqStr (index 1)*/
   ulong *retHA2StartUL, /*Start of HA2 on input sequence*/
   ulong *retConStartUL  /*First mapped base in consensus*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-03 TOC: findHA2Start
   '  - Finds the HA2 start position by doing an waterman
   '    alignment against a consensus sequence.
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    char HA2ConStr[] = "arrGGNHTNYHNrGNGCNDWHrYNrKNYKBAT";
    ulong lenHA2ConUL = 32;

    long scoreL = 0;
    struct alnSet settings;


    initAlnSet(&settings);

    *retHA2StartUL = 0;
    *retConStartUL = 0;

    /*This step speeds things up a bit*/
    seqToLookupIndex(seqStr);
    seqToLookupIndex(HA2ConStr);

    scoreL =
       memWaterAln( /*Returns the best score or 0*/
          HA2ConStr,    /*HA2 consensus (hardcoded in)*/
          lenHA2ConUL,  /*Length HA2 consensus (hardcode)*/
          seqStr,       /*Input sequence*/
          lenSeqUL,     /*Length of the input sequence*/
          &settings,    /*Has the settings for alignment*/
          retConStartUL,/*First mapped base in consensus*/
          retHA2StartUL /*Start of HA2 on input sequence*/
       );
    /*HA2ConStr and lenHA2ConUL are at the top of this 
    ` file in the header: section
    */

    lookupIndexToSeq(seqStr);

    if(scoreL < 0) return -1;          /*Memory error*/
    if(scoreL < defMinScore) return 0; /*No good answer*/
    return scoreL;
} /*findHA2Start*/

#endif
