/*#########################################################
# Name: seqStruct
#   - Holds functions for reading in or manipulating
#     sequences
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - Has includes and default variables
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
'  O .h fun07 init_seqST:
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
'    - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Has includes and definede variables
\--------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "seqST.h"

#include <stdio.h>

#include "../generalLib/ulCp.h"

/*-------------------------------------------------------\
| Fun01: addLine_seqST
|  - Read line of characters into the buffer.If needed
|    this will resize the buffer.
| Input:
|  - buffCStr:
|    o Pionter to c-string to add buffer into.
|  - lenBuffUL:
|    o Pionter to the size of buffCStr
|  - curBuffUL:
|    o Pointer to the number of elements in buffCStr  
|  - resBuffUL:
|    o How much to expand buffer by when the buffer is
|      full
|  - inFILE:
|    o pointer to FILE handle to get the next lein from
| Output:
|   - Modifies:
|     o buffCStr to hold the next line.
|       - buffCStr is resizied if it is to small to hold
|         the next line.
|       - buffCStr + lenBuffUL - 2 will be '\0' or '\n'
|       - buffCStr set to 0 for memory allocation errors
|     o curBuffUL: Has the number of chars in the buffer
|     o lenBuffUL: Has the buffer size
|     o inFILE: Points to next line in file
|   - Returns:
|     o 0 if no errors
|     o def_fileErr_seqST if was end of file (EOF)
|     o def_memErr_seqST if had a memory allocation error
\-------------------------------------------------------*/
unsigned char
addLine_seqST(
    char **buffCStr,         /*Buffer to add data to*/
    unsigned long *lenBuffUL,/*Size of the buffer*/
    unsigned long *curBuffUL,/*Number of chars in buffer*/
    unsigned long resBuffUL, /*How much to resize buff by*/
    void *inFILE             /*File to grab data from*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC: addLine_seqST
   '  - Read line of characters into the buffer.If needed
   '    this will resize the buffer.
   '   o fun01 sec-1:
   '     - variable declerations
   '   o fun01 sec-2:
   '     - Check if need to resize the buffer
   '   o fun01 sec-3:
   '     - Read in the next line in the buffer
   '   o fun01 sec-4:
   '     - If at end of file, update read in lengths
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun01 Sec-1:
    ^  - variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *tmpCStr = 0;
    unsigned long spareBuffUL = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun01 Sec-2:
    ^  - Check if need to resize the buffer
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(*curBuffUL == 0 && *lenBuffUL > 0)
        spareBuffUL = *lenBuffUL;

    else if(*lenBuffUL == 0 || *curBuffUL-1 >= *lenBuffUL)
    { /*If need to resize the buffer (-1 for 0 index)*/
        *lenBuffUL += resBuffUL - 1;
           /*-1 to account for adding 2 index one items*/

        if(*lenBuffUL == 0)
          *buffCStr=malloc(sizeof(char) * (*lenBuffUL+1));
        else
        { /*Else I need to resize the buffer*/
            *buffCStr =
              realloc(
                *buffCStr,
                sizeof(char) * (*lenBuffUL + 1)
            ); /*Resize hte buffer*/
        } /*Else I need to resize the buffer*/
       
        if(*buffCStr == 0)
            return def_memErr_seqST; /*Memory error*/

        /*Amount of extra space in the buffer*/
        spareBuffUL = resBuffUL - 1;/*resBuff is index 1*/
    } /*If need to resize the buffer*/

    else
        spareBuffUL = *lenBuffUL - *curBuffUL;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun01 Sec-3:
    ^  - Read in the next line in the buffer
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    
    /*Set up marker to catch when read in the entire line*/
    *(*buffCStr + *lenBuffUL - 2) = '\0';
    tmpCStr = *buffCStr + *curBuffUL;

    while(fgets(tmpCStr, spareBuffUL, (FILE *) inFILE))
    { /*While I have lines to read*/

        if(*(*buffCStr + *lenBuffUL - 2) == '\0' ||
           *(*buffCStr + *lenBuffUL - 2) == '\n'
        ) { /*If read in the line*/

            /*Check if I used the entire buffer*/
            if(*(*buffCStr + *lenBuffUL - 2) == '\n')
                *curBuffUL = *lenBuffUL;
            else
            { /*Else only read in part of the buffer*/
                while(*tmpCStr != '\0')
                { /*While have characters in the buffer*/
                    ++tmpCStr;
                    ++(*curBuffUL);
                } /*While have characters in the buffer*/
            } /*Else only read in part of the buffer*/

            return 0; /*Finshed with line*/
        } /*If read in the line*/

        /*Else resize the buffer*/
        *curBuffUL = *lenBuffUL - 1; /*Filled up buffer*/
            /*-1 to account for 0 index*/
        *lenBuffUL += resBuffUL - 1;
           /*-1 to account for adding two index 1 items*/

        *buffCStr =
          realloc(
            *buffCStr,
            sizeof(char) * (*lenBuffUL + 1)
        ); /*Resize the buffer*/

        if(*buffCStr == 0)
            return def_memErr_seqST; /*Memory error*/

        /*Reset my maker for entire line read in*/
        *(*buffCStr + *lenBuffUL - 2) = '\0';
        spareBuffUL = resBuffUL;
        tmpCStr = *buffCStr + *curBuffUL;
    } /*While I have lines to read*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun01 Sec-4:
    ^  - If at end of file, update read in lengths
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(*(*buffCStr + *lenBuffUL - 2) == '\n')
        *curBuffUL = *lenBuffUL; /*used entire buffer*/
    else
    { /*Else only read in part of the buffer*/
        while(*tmpCStr != '\0')
        { /*While have characters in the buffer*/
            ++tmpCStr;
            ++(*curBuffUL);
        } /*While have characters in the buffer*/
    } /*Else only read in part of the buffer*/

    
    return def_EOF_seqST; /*End of file*/
} /*addLine_seqST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC: readRefFqSeq
   '  -  Grabs the next read in the fastq file
   '  o fun02 sec-01:
   '    - Variable declarations
   '  o fun02 sec-02:
   '    - Check if need to allocate memory for buffer
   '  o fun02 sec-03:
   '    - Read in the first data
   '  o fun02 sec-04:
   '    - If not at file end, see if have the full entry
   '  o fun02 sec-05:
   '    -Read till end of file, check if valid fastq entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun02 Sec-01:
    ^  - Variable declarations
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    char tmpC = 'C';           /*initilize sequence loop*/

    unsigned short extraBuffUS = 1024;
    unsigned long tmpBuffUL = 0;

    /*Holds number of lines in sequence entry*/
    unsigned char numLinesUC = 0;

    unsigned char errUC = 0;
    char *oldIterCStr = 0;

    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun02 Sec-02:
    ^  - Read in the header
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    if((FILE *) fqFILE == 0)
        return def_fileErr_seqST;    /*No file provided*/

    seqST->lenIdUL = 0;

    errUC =
        addLine_seqST(
            &seqST->idStr,     /*C-string; hold header*/
            &seqST->lenIdBuffUL,/*Length of header buff*/
            &seqST->lenIdUL,    /*Number bytes in buffer*/
            extraBuffUS,        /*Amount to increase buf*/
            (FILE *) fqFILE     /*Fq file with header*/
    ); /*Get the header (will resize as needed)*/


    if(errUC)
       return errUC; /*EOF or memory allocation error*/

    /*Account for the new line*/
    --seqST->lenIdUL;
    seqST->idStr[seqST->lenIdUL] = '\0';
    
    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun02 Sec-03:
    ^  - Read in the sequence & spacer
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    seqST->lenSeqUL = 0;
    /*need to set this up so the loop does not error out*/
    oldIterCStr = &tmpC;

    while(*oldIterCStr != '+')
    { /*While I have not reached the spacer entry*/
        /*So can get to this position later*/
        tmpBuffUL = seqST->lenSeqUL;

        errUC =
            addLine_seqST(
                &seqST->seqStr,    /*Buff to copy seq*/
                &seqST->lenSeqBuffUL,/*Size; seq buffer*/
                &seqST->lenSeqUL,    /*Lenth of sequence*/
                extraBuffUS,     /*resize buff by*/
                (FILE *) fqFILE  /*Fq file with sequence*/
        ); /*Get the header*/

        /*Set up new lines to be removed on the next read*/
        oldIterCStr = seqST->seqStr + seqST->lenSeqUL - 1;

        while(*oldIterCStr < 33)
        { /*While removing new lines*/
            --seqST->lenSeqUL;
            --oldIterCStr;
        } /*While removing new lines*/

        /*Check for memory errors (64) & invalid entries*/
        if(errUC & def_memErr_seqST)
           return errUC; 

        if(errUC & def_EOF_seqST)
           return def_badLine_seqST | def_fileErr_seqST;

        /*Get on first character in the new buffer*/
        oldIterCStr = seqST->seqStr + tmpBuffUL;
        ++numLinesUC; /*number of '\n' in sequence entry*/
    } /*While I have not reached the spacer entry*/

    --numLinesUC; /*Account for the overcounting*/

    oldIterCStr = seqST->seqStr + seqST->lenSeqUL;

    while(*oldIterCStr != '+')
    { /*While not at start of spacer entry*/
        --oldIterCStr;
        --seqST->lenSeqUL; /*Acount for white space*/
    } /*While not at start of spacer entry*/

    *oldIterCStr = '\0';  /*Set spacer to null*/
    --oldIterCStr;
    --seqST->lenSeqUL;    /*Acount for white space*/

    while(*oldIterCStr < 33)
    { /*While have white space at end to remove*/
        *oldIterCStr = '\0';
        --oldIterCStr;
        --seqST->lenSeqUL; /*Acount for white space*/
    } /*While have white space at end to remove*/
    
    ++seqST->lenSeqUL; /*Account for being one base off*/

    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun02 Sec-04:
    ^  - Read in the Q-score entry
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    seqST->lenQUL = 0;

    while(numLinesUC > 0)
    { /*While I need to read in the Q-score entry*/
        errUC =
            addLine_seqST(
              &seqST->qStr,     /*Buffer; q-score entry*/
              &seqST->lenQBuffUL,/*Size of buffer*/
              &seqST->lenQUL,   /*Length; Q-score entry*/
              extraBuffUS,      /*For reszing buffer*/
              (FILE *) fqFILE   /*Fq file with q-score*/
        ); /*Get the header*/

        /*Set up white space to removed on next read*/
        oldIterCStr = seqST->qStr + seqST->lenQUL - 1;

        while(*oldIterCStr < 33) /*removes white space*/
        { /*While removing new lines*/
            --seqST->lenQUL;
            --oldIterCStr;
        } /*While removing new lines*/

        /*Check for errors; memory=64/invalid file=130*/
        if(errUC & def_memErr_seqST)
           return errUC; 

        if(errUC & def_EOF_seqST)
           return def_badLine_seqST | def_fileErr_seqST;

        --numLinesUC; /*Number of lines to read in*/
    } /*While I need to read in the Q-score entry*/

    /*Remove any white space at the end*/
    oldIterCStr = seqST->qStr + seqST->lenQUL;

    while(*oldIterCStr < 33)
    { /*While have white space at end to remove*/
        *oldIterCStr = '\0';
        --oldIterCStr;
        --seqST->lenQUL; /*Acount for white space*/
    } /*While have white space at end to remove*/

    seqST->endAlnUL = seqST->lenSeqUL - 1;
    seqST->offsetUL = 0;

    return errUC; /*0 = more reads or def_EOF_seqST*/
} /*getFqSeq_seqST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC: getFaSeq_seqST
   '  -  Grabs the next read in the fasta file
   '  o fun03 sec-01:
   '    - Variable declarations
   '  o fun03 sec-02:
   '    - Check if need to allocate memory for buffer
   '  o fun03 sec-03:
   '    - Read in the sequence
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun03 Sec-01:
    ^  - Variable declarations
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    /*Used To initilize the sequence loop*/
    char tmpC = 'C';
    unsigned long tmpUL = 0;

    unsigned short extraBuffUS = 1024;
    unsigned char errUC = 0;
    char *oldIterCStr = 0;

    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun03 Sec-02:
    ^  - Read in the header
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    if((FILE *) faFILE == 0)
        return def_fileErr_seqST;    /*No file provided*/

    seqST->lenIdUL = 0;

    errUC =
        addLine_seqST(
            &seqST->idStr,     /*Buffer to hold header*/
            &seqST->lenIdBuffUL,/*Length of buffer*/
            &seqST->lenIdUL,    /*length of header*/
            extraBuffUS,        /*resize buff by*/
            (FILE *) faFILE     /*Fasta file with header*/
    ); /*Get the header (will resize as needed)*/

    if(errUC)
       return errUC; /*EOF or memory error*/

    /*Account for the new line*/
    --seqST->lenIdUL;
    seqST->idStr[seqST->lenIdUL] = '\0';

    /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
    ^ Fun03 Sec-03:
    ^  - Read in the sequence
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    /*need to set this up so the loop does not error out*/
    oldIterCStr = &tmpC;

    /*Overwrite the older sequence entry*/
    seqST->lenSeqUL = 0;

    if(seqST->seqStr != 0) *seqST->seqStr = '\0';

    while(*oldIterCStr != '>')
    { /*While I have not reached the spacer entry*/
        errUC =
            addLine_seqST(
                &seqST->seqStr,     /*Holds sequence*/
                &seqST->lenSeqBuffUL,/*size of seq buff*/
                &seqST->lenSeqUL,   /*Length of sequence*/
                extraBuffUS,
                (FILE *) faFILE    /*Fasta with sequence*/
        ); /*Get the header*/

        /*Sets up to remove newlines on the next read in*/
        oldIterCStr = seqST->seqStr + seqST->lenSeqUL -1;

        while(*oldIterCStr < 33)/*Remove end white space*/
        { /*While removing new lines*/
            --seqST->lenSeqUL;
            --oldIterCStr;
        } /*While removing new lines*/

        /*Check for errors: memory (64), EOF (0)*/
        if(errUC & def_memErr_seqST)
           return errUC; 

        if(errUC & def_EOF_seqST)
           break;

        /*Get on first character in the new buffer*/
        oldIterCStr = seqST->seqStr + seqST->lenSeqUL;

        /*Peek ahead in the file*/
        tmpUL = fread(&tmpC, 1, 1, (FILE *) faFILE);

        if(tmpUL == 0)
           break;

        fseek((FILE *) faFILE, -1, SEEK_CUR);

        if(tmpC == '>')
           break;
    } /*While I have not reached the spacer entry*/

    /*Check if is a valid fasta entry*/
    if(seqST->seqStr == 0 || *seqST->seqStr == '\0')
        return def_badLine_seqST | def_fileErr_seqST;

    /*Make sure the new line at the end is removed*/
    oldIterCStr = seqST->seqStr + seqST->lenSeqUL;

    while(*oldIterCStr < 33)
    { /*While I have white space at the end*/
        *oldIterCStr = '\0';
        --oldIterCStr;
        --seqST->lenSeqUL;
    } /*While I have white space at the end*/

    ++seqST->lenSeqUL; /*Account for being one base off*/

    seqST->endAlnUL = seqST->lenSeqUL - 1;
    seqST->offsetUL = 0;

    return errUC; /*is EOF or no error*/
} /*getFaSeq_seqST*/

