/*########################################################
# Name: ulCp
#   - uses longs to copy contents of a string
#   - These functions are slower than strcpy and strlen,
#     but they allow deliminators to be used.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries, definitions and preprocessor
'       checks
'   o fun01: cpLen_ulCp
'     - Copies cpStr into dupStr using unsigned longs
'   o fun02: mkDelim_ulCp
'     - Makes an unsigned long delimintor from a character
'       deliminator for use in cpStrDelim
'   o fun03: ifDelim_ulCp
'     - Detect if the input deliminator is in the input
'       unsigned long
'     - This is a bit slow (9 op), 8 if delimUL can be
'       deterimined at compile time. This is less efficent
'       on 32 and 16 bit cpus (no longer borderline).
'   o fun04: cpDelim_ulCp
'     - Copies cpStr into dupStr until delimC is found
'   o fun05: lenStr_ulCp
'     - Finds the length of a string using unsigned longs
'   o fun06: endLine_ulCp
'     - Finds the end of a c-string. This assumes that
'       the line ends in an '\0' or an '\n'
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries, definitions and preprocessor
|     checks
\-------------------------------------------------------*/

#ifndef UNSINGED_LONG_COPY_H
#define UNSINGED_LONG_COPY_H

#define def_bitsPerChar_ulCp 8
#define def_null_ulCp 0

#define def_charInUL_ulCp sizeof(unsigned long)
#define def_modUL_ulCp (sizeof(unsigned long )- 1)
   /*The mod values are 7 (longs), 3 (ints), and 1
   `  (shorts), so, 1 - number of bytes will give me the
   `  mod value
   */

/*This should be compliled in.
`  This will only work for up to 64 bit numbers, after
`  that it will fail. However, it is very unlikely a
`  long will by 128 bits.
*/
#define def_shiftULBy_ulCp (\
     (sizeof(unsigned long) >> 1)\
   - (sizeof(unsigned long) >> 3)\
)
/*Logic:
`   - sizeof(unsigned long) >> 1:
`     o This gets the number of bytes to shift to get
`       an 32 bit or 16 bit number to one bytes. Basically
`       it is divide by 2
`   - (sizeof(unsigned long) >> 3)
`     o This adds in 1 if I have eight bytes. It is needed
`       as the 64 bit correction step
`   - (sizeof(long) >> 1) - (sizeof(long) >> 3)
`     o Gives me the number of bits I need to shift to
`       get the number of characters in an long
`   - (sizeof(long) >> 3) << (sizeof(long) >> 4)
`     o This is not used, but would correct for a 128 bit
`       number. Each time I increase the bit size I need
`       to add a correction in
*/

/* This will work for any number, but might be slower if
`    it is not compiled in. It also needs to be called
`    like a function
*/
/*
#define \
def_shiftULBy_ulCp( \
)({\
   unsigned int tmpNumUI = 0;\
   unsigned int retUI = 0;\
   unsigned int shiftUI = 0;\
   \
   retUI = (sizeof(unsigned long) > 0xF) << 2;\
   tmpNumUI = sizeof(unsigned long) >> retUI;\
   \
   shiftUI = (tmpNumUI > 0x3) << 1;\
   tmpNumUI >>= shiftUI;\
   retUI |= shiftUI;\
   \
   retUI | (tmpNumUI >> 1);\
})
*/

/*-------------------------------------------------------\
| Fun01: cpLen_ulCp
|   - Copies cpStr into dupStr using unsigned longs
| Input:
|   - dupStr:
|     o Pointer to string to copy cpStr into
|   - cpStr:
|     o Pointer to string to copy
|   - lenUI:
|     o length of cpStr
| Output:
|   - Modifies:
|     o  dupStr to hold lenUI characters from cpStr
\-------------------------------------------------------*/
#define \
cpLen_ulCp( \
   dupStr, \
   cpStr, \
   lenUI \
){\
   unsigned long *macCpUL = (unsigned long *) (cpStr);\
   unsigned long *macDupUL = (unsigned long *) (dupStr);\
   unsigned int uiCharMac = 0;\
   \
   for(\
      uiCharMac= 0;\
      uiCharMac < ((lenUI) >> def_shiftULBy_ulCp);\
      ++uiCharMac\
   ) macDupUL[uiCharMac] = macCpUL[uiCharMac];\
   \
   for(\
      uiCharMac = ((lenUI) - ((lenUI) & def_modUL_ulCp));\
      uiCharMac < (lenUI);\
     ++uiCharMac\
   ) (dupStr)[uiCharMac] = (cpStr)[uiCharMac];\
   \
   (dupStr)[lenUI] = '\0';\
   /*Faster than doing *val1++ = *val2++*/\
} /*cpLen_ulCp*/

