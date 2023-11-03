/*#########################################################
# Name: hiLowPathFun
# Use:
#  - Holds functions used to determine if a flu strain is
#    a high path or low path strain
# Libraries
#  - "dataTypeShortHand.h" (No .c file)
#  - "hiLowPathTbls.h"     (No .c file)
# C Stanard Libraries:
#  - <stdlib.h>
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hiLowPathFun SOF: Start Of File
'  - Holds functions to find if flu strain is hi/low path
'  o Header:
'    - Has the libraries and definitions for this file
'  o fun-01 isHiOrLowPath:
'    - determines if the cleavege sight is an high or low
'      path cleave site.
'  o fun-02 isP2PheTryMut:
'    - determines if the P2 site is an phe or try. This was
'      found to be more virulent in H1N1.
'  o fun-03 getP1ToP6AA:
'    - Get the cleavege site (P1 to P6) amino acid sequence
'      from the flu HA gene.
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Has the libraries and definitions for this file
\--------------------------------------------------------*/

#ifndef HI_LOW_PATH_FUN_H
#define HI_LOW_PATH_FUN_H

#include <stdlib.h>
#include "alnSeq/dataTypeShortHand.h"
#include "hiLowPathTbls.h"

#define defNumPAA 6

/*--------------------------------------------------------\
| Name: isHiOrLowPath (Fun-01:)
| Use:
|  - determines if the cleavege sight is an high or low
|    path cleave site.
| Input:
|  - pxAaStr:
|    o Array with the cleaved site (P1 to P6) ammino acids
| Output:
|  - Returns:
|    o 1 if is a high path cleavege site
|    o 0 if is a low path cleavege site
\--------------------------------------------------------*/
#define isHiOrLowPath(pxAaStr)(\
     ((pxAaStr)[0] == 'r')\
   & (   ((pxAaStr)[1] == 'r')\
       | ((pxAaStr[1]) == 'k')\
       | ((pxAaStr)[2] =='k')\
       | ((pxAaStr)[2] =='r')\
     )\
   /*See if this is high path by checking the minimum
   ` consensus for hi path; RXK or RR (from PMID 19782482)
   */\
) /*isHiOrLowPath*/

/*--------------------------------------------------------\
| Name: isP2PheTryMut (Fun-02:)
| Use:
|  - determines if the P2 site is an phe or try. This was
|    found to be more virulent in H1N1
| Input:
|  - H1H2CleaveST:
|    o H1H2Cleave structure with the P1 to P6 amino acids
|      in the cleavege site (aa found by getP1ToP6AA)
| Output:
|  - Returns:
|    o 1 if there is an phe or try at the P2 site
|    o 0 if no phe or try at the P2 site
\--------------------------------------------------------*/
#define isP2PheTryMut(pxAaStr)\
   (((pxAaStr)[1] == 'p') | ((pxAaStr)[1] == 'y'))
   /*This P2 virulence mutation is from PMID 20554779*/

/*--------------------------------------------------------\
| Name: getPiToP6AA (Fun-03:)
| Use:
|  - Find the amino acids for the P1 to P6 (cleavege site)
|    positions in the HA gene
| Input:
|  - HaSeqStr:
|    - String with the sequence of the HA gene
|  - startH2UL:
|    - Starting point of the HA2 segement in the HA gene
| Output:
|  - Returns:
|    o c-string with the P1 (index 0) to P6 (index 5)
|      ammino acids.
\--------------------------------------------------------*/
inline static char * getP1ToP6AA(
   char * HaSeqStr,  /*Ha sequence to precess*/
   ulong startH2UL   /*fist base in H2 gene (index 0)*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-03: getP1ToP6AA
   '  - Get the cleavege site (P1 to P6) amino acid
   '    sequence for flu
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   uchar oneUC = 0;
   uchar twoUC = 0;
   uchar threeUC = 0;
   char *pxAaStr = malloc(sizeof(char) * (defNumPAA + 1)); 

   if(pxAaStr == 0) return 0;

   *(pxAaStr + defNumPAA) = '\0'; /*Make sure a C-string*/

   /*Move to the last base in the P1 codon*/
   HaSeqStr = HaSeqStr + startH2UL - 1;

   /*Find the P1, P2, P3, P4, P5, and P6 ammino acids*/
   for(uchar ucCodon = 0; ucCodon < defNumPAA; ++ucCodon)
   { /*Loop: Get P2 to P6 amino acids*/
      threeUC = baseToCodeLkTbl[(uchar) *HaSeqStr];
      --HaSeqStr;

      twoUC = baseToCodeLkTbl[(uchar) *HaSeqStr];
      --HaSeqStr;

      oneUC = baseToCodeLkTbl[(uchar) *HaSeqStr];
      --HaSeqStr;

      pxAaStr[ucCodon] = codonLkTbl[oneUC][twoUC][threeUC];
   } /*Loop: Get P2 to P6 amino acids*/

   return pxAaStr;
} /*hiLowPath*/

#endif