/*-------------------------------------------------------\
| Fun04: complementBase
|  - Returns the complement of a base
| Input:
|  - baseC:
|    o base to get the complement of
| Output:
|  - Returns
|    o the complement of input base (0 if invalid base)
\-------------------------------------------------------*/
#define complementBase(baseC)({\
    char tmpC = 0;\
    switch((baseC) & (~32)) /*Make sure is upper case*/\
    { /*switch, reverse complement*/\
        case 'A': tmpC = 'T'; break;\
        case 'C': tmpC = 'G'; break;\
        case 'G': tmpC = 'C'; break;\
        case 'T': tmpC = 'A'; break;\
        case 'U': tmpC = 'A'; break;\
        case 'W': tmpC = 'W'; break;\
        case 'S': tmpC = 'S'; break;\
        case 'M': tmpC = 'K'; break;/*A/C to T/G*/\
        case 'K': tmpC = 'M'; break;/*T/G to A/C*/\
        case 'R': tmpC = 'Y'; break;/*A/G to T/C*/\
        case 'Y': tmpC = 'R'; break;/*T/C to A/G*/\
        case 'B': tmpC = 'V'; break;/*C/G/T to G/C/A*/\
        case 'D': tmpC = 'H'; break;/*G/T/A to C/A/T*/\
        case 'H': tmpC = 'D'; break;/*C/A/T to G/T/A*/\
        case 'V': tmpC = 'B'; break;/*A/C/G to T/G/C*/\
        case 'N': tmpC = 'N'; break;/*A/C/G/T->A/C/G/T*/\
        default: tmpC = 0;\
    } /*switch, reverse complement*/\
    \
    tmpC;\
}) /*complementBase*/

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
){
    char *seqStr = seqST->seqStr;
    char *endCStr = seqST->seqStr + seqST->lenSeqUL - 1;
    char *qStr = 0;
    char *qEndCStr = 0;
    char swapC = 0;

    if(seqST->qStr != 0 && *seqST->qStr != '\0')
    { /*If have a Q-score entry*/
      qStr = seqST->qStr;
      qEndCStr = seqST->qStr + seqST->lenQUL;
    } /*If have a Q-score entry*/

    while(endCStr > seqStr)
    { /*While I have bases to reverse complement*/
        swapC = *seqStr;
        *seqStr = complementBase(*endCStr);
        *endCStr = complementBase(swapC);
        
        if(qStr)
        { /*If I also need to swap Q-scores*/
          swapC = *qStr;
          *qStr = *qEndCStr;
          *qEndCStr = swapC;

          ++qStr;
          --endCStr;
        } /*If I also need to swap Q-scores*/
        
        ++seqStr;
        --endCStr;
    } /*While I have bases to reverse complement*/

    /*Check if ended on same base; if so complement base*/
    if(endCStr == seqStr)
        *seqStr = complementBase(*seqStr);

    return;
} /*revComp_seqST*/