/*-------------------------------------------------------\
| Fun02: mkDelim_ulCp
|   - Makes an unsigned long delimintor from a character
|     deliminator for use in cpStrDelim
| Input:
|   - delimC:
|     o Deliminator to use to end a string copy
| Output:
|   - Modifies:
|     o  dupStr to hold the characters from cpStr
\-------------------------------------------------------*/
#define \
mkDelim_ulCp( \
   delimC \
)({\
   int iRepMac = 0;\
   unsigned long macDelimUL = (unsigned char) (delimC);\
   \
   for(\
      iRepMac = sizeof(unsigned long) << 2;\
      iRepMac >= def_bitsPerChar_ulCp;\
      iRepMac >>= 1\
   ) macDelimUL |= (macDelimUL << iRepMac);\
   \
   macDelimUL;\
}) /*ulMakeDelim*/
   /*Logic:
   `   - macDelimUL  = (unsigned char) delimC:
   `     o the (unsigned char) is needed to avoid negative
   `       values being converted to their long
   `       equivulents
   `   - sizeof(unsigned long) << 2:
   `     o Gets me to the half way point in bits. So
   `       8 (64 bit) becomes 32, 4 (32 bit) becomes 16,
   `       an 2 (16 bit) becomes 8
   `   - iRepMac << 1:
   `     o Divides the shift range by 2 each round, so
   `       that each round fills in a different part of
   `       the long
   `   - macDelimUL |= (macDelimUL << iRepMac);
   `     o Moves the deliminator up (shifts) to an
   `       unfilled position.
   */

/*-------------------------------------------------------\
| Fun03: ifDelim_ulCp
|   - Detect if the input deliminator is in the input
|     unsigned long
| Variations:
|   - SAFE_DELIM uses 9 to 8 OP for 64 bit, 7 to 6 OP for
|     for 32 bit, and 5 to 4 OP for 16 bit. The amount
|     depends on if the deliminator is hardcoded or not.
|   - ifndef SAFE_DELIM takes 5 op no matter what your
|     OS type is. You could reduce it down by removing the
|     (addUL & 1) + addUL step, but this does cause some
|     issues with values that are one off the actual. Tab
|     is safe, since it is odd, but not comma or space.
| Input:
|   - strUL:
|     o Unsigned long to check for the delimintor in
|   - delimUL:
|     o Unsinged long with delimintor (use ulMakeDelim
|       to get this value)
| Output:
|   - Returns:
|     o unsigned long == 0; strUL has no deliminator. The
|       position of the delimintor can be found with
|       log2(returned unsigned long) >> 8. The 1 is
|       shifted to the left of the character.
|     o unsigned long > 0; strUL has deliminator
\-------------------------------------------------------*/
#define \
ifDelim_ulCp( \
   strUL, \
   delimUL \
)({\
   unsigned long macAddUL = (strUL) ^ (delimUL);\
   macAddUL = ( (macAddUL & 1) + macAddUL );\
   macAddUL -= mkDelim_ulCp(0x01);\
   macAddUL & mkDelim_ulCp(0x80); /*Answer to return*/\
})
   /*Logic:
   ` - addUL = strUL ^ delimUL:
   `   o Converts all values matching delimUL to 0, and
   `     everything else to > 0. Saves result to addUL
   ` - (addUL & 1) + addUL
   `   o Convets odd numbers to even numbers. This gets
   `     means there is no 1;
   `   o This is needed to ensure -1 will not cause an
   `     overflow when 1 is next to an deliminator. The
   `     & 1 ensures that -1 will alwas remove the sign
   `     bit, if one character was one off.
   ` - ((addUL & 1) + addUL) - defOne
   `   o subtracts one from every character. This results
   `     in an overflow for only 0, while every other
   `     value is >= 1, but can go to 0 if next number
   `     overflowed. This sets the value 0 numbers to -1
   ` - (( (addUL & 1) + addUL) - defOne) & defHiBit
   `   o This keeps only the overflow bit (sign bit). This
   `     means that every non-deliminator value becomes
   `     0.
   */
   
