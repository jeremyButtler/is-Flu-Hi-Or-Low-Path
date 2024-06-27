/*########################################################
# Name: charCp
#   - Copies contents of string 1 to string using chars
#   - These functions are slower than strcpy and strlen,
#     but they allow deliminators to be used.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards
'   o fun01: cp_charCp
'    - Copies cpStr into dupStr using characters
'   o fun02: delimCp_charCp
'     - Copies cpStr into dupStr until delimC is found
'   o fun03: lenDelim_charCp
'     - Finds the length of a string using characters
'   o fun04: eql_charCp
'     - Checks to see if two strings are equal
'   o fun05: match_charCp
'     - Checks to see if two strings are equal, but does
'       not check to see if there is anything past the
'       query's deliminator. This is to deal with strings
'       with differnt deliminators.
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef CHARACTER_COPY_H
#define CHARACTER_COPY_H

/*-------------------------------------------------------\
| Fun01: cp_charCp
|   - Copies cpStr into dupStr using characters
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
cp_charCp( \
   dupStr, \
   cpStr, \
   lenUI \
){\
   unsigned int uiIterMac = 0;\
   for(uiIterMac = 0; uiIterMac < lenUI; ++uiIterMac)\
      (dupStr)[uiIterMac] = (cpStr)[uiIterMac];\
   (dupStr)[uiIterMac] = '\0';\
}

/*-------------------------------------------------------\
| Fun02: delimCp_charCp
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
\-------------------------------------------------------*/
#define \
delimCp_charCp(\
   dupStr,\
   cpStr,\
   delimC\
)({\
   char *dupMacStr = (dupStr);\
   char *cpMacStr = (cpStr);\
   \
   while(*(cpMacStr) != (delimC))\
      *dupMacStr++ = *cpMacStr++;\
   \
   *dupMacStr = '\0';\
   dupMacStr - (dupStr);\
})
/*The *dupStr++ = *cpStr++; is faster than incurmenting
` individually or by number
*/

/*-------------------------------------------------------\
| Fun03: lenDelim_charCp
|   - Finds the length of a string using characters
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
lenDelim_charCp( \
   inStr, \
   delimC \
)({\
   char *inMacStr = (inStr);\
   while(*inMacStr != (delimC)) ++inMacStr;\
   inMacStr - (inStr);\
})

/*-------------------------------------------------------\
| Fun04: eql_charCp
|   - Checks to see if two strings are equal
| Input:
|   - qryStr:
|     o The query c-string to compare against the
|       reference
|   - refStr:
|     o The reference c-string to compare against the
|       query
| Output:
|   - Returns:
|     o < 0 for qry less than reference
|     o 0 for qry is same as the reference
|     o >0 for qry is greater than the reference
\-------------------------------------------------------*/
#define \
eql_charCp( \
   qryStr, \
   refStr, \
   delimC \
)({\
   int iCntMac = 0;\
   \
   /*This is faster than incurmenting qryMacStr and
   ` refMacStr separately. Doing *val++ == *val2++ does
   ` not work here.
   */\
   while((qryStr)[iCntMac] == (refStr)[iCntMac])\
   { /*Looop*/\
      if((qryStr)[iCntMac] == (delimC)) break;\
      ++iCntMac;\
   } /*Looop*/\
   \
   (qryStr)[iCntMac] - (refStr)[iCntMac];\
   \
   /*this was removed because it caused different behavior
   ` them strcmp
   */\
   /*(-(*(qryStr) !=(delimC))) &(*(qryStr) -*(refStr));*/\
   /*Logic (for old code):
   `  - qryStr != delimC:
   `    o 0 if I have the delimintaor (match)
   `    o 1 if I do not have the delimintaor (no match)
   `  - -(qryStr != delimC):
   `    o Converts 1 to -1 (all bits set) and 0 to 0, no
   `      bits set
   `  - qryStr - refStr
   `    o 0 if the stings were equal
   `    o < 0 if qryStr is less than refStr
   `    o > 0 if qryStr is greater than refStr
   `  - (-(qryStr != delimC)) & (qryStr - refStr)
   `    o is (-1 & number) = number, if qryStr was not on
   `      the deliminator
   `    o is (0 & number) = 0, if qryStr was on the
   `      deliminator
   */\
})

/*-------------------------------------------------------\
| Fun05: match_charCp
|   - Checks to see if two strings are equal, but does
|     not check to see if there is anything past the
|     query's deliminator. This is to deal with strings
|     with differnt deliminators.
| Input:
|   - qryStr:
|     o The query c-string to compare against the
|       reference
|   - refStr:
|     o The reference c-string to compare against the
|       query
|   - delimC:
|     o Deliminator to stop at
|   - endI:
|     o Will have the position before the deliminator.
|       This assumes that you know what you are doing,
|       so if you input endI > 0; this will assume you
|       do not want to compare at 0. This is handy for
|       looping through headers in tsv files.
| Output:
|   - Returns:
|     o < 0 for qry less than reference
|     o 0 for qry is same as the reference
|     o >0 for qry is greater than the reference
\-------------------------------------------------------*/
#define \
match_charCp( \
   qryStr, \
   refStr, \
   delimC, \
   endI \
)({\
   char *refMacStr = (refStr);\
   char *qryMacStr = (qryStr);\
   /*This is faster than incurmenting qryMacStr and
   ` refMacStr separately. Doing *val++ == *val2++ does
   ` not work here.
   */\
   while(*qryMacStr++ == *refMacStr++)\
   { /*Looop*/\
      if(*qryMacStr == (delimC))\
      { /*If: I found the  deliminator*/\
         --qryMacStr;\
         --refMacStr;\
         break;\
      } /*If: I found the  deliminator*/\
   } /*Looop*/\
   \
   (endI) += qryMacStr - (qryStr);\
   *qryMacStr - *refMacStr;\
})

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