/*-------------------------------------------------------\
| Fun06: reverseCStr
|  - Reverse a c-string to be backwards
|    (here for Q-score entries)
| Input:
|  - inCStr:
|    o c-string to reverse
|  - lenCStrUI:
|    o length of inCStr (as index 1)
| Output:
|  - Modifies
|    o inCStr to be backwards (end at start, start at end)
\-------------------------------------------------------*/
void
reverseCStr(
  char *inCStr,            /*C-string to refeverse*/
  unsigned long lenCStrUI  /*Length of dinCStr index 1*/
){
    char *endCStr = inCStr + lenCStrUI - 1;
    char swapC = 0;

    while(endCStr > inCStr)
    { /*While I have bases to reverse complement*/
        swapC = *inCStr;

        *inCStr = *endCStr;
        *endCStr = swapC;
        ++inCStr;
        --endCStr;
    } /*While I have bases to reverse complement*/

    return;
} /*reverseCStr*/

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
){
   if(seqSTPtr)
   { /*If: I have something to free*/
      free(seqSTPtr->idStr);
      seqSTPtr->idStr = 0;

      free(seqSTPtr->seqStr);
      seqSTPtr->seqStr = 0;

      free(seqSTPtr->qStr);
      seqSTPtr->qStr = 0;

      blank_seqST(seqSTPtr);
   } /*If: I have something to free*/\
} /*freeStack_seqST*/