/*-------------------------------------------------------\
| Fun04: cpDelim_ulCp
|   - Copies cpStr into dupStr until delimC is found
| Input:
|   - dupStr:
|     o Pointer to string to copy cpStr into
|   - cpStr:
|     o Pointer to string to copy
|   - delimUL:
|     o Deliminatro to end at (as long). Use makeULDelim
|       to build this deliminator
| Output:
|   - Modifies:
|     o  dupStr to hold the characters from cpStr
| Note:
|   - This will likely not be very good at copying short
|     strings.
\-------------------------------------------------------*/
#define \
cpDelim_ulCp( \
   dupStr, \
   cpStr, \
   delimUL, \
   delimC \
)({\
   unsigned long *macCpUL = (unsigned long *) (cpStr);\
   unsigned long *macDupUL = (unsigned long *) (dupStr);\
   char *dupMacStr = 0;\
   char *cpMacStr = 0;\
   unsigned long macAtDelimUL = 0;\
   unsigned int uiCharMac = 0;\
   \
   macAtDelimUL =ifDelim_ulCp(*macCpUL, (delimUL));\
   \
   while(!macAtDelimUL)\
   { /*Loop: Copy cpStr to dupStr*/\
      *macDupUL++ = *macCpUL++;\
      macAtDelimUL = ifDelim_ulCp(*macCpUL, (delimUL));\
   } /*Loop: Copy cpStr to dupStr*/\
   \
   cpMacStr = (char *) macCpUL;\
   dupMacStr = (char *) macDupUL;\
   /*Loop: Till macAtDelimUL says there is no longer
   ` a deliminator. The last character in the string
   ' is at the end
   */\
   while(*cpMacStr != (delimC))\
      *dupMacStr++ = *cpMacStr++;\
   \
   uiCharMac = dupMacStr - (dupStr);\
   (dupStr)[uiCharMac] = '\0';\
   uiCharMac; /*the number of characters copied*/\
}) /*cpDelim_ulCp*/

/*-------------------------------------------------------\
| Fun05: lenStr_ulCp
|   - Finds the length of a string using unsigned longs
| Input:
|   - inStr:
|     o C-string or string with deliminator to find length
|       of
|   - delimUL:
|     o Deliminator marking the end of the string
| Output:
|   - Returns:
|     o Number of characters in the string
\-------------------------------------------------------*/
#define \
lenStr_ulCp( \
   inStr, \
   delimUL, \
   delimC \
)({\
   unsigned long *macPtrUL = (unsigned long *) (inStr);\
   unsigned long macAtDelimUL = 0;\
   unsigned int uiLenStrMac = 0;\
   \
   macAtDelimUL = ifDelim_ulCp(*macPtrUL, (delimUL));\
   \
   while(!macAtDelimUL)\
   { /*Loop: Copy cpStr to dupStr*/\
      uiLenStrMac += def_charInUL_ulCp;\
      ++macPtrUL;\
      macAtDelimUL= ifDelim_ulCp(*macPtrUL,(delimUL));\
   } /*Loop: Copy cpStr to dupStr*/\
   \
   while((inStr)[uiLenStrMac] !=(delimC)) ++uiLenStrMac;\
   \
   uiLenStrMac;\
}) /*cpDelim_ulCp*/

/*-------------------------------------------------------\
| Fun06: endLine_ulCp
|   - Finds the end of a c-string. This assumes that the
|     line ends in an '\0' or an '\n'
| Input:
|   - inStr:
|     o C-string or string to look for end in
| Output:
|   - Returns:
|     o Number of characters in the string
\-------------------------------------------------------*/
#define \
endLine_ulCp(\
   inStr\
)({\
   unsigned long *macPtrUL = (unsigned long *) (inStr);\
   unsigned int uiLenStrMac = 0;\
   \
   unsigned long newLineMacUL = mkDelim_ulCp('\n');\
   unsigned long oneMacUL = mkDelim_ulCp(0x01);\
   unsigned long highBitMacUL = mkDelim_ulCp(0x80);\
   \
   while( !\
     (\
          (((*macPtrUL++) & (~ newLineMacUL)) - oneMacUL)\
        & highBitMacUL\
     )\
   ) uiLenStrMac += def_charInUL_ulCp;\
   \
   while((inStr)[uiLenStrMac] & (~ '\n')) ++uiLenStrMac;\
   \
   uiLenStrMac;\
}) /*endLine_ulCp*/

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
