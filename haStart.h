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
'    - guards and foward declerations
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
|  - guards and foward declerations
\-------------------------------------------------------*/

#ifndef HA_START_H
#define HA_START_H

typedef struct seqStruct seqStruct;
typedef struct alnSet alnSet;

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
);

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
);

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
);

#endif