/*-------------------------------------------------------\
| Fun09: freeHeap_seqST
|  - Frees the seqST strucuter
| Input:
|  - seqSTPtr:
|    o Pointer to an seqStruct structuer to free
| Output:
|  - Frees
|    o seqST from memory
\-------------------------------------------------------*/
void
freeHeap_seqST(
   struct seqStruct *seqSTPtr
){
   freeStack_seqST(seqSTPtr); /*checks if have null*/
   free(seqSTPtr);
} /*freeHeap_seqST*/

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
){
   int siFree = 0;

   for(
      siFree = 0;
      siFree < numSeqSI;
      ++siFree
   ){ /*Loop: Free the individual structures*/
      freeStack_seqST(&seqSTAryPtr[siFree]);
   } /*Loop: Free the individual structures*/

   free(seqSTAryPtr);
} /*freeAry_seqST*/

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
){
   int idCntI = 0;
   char *tmpStr = seqST->idStr;

   if(*tmpStr == '>') ++tmpStr; /*Get of header*/

   while(*tmpStr != '\0')
   { /*While I have a read id to copy over*/
      *buffStr = *tmpStr;
      ++buffStr;
      ++tmpStr;
      ++idCntI;
   } /*While I have a read id to copy over*/

   if(endIdC)
   { /*If adding in a character after the read id*/
      *buffStr = endIdC;
      ++buffStr; /*If not a null*/
      ++idCntI;
   } /*If adding in a character after the read id*/

   while(idCntI < padRI)
   { /*While I have padding to add to the end*/
      *buffStr = ' ';
      ++buffStr;
      ++idCntI;
   } /*While I have padding to add to the end*/

   return buffStr;
} /*cpIdEndPad_seqST*/

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
){
   freeStack_seqST(dupSeqST);

   dupSeqST->offsetUL = cpseqST->offsetUL;
   dupSeqST->endAlnUL = cpseqST->endAlnUL;
   
   dupSeqST->lenQUL = (cpseqST)->lenQUL;
   dupSeqST->lenQBuffUL = (cpseqST)->lenQUL + 1;
   
   dupSeqST->lenSeqUL = (cpseqST)->lenSeqUL;
   dupSeqST->lenSeqBuffUL=(cpseqST)->lenSeqUL +1;
   
   dupSeqST->lenIdUL = (cpseqST)->lenIdUL;
   dupSeqST->lenIdBuffUL=(cpseqST)->lenIdUL + 1;
   
   dupSeqST->idStr =
      malloc(dupSeqST->lenIdBuffUL * sizeof(char));
   
   if(! dupSeqST->idStr)
      return def_memErr_seqST;

   cpDelim_ulCp(dupSeqST->idStr, cpseqST->idStr, 0, '\0');

   dupSeqST->seqStr =
      malloc(dupSeqST->lenSeqBuffUL * sizeof(char));

   if(! dupSeqST->seqStr)
      return def_memErr_seqST;

   cpDelim_ulCp(dupSeqST->seqStr,cpseqST->seqStr,0,'\0');

   if(cpseqST->qStr)
   { /*If: I have an q-score entry*/
      dupSeqST->qStr =
         malloc(dupSeqST->lenSeqBuffUL * sizeof(char));

      if(! dupSeqST->qStr)
         return def_memErr_seqST;

      cpDelim_ulCp(dupSeqST->qStr,cpseqST->qStr,0,'\0');
   } /*If: I have an q-score entry*/

   return 0;
} /*cp_seqST*/
  
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
