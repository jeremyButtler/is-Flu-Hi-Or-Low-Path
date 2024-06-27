/*########################################################
# Name memWater
# Use:
#  o Holds functions doing a memory efficent Smith
#    Waterman pairwise alignments.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of Functions
'   o header:
'     - header guards
'   o .c fun01: macroMax
'     - Find the maximum value (branchless)
'   o .c fun02: macroIfMax
'     - Set a value (ret) to a value based on which value
'       is greater.
'   o .c fun03: indexToQry
'     - Gets the query coordinates of the query sequence
'       in an matrix.
'   o .c fun04: indexToRef
'     - Gets the coordinates of the reference sequence in
'       in an matrix.
'   o .c fun05: indexToCoord
'     - Gets the coordinates of the reference and query
'       sequence in an matrix.
'   o .c fun06: waterMaxScore
'     - Maximizes the score for a single base pair in an
'       waterman query reference scan
'   o .c fun07: waterMaxEndRowScore
'     - Maximizes the score for a single waterman
'       alignment when at the end of an row
'   o .c fun08: scoreGt0
'     - Checks to see if the score is greater then zero.
'       if not, this resets the input values
'   o fun09 memWater:
'     - Run a memory efficent Waterman Smith alignment on
'       input sequences
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - header guards
\-------------------------------------------------------*/

#ifndef MEMORY_EFFICENT_WATERMAN_H
#define MEMORY_EFFICENT_WATERMAN_H

/*-------------------------------------------------------\
| Fun09: memWater
|   - Performs a memory efficent Smith Waterman alignment
|     on a pair of sequences
| Input;
|   - qrySeqSTVoidPtr:
|     o Point to an seqStruct with the query sequence and
|       index 0 coordinates to start (offsetUL)/end
|       (endAlnUL) the alignment.
|   - refSeqSTVoidPtr:
|     o Point to an seqStruct with the reference sequence
|       and index 0 coordinates to start (offsetUL)/end
|       (endAlnUL) the alignment.
|   - refStartUL:
|     o Pointer to unsigned long to hold the frist
|       reference base in the alignment
|   - refEndUL:
|     o Pointer to unsigned long to hold the last
|       reference base in the alignment
|   - qryStartUL:
|     o Pointer to unsigned long to hold the frist query 
|       base in the alignment
|   - qryEndUL:
|     o Pointer to unsigned long to hold the last query
|       base in the alignment
|   - alnSetVoidPtr:
|     o Pointer to an alnSet structure with the gap open,
|       gap extend, and scoring matrix for the alingment
| Output:
|  - Modifies:
|    o refStartUL to have 1st reference base in alignment
|    o refEndUL to have last reference base in alignment
|    o qryStartUL to have first query base in alignment
|    o qryEndUL to have last query base in alignment
|  - Returns:
|    o Score for aligment
|    o 0 for memory errors
\-------------------------------------------------------*/
long
memWater(
    void *qrySeqSTVoidPtr,
    void *refSeqSTVoidPtr,
    unsigned long *refStartUL,
    unsigned long *refEndUL,
    unsigned long *qryStartUL,
    unsigned long *qryEndUL,
    void *alnSetVoidPtr      /*Settings for alignment*/
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
