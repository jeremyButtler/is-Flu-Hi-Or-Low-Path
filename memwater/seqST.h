/*#########################################################
# Name: seqStruct
#   - Holds functions for reading in or manipulating
#     sequences
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - guards
'  o .h st01: seqStruct
'    - Holds an single sequence (fasta/fastq)
'  o .c fun01 addLine_seqST:
'    - Add characters from file to buffer, if needed 
'      resize. This will only read in till the end of the
'      line
'  o fun02 getFqSeq_seqST:
'    - Reads a fastq sequence from a fastq file
'  o fun03 getFaSeq_seqST:
'    - Grabs the next read in the fasta file
'  o .c fun04 complementBase:
'    - Returns the complement of a base
'  o fun05 revComp_seqST:
'    - Reverse complement a sequence
'  o .h fun06 blank_seqST:
'    - Sets values in seqST to zero
'  o .h fun07 init_seqST:
'     - Sets values in seqST to blank values
'  o fun08 freeStack_seqST:
'    - Frees variables in an seqStruct (calls blank_seqST)
'  o fun09 freeHeap_seqST:
'    - Frees an seqST strucuter (calls fredSeqSTStack)
'  o fun10: freeAry_seqST
'    - Frees an array of seqStruct's
'  o fun11 cpIdEndPad_seqST:
'     - Copies read id to a buffer and adds in endIdC to
'       the end. If needed, this function will add right
'       padding of spaces to the end.
'  o fun12: cp_seqST
'    - Copies an seqStruct structure
'  o license:
'    - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - guards
\--------------------------------------------------------*/

#ifndef SEQUENCE_STRUCTURE_H
#define SEQUENCE_STRUCTURE_H

#define def_EOF_seqST 1
#define def_fileErr_seqST 2
#define def_badLine_seqST 16 /*invald fastx entry*/
#define def_memErr_seqST 4

/*--------------------------------------------------------\
| ST01: seqStruct
|  - Holds an single sequence (fasta/fastq)
\--------------------------------------------------------*/
typedef struct seqStruct
{ /*seqStruct*/
  char *idStr;               /*Id of th sequence*/
  unsigned long  lenIdUL;     /*Length of sequence id*/
  unsigned long  lenIdBuffUL; /*Lenght of Id buffer*/

  char *seqStr;              /*Sequence*/
  unsigned long  lenSeqUL;    /*Length of the sequence*/
  unsigned long  lenSeqBuffUL;/*Length sequence buffer*/

  char *qStr;                /*q-score entry*/
  unsigned long  lenQUL;      /*Length of the Q-score*/
  unsigned long  lenQBuffUL;  /*Length of Q-score buffer*/

  unsigned long  offsetUL;    /*Offset for an alignment*/
  unsigned long  endAlnUL;    /*Marks end of alignment*/
}seqStruct;

/*-------------------------------------------------------\
| Fun02: readRefFqSeq
|  - Grabs the next read in the fastq file
| Input:
|  - fqFILE:
|    o Pointer to FILE handle to a fastq file to get the
|      next read from
|  - seqST:
|    o Pointer to seqStruct structure to hold next read
| Output:
|  - Modifies:
|    o refStruct to hold the read in fastq entry & sets
|      its pointers
|  - Returns:
|     o 0: for no errors
|     o def_EOF_seqST: if EOF
|     o def_fileErr_seqST If file was not a fastq file
|     o def_badLine_seqST | def_fileErr_seqST for  a
|       invalid fastq entry
|     o def_memErr_seqST If malloc failed to find memory
\-------------------------------------------------------*/
unsigned char
getFqSeq_seqST(
  void *fqFILE,           /*fastq file with sequence*/
  struct seqStruct *seqST /*Will hold one fastq entry*/
);

/*-------------------------------------------------------\
| Fun03 TOC: getFaSeq_seqST
|  -  Grabs the next read in the fasta file
| Input:
|  - faFILE:
|    o Pointer to FILE handle to a fasta file to get the
|      next read from
|  - seqST:
|    o Pointer to seqStruct structure to hold next read
| Output:
|  - Modifies:
|    o seqST to hold one fasta entry
|  - Returns:
|     o 0: for no errors
|     o def_EOF_seqST: if EOF
|     o def_fileErr_seqST If file was not a fastq file
|     o def_badLine_seqST | def_fileErr_seqST for  a
|       invalid fasta entry
|     o def_memErr_seqST If malloc failed to find memory
| Note:
|   - This will remove new lines from the sequence.
|   - This will only remove spaces or tabs at the end of
|     each sequence entry, so "atg atc \n" will got to
|     "atc atc".
\-------------------------------------------------------*/
unsigned char
getFaSeq_seqST(
  void *faFILE,           /*Fasta file with sequence*/
  struct seqStruct *seqST /*Will hold one fastq entry*/
);

/*-------------------------------------------------------\
| Fun05: revComp_seqST
|  - Reverse complement a sequence
| Input:
|  - seqST:
|    o Pointer to seqStruct structure with a sequence to
|      reverse complement
| Output:
|  - Modfies
|    o Reverse complements the sequence entry and if
|      reverses the Q-score entry
\-------------------------------------------------------*/
void
revComp_seqST(
  struct seqStruct *seqST  /*Sequence to reverse comp*/
);


