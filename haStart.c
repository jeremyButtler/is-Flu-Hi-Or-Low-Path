/*########################################################
# Name: haStart
# Use:
#  - Holds the function to get the HA2 starting position
#    from a feature table or read
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  - Functions for getting the HA2 start position and HA
'    sequence from a fasta file.
'  o Header:
'    - Has the libraries and definitions for this file
'  o fun01 get_haStart:
'    - Find the first H2 base in the feature table
'  o fun02 getSeq_haStart:
'    - Get the HA sequence from a fasta file
'  o fun02 find_haStart:
'    - Finds the starting position of the HA2 gene in a
'      sequence (this assumes it is an HA sequence)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - libraries for this file
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "haStart.h"

#include <stdio.h>
#include <string.h>

#include "memwater/memwater.h"
#include "memwater/alnSetST.h"
#include "memwater/seqST.h"

/*.h files only*/
#include "generalLib/dataTypeShortHand.h"
#include "generalLib/base10str.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .h #include "memwater/alnDefaults.h"
!   - .h #include "generalLib/ulCp.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_minAlnScore_haStart 40

/*-------------------------------------------------------\
| Fun01: get_haStart
| Use:
|   - Gets the starting position of the HA2 gene
| Input:
|   - tblPathStr:
|     o Path to the feature table to get HA2 gene from
|       (idealy from NCBI flu).
|     o https://www.ncbi.nlm.nih.gov/genomes/FLU/annotation/
| Output:
|  - Returns:
|    o The starting position (index 0) of the feature
|    o 0 if no feature was found
\-------------------------------------------------------*/
unsigned long
get_haStart(
   char *tblPathStr /*Path to the feature table*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '  - Find the first H2 base in the feature table
   '  o fun01 sec01:
   '    - Variable declerations
   '  o fun01 sec02:
   '    - Check if have valid input/get firt line
   '  o fun01 sec03:
   '    - Find the HA feature
   '  o fun01 sec04:
   '    - Handle invalid entries and EOFS
   '  o fun01 sec05:
   '    - Move past the CDS entry (if present)
   '  o fun01 sec06:
   '    - Find the HA2 entry if present
   '  o fun01 sec07:
   '    - Get the starting position from the HA2 entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   short buffSizeS = 1 << 10; /*Is 1024*/
   short usCharOn = 0;

   char lastLineStr[buffSizeS];
   char thisLineStr[buffSizeS];
   char *buffStr = lastLineStr;
   char *errStr = 0; /*Reports errors from fgets*/

   ulong startUL = 0;
   FILE *tblFILE = fopen((char *) tblPathStr, "r");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^  - Check if have valid input/get firt line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(tblFILE == 0)
      return 0; /*No file*/

   /*fgets makes my life easier, but is less efficent*/
   errStr = fgets(buffStr, buffSizeS, tblFILE);

   if(errStr == 0)
      goto invalidEntry;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^  - Find the HA feature
   ^  o fun01 sec03 sub01:
   ^    - Check if new feature (if so move to first entry)
   ^  o fun01 sec03 sub02:
   ^    - Check if the feature entry is a gene entry
   ^  o fun01 sec03 sub03:
   ^    - Check if the gene entry is an HA entry
   ^  o fun01 sec03 sub04:
   ^    - Move to next line if no HA entry in this feature
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *  - Check if new feature (if so move to first entry)
   \*****************************************************/

   while(errStr != 0)
   { /*Loop: Find the HA gene entry*/
      if(buffStr[0] == '>')
      { /*If: If I have a header*/
         /*Is the first entry*/
         errStr = fgets(buffStr, buffSizeS, tblFILE);

         if(errStr == 0)
            goto invalidEntry;

         if(buffStr[0] == '>')
            continue;

         checkIfGene:

         usCharOn = 0;

         /*Find first alphebetical character in entry*/
         while(buffStr[usCharOn] < 64)
            ++usCharOn;

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
               errStr =
                  fgets(buffStr, buffSizeS, tblFILE);

               if(errStr == 0)
                  goto invalidEntry;
            } /*Loop: Get past non-numeric entries*/

            if(buffStr[0] == '>')
               continue;
            else
               goto checkIfGene;
         } /*If: not a gene entry, then find next entry*/

         /***********************************************\
         * Fun01 Sec03 Sub02:
         *  - Check if the feature is an HA feature
         \***********************************************/

         /*Has the gene name of the first entry*/
         errStr = fgets(buffStr, buffSizeS, tblFILE);

         if(errStr == 0)
            goto invalidEntry;

         if(buffStr[0] == '>')
            continue;

         usCharOn = 8;
         /*3 tabs + 4 chars in "gene" + 1 tab = 8*/

         /*& ~32 here to make sure is upper case*/
         if(
              ((buffStr[usCharOn] & (~32)) == 'H')
           && ((buffStr[usCharOn] & (~32)) == 'A')
           == 0
         ) break;
      } /*If: If I have a header*/

      /**************************************************\
      * Fun01 Sec03 Sub04:
      *  - Move to next line if no HA entry in this feature
      \**************************************************/

      /*Get the next line*/
      errStr = fgets(buffStr, buffSizeS, tblFILE);

      if(errStr == 0)
         goto invalidEntry;
   } /*Loop: Find the HA gene entry*/

   /*At this point I am on the HA feature, but I still
   ` need to to find the HA2 product
   */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^  - Handle invalid entries and EOFS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(!errStr)
   { /*If: at end of file*/
      invalidEntry:

      fclose(tblFILE);
      return 0;
   } /*If: at end of file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec06:
   ^  - Find the HA2 entry if present
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Starts out on CDS entry*/
   errStr = fgets(buffStr, buffSizeS, tblFILE);

   if(errStr == 0)
      goto invalidEntry;

   if(buffStr[0] == '>')
      goto invalidEntry;

   while(errStr != 0) 
   { /*Loop: find the HA2 entry*/
      /*See if next entry is HA2 (not at start)*/
      if(buffStr == thisLineStr)
         buffStr = lastLineStr;

      else
         buffStr = thisLineStr;

      errStr = fgets(buffStr, buffSizeS, tblFILE);

      if(errStr == 0)
         goto invalidEntry;

      if(buffStr[0] == '>')
         goto invalidEntry;

      /*Check if there is a product entry, if not, then
      ` I am on the next entry (this was a one line
      ` entry). Just restart the loop.
      */
      if(buffStr[0] > 33)
         continue;

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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec07:
   ^  - Get the starting position from the HA2 entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fclose(tblFILE);

   if(errStr == 0)
      return 0;

   /*Find out which buffer has the starting position*/
   if(buffStr == thisLineStr)
      buffStr = lastLineStr;
   else
      buffStr = thisLineStr;

   strToUL_base10str(buffStr, startUL);

   return startUL - 1; /*Convert to index 0*/
} /*getHA2Start*/

