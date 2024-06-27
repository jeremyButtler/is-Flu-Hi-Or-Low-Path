/*########################################################
# Name: getHaPath
# Use:
#  - Determines if a flu strain is high or low path by the
#    P1 to P6 amino acids in the HA gene
# Libraries
#  - "hiLowPathFun.h"           (No .c file)
#  - "getHA2Start.h"            (No .c file)
#  o "dataTypeShortHand.h"      (No .c file)
#  o "hiLowPathTbls.h"          (No .c file)
#  o "alnSeq/memWater.h"        (No .c file)
#  o "alnSeq/cStrToNumberFun.h" (No .c file)
#  o "alnSeq/seqStruct.h"       (No .c file)
#  o "alnSeq/generalAlnFun.h"   (No .c file)
#  o "alnSeq/alnSetStruct.h"    (No .c file)
#  o "alnSeq/alnSeqDefaults.h"  (No .c file)
# C Stanard Libraries:
#  o <stdlib.h>
#  o <sdtio.h>
#  o <stdint.h>
#  o <string.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  - This file is the main file for determining if a flu
'    strain is hi or low path.
'  o header:
'    - Has includes and default variables
'  o main:
'    - Driver function to find if a flu strain is high or
'      low path
'  o fun01 getUserInput:
'    - Get user input
'  o fun02 pHelpMesg
'    - Prints out the help message
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Has includes and default variables
\--------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include <string.h>

#include "haStart.h"
#include "haPath.h"

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

#define defVersion 20240627

/*-------------------------------------------------------\
| Fun01: getUserInpt
| Use:
|  - Gets the user input from args
| Input:
|  - numArgsI:
|    o Number of arguments and parameters the user input
|  - args:
|    o Has the user input and parameters
|  - faFileStr:
|    o Set to point to the fasta file path in args
|  - featureFileStr:
|    o Set to point to the feature table path in args
|  - HA2StartUL:
|    o Changed to hold the starting position of the HA2
|      gene (if the user supplied it)
| Output:
|  - Modifies:
|    - All input variables except numArgsI and args; see
|      input for how modified.
\-------------------------------------------------------*/
char * getUserInput(
   int numArgsI,     /*Number of args/parms user input*/
   char *args[],     /*Input user arguments*/
   char **faFileStr, /*Holds path to input fasta file*/
   char **featureFileStr, /*Holds path to feature table*/
   ulong *HA2StartUL /*In case user supplied HA2 start*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC: getUserInput
   '  - Get user input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   char *parmStr = 0; /*Points to a single parameter*/
   char *argStr = 0;  /*Points to a single argument*/
   uchar ucInput = 1;

   for(ucInput = 1; ucInput < numArgsI; ++ucInput)
   { /*Loop: Get user input*/
      parmStr = args[ucInput];
      argStr = args[ucInput + 1];

      if(strcmp(parmStr, "-tbl") == 0)
      { /*If: the user provided a feature table*/
         *featureFileStr = argStr;
         ++ucInput;
      } /*If: the user provided a feature table*/

      else if(strcmp(parmStr, "-fa") == 0)
      { /*Else if: the user provied a fasta file*/
         *faFileStr = argStr;
         ++ucInput;
      } /*Else if: the user provied a fasta file*/

      else if(strcmp(parmStr, "-HA2-start") == 0)
      { /*Else if: the user provided the start of HA2*/
         /*Check if this is a numeric input*/
         if(*strToUL_base10str(argStr, *HA2StartUL) > 32)
            return "0 non-numeric HA2 length provided\n";

         --(*HA2StartUL); /*Convert to index 0*/
         ++ucInput;
      } /*Else if: the user provided the start of HA2*/

      else return parmStr;
   } /*Loop: Get user input*/

   return 0;
} /*getUserInput*/