/*-------------------------------------------------------\
| Fun06: blank_seqST
|  - Sets values in seqST to blank values
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to blank
| Output:
|  - Modifies
|    o Sets id, sequence, and Q-score entreis to start
|      with '\0' and the id, sequence, and Q-score lengths
|      to 0. This does not change the buffer lengths.
\-------------------------------------------------------*/
#define \
blank_seqST(seqSTPtr){\
   if((seqSTPtr)->idStr)\
      *(seqSTPtr)->idStr = '\0';\
   \
   (seqSTPtr)->lenIdUL = 0;\
   \
   if((seqSTPtr)->seqStr)\
      *(seqSTPtr)->seqStr='\0';\
   \
   (seqSTPtr)->lenSeqUL = 0;\
   \
   if((seqSTPtr)->qStr)\
      *(seqSTPtr)->qStr = '\0';\
   \
   (seqSTPtr)->lenQUL = 0;\
   \
   (seqSTPtr)->offsetUL = 0;\
   (seqSTPtr)->endAlnUL = 0;\
} /*blank_seqST*/

/*-------------------------------------------------------\
| Fun07: init_seqST
|  - Sets vlues in seqST to zero
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to initialize
| Output:
|  - Modifies
|    o all values in seqST to be 0
\-------------------------------------------------------*/
#define \
init_seqST(seqSTPtr){\
   (seqSTPtr)->idStr = 0;\
   (seqSTPtr)->lenIdBuffUL = 0;\
   \
   (seqSTPtr)->seqStr = 0;\
   (seqSTPtr)->lenSeqBuffUL = 0;\
   \
   (seqSTPtr)->qStr = 0;\
   (seqSTPtr)->lenQBuffUL = 0;\
   \
   blank_seqST((seqSTPtr));\
} /*init_seqST*/

/*-------------------------------------------------------\
| Fun08: freeStack_seqST
|  - Frees the variables in an seqStruct strucuter
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer that has variables
|      to free
| Output:
|  - Frees
|    o seqSTPtr-idStr and sets to 0
|    o seqSTPtr-seqStr and sets to 0
|    o seqSTPtr-qStr and sets to 0
\-------------------------------------------------------*/
void
freeStack_seqST(
   struct seqStruct *seqSTPtr
);

/*-------------------------------------------------------\
| Fun09: freeHeap_seqST
|  - Frees the seqST strucuter
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to free
| Output:
|  - Frees
|    o seqST from memory and sets to 0
\-------------------------------------------------------*/
void
freeHeap_seqST(
   struct seqStruct *seqSTPtr
);

/*-------------------------------------------------------\
| Fun10: freeAry_seqST
|  - Frees an array of seqStruct's
| Input:
|  - seqSTAryPtr:
|    o Pointer to an arrya of seqStruct structuer to free
|  - numSeqSI:
|    o Number of seqStructs in the seqStruct array
|     (seqSTAryPtr)
| Output:
|  - Frees
|    o seqST from memory
\-------------------------------------------------------*/
void
freeAry_seqST(
   struct seqStruct *seqSTAryPtr,
   int  numSeqSI
);

/*-------------------------------------------------------\
| Fun11: cpIdEndPad_seqST
|  - Copies read id to a buffer and adds in endIdC to
|    the end. If needed, this function will add right
|    padding of spaces to the end.
| Input:
|  - seqST:
|    o Pointer to seqStruct with read id to copy
|  - buffStr:
|    o c-string to copy read id to
|  - endIdC:
|    o character to add in to mark the end of the read id
|  - padRI:
|    o Amount of padding to add to the right of the read
|      id
| Output:
|  - Modifies:
|    o buffStr to hold the sequence id + endIdC. If the
|      id + endIdC is shorter than padRI, the copied id is
|      padded with spaces on the right till it is the same
|      size as padRI.
|  - Returns:
|    o Pointer to end of copied id or padding if padding
|      was applied.
\-------------------------------------------------------*/
char *
cpIdEndPad_seqST(
   struct seqStruct *seqST, /*Has read id to copy*/
   char *buffStr,           /*Buffer to add read id to*/
   char endIdC,      /*padding for end of id (0 to skip)*/
   int padRI         /*Padding to add to right of id*/
);

/*-------------------------------------------------------\
| Fun12: cp_seqST
|   - Copies an seqStruct structure
| Input:
|   - dupSeqST:
|     o Pointer to an seqStruct to copy to (duplicate)
|   - cp_seqST:
|     o Pointer to an seqStruct to copy
| Output:
|   - Modifies:
|     o dupSeqST to be an copy of cp_seqST
|   - returns:
|     o 0 for no errors
|     o def_memErr_seqST for an memory error
\-------------------------------------------------------*/
signed char
cp_seqST(
   struct seqStruct *dupSeqST, /*copy to (duplicate)*/
   struct seqStruct *cpseqST   /*seqStruct to copy*/
);

#endif