/*-------------------------------------------------------\
| Fun02: getSeq_haStart
| Use:
|   - Finds the HA sequence in a fasta file
| Input:
|   - faStr:
|    o Fasta file to search for the HA sequence in
| Output:
|   - Returns:
|     o seqStruct with the fasta sequence (seqST->seqCStr)
|     o 0 if no HA sequence was found
\-------------------------------------------------------*/
seqStruct *
getSeq_haStart(
   char *faStr  /*Path to fasta with HA sequence*/
){
   FILE *faFILE = 0;
   struct seqStruct *seqST = malloc(sizeof(seqStruct));

   if(seqST == 0)
      return 0; /*Memory error*/

   init_seqST(seqST);

   faFILE = fopen(faStr, "r");

   if(faFILE == 0)
   { /*If: the reference file could not be opened*/
      freeHeap_seqST(seqST); /*1 = Struct on head*/
      seqST = 0;
      return 0; 
   } /*If: the reference file could not be opened*/

   while(getFaSeq_seqST(faFILE, seqST) != 0)
   { /*Loop: Till I have found the HA sequence*/
      if(strstr(seqST->idStr, "HA") != 0)
      { /*If: I found the HA sequence*/ 
         fclose(faFILE);
         return seqST;
      } /*If: I found the HA sequence*/ 
   } /*Loop: Till I have found the HA sequence*/

   /*No HA sequence found*/
   fclose(faFILE);
   freeHeap_seqST(seqST);
   return 0;
} /*getSeq*/

/*-------------------------------------------------------\
| Fun03: find_haStart
| Use:
|  - Finds the starting position of the HA2 gene in a
|    sequence (this assumes it is an HA sequence)
| Input:
|  - seqSTPtr:
|    o pointer to a seqStruct with the sequence to search
|  - alnSTPtr:
|    o pointer to alnSet structure with alignment settings
|  - retStartUL:
|    o this will hold the start position of the HA gene
|      in seqStr (index 0)
|  - retConStartUL:
|    o This will report the frist mapped base in the
|      consensus (index 0; first three bases are P1)
| Output:
|  - Modifies:
|    o retStartUL to hold the found HA2 position in
|      seqStr (index 0)
|    o retConStartUL to hold the first mapped base in the
|      consensus (index 0) 
|  - Returns:
|    o Score for the alignment
|    o 0 if the alignment is beneath the min score
|    o -1 if had a memory error
\-------------------------------------------------------*/
long
find_haStart(
   struct seqStruct *seqSTPtr, /*has sequence to search*/
   struct alnSet *alnSTPtr,   /*alignment settings*/
   unsigned long *retStartUL, /*gets sequence HA2 start*/
   unsigned long *retConStartUL /*1st mapped base in con*/
){
    struct seqStruct refStackST;
    slong scoreL = 0;
    char tmpStr[] = "arrGGNHTNYHNrGNGCNDWHrYNrKNYKBAT";
       /*this allows me to modify this later*/

    ulong refEndUL = 0;
    ulong qryEndUL = 0;

    refStackST.seqStr = tmpStr;
    refStackST.lenSeqUL = 32;

    /*set up the alignment coordiantes*/
    refStackST.endAlnUL = 31;
    refStackST.offsetUL = 0;
    seqSTPtr->offsetUL = 0;
    seqSTPtr->endAlnUL = seqSTPtr->lenSeqUL - 1;

    *retStartUL = 0;
    *retConStartUL = 0;

    /*This step speeds things up a bit*/
    seqToIndex_alnSetST(refStackST.seqStr);
    seqToIndex_alnSetST(seqSTPtr->seqStr);

    scoreL =
       memWater(
          &refStackST,  /*HA consensus (hardcoded in)*/
          seqSTPtr,     /*Input sequence*/
          retStartUL,   /*Start of HA2 on input sequence*/
          &refEndUL,
          retConStartUL,/*First mapped base in consensus*/
          &qryEndUL,
          alnSTPtr     /*Has the settings for alignment*/
       );

    indexToSeq_alnSetST(seqSTPtr->seqStr);

    if(scoreL < 1)
       return -1;          /*Memory error*/

    if(scoreL < def_minAlnScore_haStart)
       return 0; /*No good answer*/

    return scoreL;
} /*find_haStart*/