/*-------------------------------------------------------\
| Fun02: pHelpMesg
| Use:
|  - Prints out the help message
| Input:
|  - outFILE:
|    o File to print the help message to
| Output:
|  - Prints:
|    o Prints help message to outFILE
\-------------------------------------------------------*/
void pHelpMesg(
   FILE *outFILE /*File to print help message to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC: pHelpMesg
   '  - Prints out the help message
   '  o fun02 sec01:
   '    - Print out usage lines
   '  o fun02 sec02:
   '    - Print out the input options
   '  o fun02 sec02:
   '    - Print out the output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^  - Print out usage lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "isHiLowPath -tbl features.tbl -fa seq.fasta\n"
   );

   fprintf(outFILE, "Use:\n");
   fprintf(
      outFILE,
      "  - Determines if a flu HA sequence is from a hi or"
   );
   fprintf(outFILE, " low\n    path flu strain.\n");
   fprintf(
      outFILE,
      "  - Checks if flu is high path by comparing the"
   );
   fprintf(outFILE, " P1 to P6\n");
   fprintf(
      outFILE,
      "    amino acids to see if they match sequences"
   );
   fprintf(outFILE, " seen in \n    hi path flu's\n");
   fprintf(outFILE, "      o P1'-R-R\n");
   fprintf(outFILE, "      o P1'-R-K\n");
   fprintf(outFILE, "      o P1'-R-X-R\n");
   fprintf(outFILE, "      o P1'-R-X-K\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^  - Print out the input options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Input:\n");

   fprintf(outFILE, "  -fa: [Required]\n");
   fprintf(
      outFILE,
      "    o Fasta file with HA sequence to check\n"
   );
   fprintf(
     outFILE,
     "    o For tables or input HA2 starting positions;\n"
   );
   fprintf(
    outFILE,
    "      include a \"HA\" in the header (>)\n"
   );


   fprintf(outFILE, "  -tbl: [Optional]\n");
   fprintf(outFILE, "    o Feature table from\n      ");
   fprintf(
     outFILE,
     "https://www.ncbi.nlm.nih.gov/genomes/FLU/annotation/"
   );
   fprintf(outFILE, "\n");
   fprintf(
      outFILE,
      "    o This is more reliable than an alignment\n"
   );

   fprintf(outFILE, "  -HA2-start: [Not used]\n");
   fprintf(
      outFILE,
      "    o The position of first base of the HA2 gene\n"
   );
   fprintf(
      outFILE,
      "    o This disables -tbl\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^  - Print out the output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");
   fprintf(
      outFILE,
      "  - Prints result as tsv to stdout\n"
   );

   fprintf(
     outFILE,
     "    o \"header\thi/low_path\tIs_P2_phe_or_try"
   );
   fprintf(outFILE, "\tP1_to_P6_aa\tH2start\"\n");
} /*pHelpMesg*/


/*-------------------------------------------------------\
| Main:
|   - main driver function to pathotype HA sequences
| Input:
|   - numArgsI:
|     o number of arguments user input
|   - argsStr:
|     o c-string array with user input arguments
| Output:
|   - Prints:
|     o pathotype (as tsv) to the output file
\-------------------------------------------------------*/
int main(
   int numArgsI,   /*Number of arguments user input*/
   char *argsStr[] /*Arguments user input*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main: detectHiLowPath
   '  - Detects if a flu strain is high or low path
   '  o main sec01:
   '    - Variable declerations
   '  o main sec02:
   '    - get user input, check user input, and initialize
   '  o main sec03:
   '    - Get HA2 staring position and HA sequence
   '  o main sec04:
   '    - print out the header
   '  o main sec05:
   '    - Use HA2 start position to get if high/low path
   '    - only goes if user provided a start
   '  o main sec06:
   '    - Find HA2 start & then if high/low path
   '    - only goes if user did not provided a start
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^  - Variable declerations
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   char *fastaStr = 0;
   char *featureTblStr = 0;
   char *errStr = 0; /*Errors from user input*/

   char p1ToP6AaStr[def_numPSites_haPath];

   char hiPathBl = 0;
   char p2VirulBl = 0; /*is a phe or try*/

   char *outHiLowStr = "";
   char *outP2VirlStr = "";

   long HA2AlnScoreL = 0; /*Alignment score for HA2*/
   ulong HA2StartUL = 0;
   ulong HA2ConFirstBaseUL = 0;

   struct seqStruct *seqHeapST = 0;
   struct seqStruct seqStackST;
   struct alnSet alnStackST; /*alignment settings*/
   
   FILE *testFILE = 0;
   FILE *outFILE = stdout;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^  - get user input, check user input, and initialize
   ^  o main sec02 sub01:
   ^    - get user input
   ^  o main sec02 sub02:
   ^    - Check if the reference can be opened
   ^  o main sec02 sub03:
   ^    - Check if feature table can be opened
   ^  o main sec02 sub04:
   ^    - initialize variables
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *  - get user input
   \*****************************************************/

    errStr =
       getUserInput(
          numArgsI,
          argsStr,
          &fastaStr,
          &featureTblStr,
          &HA2StartUL /*In case user supplied HA2 start*/
    );

   if(errStr != 0)
   { /*If: there was an error*/
      if(
            strcmp(errStr, "-h") == 0
         || strcmp(errStr, "--h") == 0
         || strcmp(errStr, "-help") == 0
         || strcmp(errStr, "--help") == 0
         || strcmp(errStr, "help") == 0
      ) { /*If: the user wanted the help message*/
         pHelpMesg(stdout);
         exit(0);
      } /*If: the user wanted the help message*/

      if(
            strcmp(errStr, "-v") == 0
         || strcmp(errStr, "--v") == 0
         || strcmp(errStr, "-version") == 0
         || strcmp(errStr, "--versoin") == 0
         || strcmp(errStr, "version") == 0
      ) { /*If: the user wanted the help message*/
         fprintf(
            stdout,
            "isHiLowPathFlu version: %i\n",
            defVersion
         );
         exit(0);
      } /*If: the user wanted the help message*/

      if(errStr[0] == '0')
      { /*If: this was do to non-numeric entry*/
         fprintf(stderr, "%s", &errStr[2]);
         exit(-1);
      } /*If: this was do to non-numeric entry*/

      pHelpMesg(stderr);
      fprintf(stderr, "\nInvalid input (%s)\n", errStr);
      exit(-1);
   } /*If: there was an error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *  - Check if the reference file can be opened
   \*****************************************************/

   testFILE = fopen(fastaStr, "r");

   if(testFILE == 0)
   { /*If: the reference file could not be opened*/
      fprintf(
         stderr,
         "Input file (-fa %s) could not be opened\n",
         fastaStr
      );

      exit(-1);
   } /*If: the reference file could not be opened*/

   fclose(testFILE);
   testFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *  - Check if the feature table file can be opened
   \*****************************************************/

   if(HA2StartUL == 0 && featureTblStr != 0)
   { /*If: user did not provide the HA2 start position*/
      testFILE = fopen(featureTblStr, "r");

      if(testFILE == 0)
      { /*If: the freature table could not be opened*/
         fprintf(
           stderr,
           "Feature table (-tbl %s) could not be opened\n",
           featureTblStr
         );

         exit(-1);
      } /*If: the feature table could not be opened*/

      fclose(testFILE);
      testFILE = 0;
   } /*If: user did not provide the HA2 start position*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *  - initialize variables
   \*****************************************************/

   init_alnSetST(&alnStackST);
   init_seqST(&seqStackST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^  - Get HA2 staring position
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*Check if the user already provided this*/
   if(HA2StartUL == 0 && featureTblStr != 0)
   { /*If: the user did not provided the HA2 start*/
      HA2StartUL = get_haStart(featureTblStr);

      if(HA2StartUL == 0)
      { /*If: I could not extract HA2 starting position*/
         fprintf(
             stderr,
             "Could not get HA2 start from %s\n",
             featureTblStr
         );

         freeStack_seqST(&seqStackST);
         freeStack_alnSetST(&alnStackST);

         exit(-1);
      } /*If: I could not extract HA2 starting position*/
   } /*If: the user did not provided the HA2 start*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^  - print out header
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   fprintf(
       outFILE,
      "id\thiPath\tP2_virl\taa_seq\tH2_start\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^  - Use HA2 start position to get if high/low path
   ^  - only goes if user provided a start
   ^  o main sec05 sub01:
   ^    - Get the HA sequence (if have a start position)
   ^  o main sec05 sub02:
   ^    - Get the P1 to P6 amino acid sequence
   ^  o main sec05 sub03:
   ^    - Check high path/low path & other P1-P6 markers
   ^  o main sec05 sub04:
   ^    - Print out the results
   ^  o main sec05 sub05:
   ^    - Cean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec05 Sub01:
    *  - Get the HA sequence (if have a start position)
    \****************************************************/

    if(HA2StartUL)
    { /*If: have an HA2 starting position*/
       seqHeapST = getSeq_haStart(fastaStr);

       if(seqHeapST == 0)
       { /*If: I could not find the HA sequence*/
         freeHeap_seqST(seqHeapST);
         seqHeapST = 0;

         freeStack_seqST(&seqStackST);
         freeStack_alnSetST(&alnStackST);

         fprintf(
            stderr,
            "Could not find the HA sequence in %s\n",
            fastaStr
         );

         exit(-1);
       } /*If: I could not find the HA sequence*/

      /**************************************************\
      * Main Sec05 Sub02:
      *  - Get the P1 to P6 amino acid sequence
      \**************************************************/

       getP1_P6AA_haPath(
          seqHeapST->seqStr,
          p1ToP6AaStr,
          HA2StartUL
       ); /*get P1 to P6 amino acid sequence*/

      /**************************************************\
      * Main Sec05 Sub03:
      *  - Check high path/low path & other P1-P6 markers
      \**************************************************/

       hiPathBl = find_haPath(p1ToP6AaStr);

       if(hiPathBl)
          outHiLowStr = "high_path";
       else
          outHiLowStr = "low_path";

       p2VirulBl = P2PheTryMut_haPath(p1ToP6AaStr);

       if(p2VirulBl)
          outP2VirlStr = "P2=True";
       else
          outP2VirlStr = "P2=False";

      /**************************************************\
      * Main Sec05 Sub04:
      *  - Print out the results
      \**************************************************/

       /*Get rid of new line at end of header*/
       seqHeapST->idStr[seqHeapST->lenIdUL - 1] = '\0';

       fprintf(
          outFILE,
          "%s\t%s\t%s\tP1'-%c-%c-%c-%c-%c-%c\t%lu\n",
          &seqHeapST->idStr[1], /*[0] is >*/
          outHiLowStr,
          outP2VirlStr,
          p1ToP6AaStr[0],
          p1ToP6AaStr[1],
          p1ToP6AaStr[2],
          p1ToP6AaStr[3],
          p1ToP6AaStr[4],
          p1ToP6AaStr[5],
          HA2StartUL + 1   /*Convert to index 1*/
       );

       /*************************************************\
       * Main Sec05 Sub05:
       *  - Clean up and exit
       \*************************************************/

        freeHeap_seqST(seqHeapST);
        seqHeapST = 0;

        freeStack_seqST(&seqStackST);
        freeStack_alnSetST(&alnStackST);

        exit(0);
    } /*If: have an HA2 starting position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^  - Find HA2 start & then if high/low path
   ^  - only goes if user did not provided a start
   ^  o main sec06 sub01:
   ^    - Open file and read in all sequences
   ^  o main sec06 sub02:
   ^    - Check if the sequence maps to the HA2 consensus
   ^  o main sec06 sub03:
   ^    - Check if have all thre bases in the P1 position
   ^      and if so, move to P1'
   ^  o main sec06 sub04:
   ^    - Get the P1 to P6 amino acid sequence
   ^  o main sec06 sub05:
   ^    - Check high path/low path & other P1-P6 markers
   ^  o main sec06 sub06:
   ^    - Print out the results
   ^  o main sec06 sub07:
   ^    - Clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *  - Open file and read in all sequences
   \*****************************************************/

   testFILE = fopen(fastaStr, "r");

   while(! getFaSeq_seqST(testFILE, &seqStackST))
   { /*Loop: Get each sequence & check if high/low path*/

       /*Get rid of new line at end of header*/
       seqStackST.idStr[seqStackST.lenIdUL - 1] ='\0';

      /**************************************************\
      * Main Sec06 Sub02:
      *  - Check if the sequence maps to the HA2 consensus
      \**************************************************/

       HA2AlnScoreL =
          find_haStart(
             &seqStackST,
             &alnStackST,
             &HA2StartUL,
             &HA2ConFirstBaseUL
      ); /*Find the HA2 starting position*/

      if(HA2AlnScoreL < 0)
      { /*If: had a memory error*/
         freeStack_seqST(&seqStackST);
         freeStack_alnSetST(&alnStackST);

         fclose(testFILE);

         fprintf(stderr, "Ran out of memory\n");
         exit(-1);
      } /*If: had a memory error*/

      /*check if this aligment met the minimum score*/
      if(HA2AlnScoreL == 0)
      { /*If: the sequence did not map, let user know*/
         fprintf(
            outFILE,
            "%s\tNA\tNA\tNo-alignment\tNA\n",
            &seqStackST.idStr[1] /*Skip > at start*/
         );

         continue;
      } /*If: the sequence did not map, let user know*/

      /**************************************************\
      * Main Sec06 Sub03:
      *  - Check if have all thre bases in the P1 position
      *    and if so, move to P1'
      \**************************************************/


      /*If the first base in the P1 codon did not map,
      ` This means it is not a lysine or argine and so, is
      ` likely not the start of the HA2 sequence
      */
      if(HA2ConFirstBaseUL > 0)
      { /*If: the 1st base in the P1 codon did not map*/
         /*See if we can find P1' (print out for user*/
         if(HA2ConFirstBaseUL < 2)
             HA2StartUL += 3;

         fprintf(
            outFILE,
            "%s\tNA\tNA\tMissing_1st_P1_base\t%lu\n",
            &seqStackST.idStr[1], /*Skip > at start*/
            HA2StartUL + 1         /*Convert to index 1*/
         );

         continue;
      } /*If: the 1st base in the P1 codon did not map*/

      HA2StartUL += 3; /*1st 3 bases is P1, I need P1'*/

      /**************************************************\
      * Main Sec06 Sub04:
      *  - Get the P1 to P6 amino acid sequence
      \**************************************************/

       getP1_P6AA_haPath(
          seqStackST.seqStr,
          p1ToP6AaStr,
          HA2StartUL
       );

      /**************************************************\
      * Main Sec06 Sub05:
      *  - Check high path/low path & other P1-P6 markers
      \**************************************************/

       hiPathBl = find_haPath(p1ToP6AaStr);

       if(hiPathBl)
          outHiLowStr = "high_path";
       else
          outHiLowStr = "low_path";

       p2VirulBl = P2PheTryMut_haPath(p1ToP6AaStr);

       if(p2VirulBl)
          outP2VirlStr = "P2=True";
       else
          outP2VirlStr = "P2=False";

      /**************************************************\
      * Main Sec06 Sub06:
      *  - Print out the results
      \**************************************************/

       fprintf(
          outFILE,
          "%s\t%s\t%s\tP1'-%c-%c-%c-%c-%c-%c\t%lu\n",
          &seqStackST.idStr[1], /*[0] is >*/
          outHiLowStr,
          outP2VirlStr,
          p1ToP6AaStr[0],
          p1ToP6AaStr[1],
          p1ToP6AaStr[2],
          p1ToP6AaStr[3],
          p1ToP6AaStr[4],
          p1ToP6AaStr[5],
          HA2StartUL + 1  /*Convert to index 1*/
       );
    } /*Loop: Get each sequence & check if high/low path*/

    /***************************************************\
    * Main Sec06 Sub07:
    *  - Clean up and exit
    \***************************************************/

    fclose(testFILE);
    testFILE = 0;

    freeStack_seqST(&seqStackST);

    /*more for future, currently does nothing*/
    freeStack_alnSetST(&alnStackST);

    exit(0);
} /*main*/

