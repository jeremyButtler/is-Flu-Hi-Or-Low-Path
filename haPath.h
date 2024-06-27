/*########################################################
# Name: hiPath
# Use:
#  - Holds functions used to determine if a flu strain is
#    a high path or low path strain
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hiLowPathFun SOF: Start Of File
'  - Holds functions to find if flu strain is hi/low path
'  o Header:
'    - defined variable and guards
'  o .c tbl01 baseToCodeLkTbl:
'    - Table to convert bases to codes used in the codon
'      table.
'  o .c tbl02 aaTbl_haPath:
'    - Table to convert three bases to codons
'  o fun01: find_haPath
'    - determines if the cleavege sight is an high or low
'      path cleave site.
'  o fun02: P2PheTryMut_haPath
'    - determines if the P2 site is an phe or try. Was
'      found to be more virulent in H1N1.
'  o fun03: getP1_P6AA_haPath:
'    - Get the cleavege site (P1 to P6) amino acid
'      sequence from the flu HA gene.
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - defined variable and guards
\-------------------------------------------------------*/

#ifndef HI_LOW_PATH_FUN_H
#define HI_LOW_PATH_FUN_H

#define def_numPSites_haPath 6

/*-------------------------------------------------------\
| Fun01: find_haPath
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
\-------------------------------------------------------*/
#define \
find_haPath( \
   pxAaStr \
)(\
     ((pxAaStr)[0] == 'r')\
   & (   ((pxAaStr)[1] == 'r')\
       | ((pxAaStr[1]) == 'k')\
       | ((pxAaStr)[2] =='k')\
       | ((pxAaStr)[2] =='r')\
     )\
   /*See if this is high path by checking the minimum
   ` consensus for hi path; RXK or RR (from PMID 19782482)
   */\
) /*find_haPath*/

/*-------------------------------------------------------\
| Fun02: P2PheTryMut_haPath
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
\-------------------------------------------------------*/
#define \
P2PheTryMut_haPath( \
   pxAaStr \
) (((pxAaStr)[1] == 'p') | ((pxAaStr)[1] == 'y'))
   /*This P2 virulence mutation is from PMID 20554779*/

/*-------------------------------------------------------\
| Fun03: getP1_P6AA_haPath
| Use:
|  - Find the amino acids for the P1 to P6 (cleavege site)
|    positions in the HA gene
| Input:
|  - HaSeqStr:
|    - String with the sequence of the HA gene
|  - p1ToP6AA:
|    - string to hold p1 to p6 aa sequence
|    - length of 7 (6 aa + '\0')
|  - startHAUL:
|    - Starting point of the HA2 segement in the HA gene
| Output:
|  - Modifies:
|    o p1ToP6Str to hold P1 (index 0) to P6 (index 5)
|      ammino acids.
\-------------------------------------------------------*/
void
getP1_P6AA_haPath(
   char *HaSeqStr,      /*Ha sequence to precess*/
   char *p1ToP6Str,     /*will have P1 to P6 aa sequence*/
   unsigned long startHaUL   /*Ha2 gene start (index 0)*/
);

#endif
