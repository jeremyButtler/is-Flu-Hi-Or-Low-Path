/*########################################################
# Name: base10ToStrNum
# Use:
#  - These are my numeric functions to convert numbers to
#    specific data types.
# Libraries:
#   - "dataTypeShortHand.h"
# C Standard Libraries:
# Note:
#  - Currently this is set up only for base 10
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cStrToNumberFun SOF: Start Of Header
'  - Functions to converts c-strings to numbers
'  o header:
'    - Has includes and the defined size of each number
'  o fun01 strToUL_base10str:
'    - Converts a base 10 c-string to an unsigned long
'  o fun02 strToUI_base10str:
'    - Converts a base 10 c-string to an unsigned integer
'  o fun03 strToUS_base10str:
'    - Converts a base 10 c-string to an unsigned short
'  o fun04 strToUC_base10str:
'    - Converts a base 10 c-string to an unsigned char
'  o fun05 strToU64_base10str:
'    - Converts a base 10 c-string to an uint64_t
'  o fun06 strToU32_base10str:
'    - Converts a base 10 c-string to an uint32_t
'  o fun07 strToU16_base10str:
'    - Converts a base 10 c-string to an uint16_t
'  o fun08 strToU8_base10str:
'    - Converts a base 10 c-string to an uint8_t
'  o fun09 strToUL_base10str:
'    - Converts a base 10 c-string to an unsigned long
'  o fun10 strToSI_base10str:
'    - Converts a base 10 c-string to an signed integer
'  o fun11 strToSS_base10str:
'    - Converts a base 10 c-string to an signed short
'  o fun12 strToSC_base10str:
'    - Converts a base 10 c-string to an signed char
'  o fun13 strToS64_base10str:
'    - Converts a base 10 c-string to an int64_t
'  o fun14 strToS32_base10str:
'    - Converts a base 10 c-string to an int32_t
'  o fun15 strToS16_base10str:
'    - Converts a base 10 c-string to an int16_t
'  o fun16 strToS8_base10str:
'    - Converts a base 10 c-string to an int8_t
'  o fun017 revStrToUI_base10str:
'    - like strToUI_base10str, except it moves back on the
'      c-string
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - Has includes and the defined size of each number
|  o header sec-01:
|    - Includes
|  o header sec-02:
|    - Maximum digits and values for unsigned data types
|  o header sec-03:
|    - Maximum digits and values for signed data types
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-01:
^  - includes 
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifndef STRTONUM_H
#define STRTONUM_H

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-02:
^  - Maximum digits and values for unsigned data types
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*Some tricks I use that will be optimized out at
` compiler time:
*/
#define defMaxUL()(\
     ((ulong) 1 << ((sizeof(ulong) << 3) - 1))\
   | (((ulong) 1 << ((sizeof(ulong) << 3) - 1)) - 1)\
)

#define defNumBase10DigInUL()(\
      (char) (((sizeof(ulong) << 3)) / 3.333333333)\
)

#define defMaxUI()(\
     ((uint) 1 << ((sizeof(uint) << 3) - 1))\
   | (((uint) 1 << ((sizeof(uint) << 3) - 1)) - 1)\
)

#define defNumBase10DigInUI()(\
      (char) (((sizeof(uint) << 3)) / 3.333333333)\
)

#define defMaxUS()(\
     (1 << ((sizeof(ushort) << 3) - 1))\
   | ((1 << ((sizeof(ushort) << 3) - 1)) - 1)\
)

#define defNumBase10DigInUS()(\
      (char) (((sizeof(ushort) << 3)) / 3.333333333)\
)

#define defMaxUC()(\
     (1 << ((sizeof(uchar) << 3) - 1))\
   | ((1 << ((sizeof(uchar) << 3) - 1)) - 1)\
)

#define defNumBase10DigInUC()(\
      (char) (((sizeof(uchar) << 3)) / 3.333333333)\
)

/* (1 << ((sizeof(datatype) << 3) - 1)) |
`  ((1 << ((sizeof(datatype) << 3) - 1))) - 1)
`    - This gets me the maximum value of a datatype
`      without using a C library and avoides overflow
`      issues. This will be optimized out.
`    o sizeof(datatype) << 3
`      - gives me the number of bits
`    o 1 << ((sizeof(datatype) << 3) - 1)
`      - Gives me the highest power of two (2 ^ (bits - 1))
`    o highest power of 2 - 1
`      - Sets every bit to 1 except the highest bit
`        (highest power of two) (2 ^ (bits - 2))
`    o highest power of 2 | (highest power of 2 - 1)
`      - Sets all bits, thus is the maximum number
`
` (char) (((sizeof(datatype) << 3)) / 3.333333333)
`    - This gives me the number of digits that will not
`      overflow (number digits - 1) in a base 10 number.
`    o (sizeof(datatype) << 3
`      - Get number of bits in a datatype
`    o number bits / 3.333333333
`      - Converts powers of 2 to powers of 10
`    o (cast) powers 10
`      - Make sure is a whole number
*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-03:
^  - Maximum digits and values for signed data types
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define defMaxSL()(\
     ((long) 1 << ((sizeof(long) << 3) - 2))\
   | (((long) 1 << ((sizeof(long) << 3) - 2)) - 1)\
)

#define defNumBase10DigInSL()(\
      (char) (((sizeof(long) << 3) - 1) / 3.333333333)\
)

#define defMaxSI()(\
     ((int) 1 << ((sizeof(int) << 3) - 2))\
   | (((int) 1 << ((sizeof(int) << 3) - 2)) - 1)\
)

#define defNumBase10DigInSI()(\
      (char) (((sizeof(int) << 3) - 1) / 3.333333333)\
)

#define defMaxSS()(\
     (1 << ((sizeof(short) << 3) - 2))\
   | ((1 << ((sizeof(short) << 3) - 2)) - 1)\
)

#define defNumBase10DigInSS()(\
      (char) (((sizeof(short) << 3) - 1) / 3.333333333)\
)

#define defMaxSC()(\
     (1 << ((sizeof(char) << 3) - 2))\
   | ((1 << ((sizeof(char) << 3) - 2)) - 1)\
)

#define defNumBase10DigInSC()(\
      (char) (((sizeof(char) << 3) - 1) / 3.333333333)\
)

/* (1 << ((sizeof(datatype) << 3) - 2)) |
`  ((1 << ((sizeof(datatype) << 3) - 2))) - 1)
`    - This gets me the maximum value of a datatype
`      without using a C library and does not have
`      overflow issues. This will be optimized out.
`    o sizeof(datatype) << 3
`      - gives me the number of bits
`    o 1 << ((sizeof(datatype) << 3) - 2)
`      - Gives me the highest power of two (2 ^ (bits-1))
`    o 2^(bits - 1) - 1
`      - Sets every bit to 1 except the highest bit
`        (highest power of two)
`    o 2^(bits - 1) | (2^(bits - 1) - 1)
`      - Sets all bits, thus is the maximum number
`
` (char) (((sizeof(datatype) << 3) - 1) / 3.333333333)
`    - This gives me the number of digits that will not
`      overflow (number digits - 1) in a base 10 number.
`    o (sizeof(datatype) << 3
`      - Get number of bits in a datatype
`    o number bits - 1
`      - Gives me the number of bits in an signed datatype
`    o (number bits - 1) / 3.333333333
`      - Converts powers of 2 to powers of 10
`    o (cast) powers 10
`      - Make sure is a whole number
*/

/*-------------------------------------------------------\
| Fun01: strToUL_base10str
|  - Converts a base 10 c-string to an unsigned long.
|    Your system determines how many bits are in an long.
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUL:
|    o an unisnged long to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUL to hold the unsigned long
| Note:
|  - This function will only convert the first number of
|    digits in an unsigned long or till non-numeric
|    character
\-------------------------------------------------------*/
#define \
strToUL_base10str( \
   inStr, \
   retUL \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   unsigned long macroTmpUL = 0;\
   (retUL) = 0;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInUL();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retUL) = (retUL) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retUL) <= defMaxUL() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpUL = 10 * (retUL) + *macroRetStr - 48;\
         \
         if(macroTmpUL > 9)\
         { /*If: I have room for 10th digit*/\
            (retUL) = macroTmpUL;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToUL_base10str*/

/*-------------------------------------------------------\
| Fun02: strToUI_base10str
|  - Converts a base 10 c-string to an unsigned integer.
|    Your system determines how many bits are in an int.
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUI:
|    o an unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI to hold the unsigned integer
| Note:
|  - This function will only convert the first number of
|    digits in an unsigned integer or till non-numeric
|    character
\-------------------------------------------------------*/
#define \
strToUI_base10str( \
   inStr, \
   retUI \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   unsigned int macroTmpUI = 0;\
   (retUI) = 0;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInUI();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retUI) = (retUI) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retUI) <= defMaxUI() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpUI = 10 * (retUI) + *macroRetStr - 48;\
         \
         if(macroTmpUI > 9)\
         { /*If: I have room for 10th digit*/\
            (retUI) = macroTmpUI;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToUI_base10str*/

/*-------------------------------------------------------\
| Fun03: strToUS_base10str
|  - Converts a base 10 c-string to an unsigned short.
|    Your system determines how many bits are in an short.
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUS:
|    o an unsigned short to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUS to hold the unsigned short
| Note:
|  - This function will only convert the first number of
|    digits in an unsigned short or till non-numeric
|    character
\-------------------------------------------------------*/
#define \
strToUS_base10str( \
   inStr, \
   retUS \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   unsigned short macroTmpUS = 0;\
   (retUS) = 0;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInUS();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retUS) = (retUS) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retUS) <= defMaxUS() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpUS = 10 * (retUS) + *macroRetStr - 48;\
         \
         if(macroTmpUS > 9)\
         { /*If: I have room for 10th digit*/\
            (retUS) = macroTmpUS;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToUS_base10str*/

/*-------------------------------------------------------\
| Fun04: strToUC_base10str
|  - Converts a base 10 c-string to an unsigned char.
|    Your system determines how many bits are in an char.
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUC:
|    o an unsigned char to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUC to hold the unsigned char
| Note:
|  - This function will only convert the first number of
|    digits in an unsigned char or till non-numeric
|    character
\-------------------------------------------------------*/
#define \
strToUC_base10str( \
   inStr, \
   retUC \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   unsigned char macroTmpUC = 0;\
   (retUC) = 0;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInUC();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retUC) = (retUC) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retUC) <= defMaxUC() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpUC = 10 * (retUC) + *macroRetStr - 48;\
         \
         if(macroTmpUC > 9)\
         { /*If: I have room for 10th digit*/\
            (retUC) = macroTmpUC;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToUC_base10str*/

/*-------------------------------------------------------\
| Fun05: strToU64_base10str
|  - Converts a base 10 c-string to an 64 bit unsigned int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retU64:
|    o 64 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI64 to hold the uint64_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an uint64_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToU64_base10str( \
   inStr, \
   retU64 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retU64) = 0;\
   \
   for(macroCDig = 0; macroCDig < 19; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retU64) = (retU64) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retU64) <= (ulonglong) 184467440737095516)\
      { /*If: This number is < the max 64 bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if((ulong) 10 * (retU64) + *macroRetStr - 48 >9)\
         { /*If: I have room for 10th digit*/\
            (retU64) = (retU64) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 64bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToU64_base10str*/

/*-------------------------------------------------------\
| Fun06: strToU32_base10str
|  - Converts a base 10 c-string to an 32 bit unsigned int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUI32:
|    o 32 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retU32 to hold the uint32_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an uint32_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToU32_base10str( \
   inStr, \
   retU32 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retU32) = 0;\
   \
   for(macroCDig = 0; macroCDig < 9; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retU32) = (retU32) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retU32) <= (ulong) 429496729)\
      { /*If: This number is < the max 32bit size/10*/\
         /*I am not using a temporary variable here because
         ` I do not want this library to call stdint. I
         ` want the user to do that.
         */\
         if((uint) (10 * (retU32) + *macroRetStr -48 >9))\
         { /*If: I have room for 10th digit*/\
            (retU32) = (retU32) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 32bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToU32_base10str*/

/*-------------------------------------------------------\
| Fun07: strToU16_base10str
|  - Converts a base 10 c-string to an 16 bit unsigned int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUI16:
|    o 16 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retU16 to hold the uint16_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an uint16_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToU16_base10str( \
   inStr, \
   retU16 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retU16) = 0;\
   \
   for(macroCDig = 0; macroCDig < 3; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retU16) = (retU16) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retU16) <= (ulong) 6553)\
      { /*If: This number is < the max 16bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if((uint) (10 * (retU16) + *macroRetStr -48 >9))\
         { /*If: I have room for 10th digit*/\
            (retU16) = (retU16) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 32bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToU16_base10str*/

/*-------------------------------------------------------\
| Fun08: strToU8_base10str
|  - Converts a base 10 c-string to an 8 bit unsigned int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUI8:
|    o 8 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retU8 to hold the uint8_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an uint8_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToU8_base10str( \
   inStr, \
   retU8 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retU8) = 0;\
   \
   for(macroCDig = 0; macroCDig < 3; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retU8) = (retU8) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retU8) <= 25)\
      { /*If: This number is < the max 8bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if((uint) (10 * (retU8) + *macroRetStr -48 >9))\
         { /*If: I have room for 10th digit*/\
            (retU8) = (retU8) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 8bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToU8_base10str*/

/*-------------------------------------------------------\
| Fun09: strToSL_base10str
|  - Converts a base 10 c-string to an signed long.
|    Your system determines how many bits are in an long.
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUL:
|    o an signed long to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retSL to hold the signed long
| Note:
|  - This function will only convert the first number of
|    digits in an long or till non-numeric character
\-------------------------------------------------------*/
#define \
strToSL_base10str( \
   inStr, \
   retSL \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   long macroTmpL = 0;\
   (retSL) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInSL();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retSL) = (retSL) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retSL) <= defMaxSL() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpL = 10 * (retSL) + *macroRetStr - 48;\
         \
         if(macroTmpL > 9)\
         { /*If: I have room for 10th digit*/\
            (retSL) = macroTmpL;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   if(*inStr == '-') retSL = -retSL;\
   \
   macroRetStr;\
}) /*strToSL_base10str*/

/*-------------------------------------------------------\
| Fun10: strToSI_base10str
|  - Converts a base 10 c-string to an signed integer
|    Your system determines number bits in an integer
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUL:
|    o an signed long to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retSL to hold the signed integer
| Note:
|  - This function will only convert the first number of
|    digits in an integer or till non-numeric character
\-------------------------------------------------------*/
#define \
strToSI_base10str(\
   inStr,\
   retSI\
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   int macroTmpI = 0;\
   \
   (retSI) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInSI();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retSI) = (retSI) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retSI) <= defMaxSI() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpI = 10 * (retSI) + *macroRetStr - 48;\
         \
         if(macroTmpI > 9)\
         { /*If: I have room for 10th digit*/\
            (retSI) = macroTmpI;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   if(*inStr == '-') retSI = -retSI;\
   \
   macroRetStr;\
}) /*strToSI_base10str*/

/*-------------------------------------------------------\
| Fun11: strToSS_base10str
|  - Converts a base 10 c-string to an signed short
|    Your system determines number bits in an short
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUL:
|    o an signed long to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retSL to hold the signed short
| Note:
|  - This function will only convert the first number of
|    digits in an short or till non-numeric character
\-------------------------------------------------------*/
#define \
strToSS_base10str( \
   inStr, \
   retSS \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   short macroTmpS = 0;\
   (retSS) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInSS();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retSS) = (retSS) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retSS) <= defMaxSS() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpS = 10 * (retSS) + *macroRetStr - 48;\
         \
         if(macroTmpS > 9)\
         { /*If: I have room for 10th digit*/\
            (retSS) = macroTmpS;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   if(*inStr == '-') retSS = -retSS;\
   \
   macroRetStr;\
}) /*strToSL_base10str*/

/*-------------------------------------------------------\
| Fun12: strToSC_base10str
|  - Converts a base 10 c-string to an signed char
|    Your system determines number bits in an char
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUL:
|    o an signed long to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retSL to hold the signed char
| Note:
|  - This function will only convert the first number of
|    digits in an char or till non-numeric character
\-------------------------------------------------------*/
#define \
strToSC_base10str( \
   inStr, \
   retSC \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   char macroTmpC = 0;\
   (retSC) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInSC();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retSC) = (retSC) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retSC) <= defMaxSC() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpC = 10 * (retSC) + *macroRetStr - 48;\
         \
         if(macroTmpC > 9)\
         { /*If: I have room for 10th digit*/\
            (retSC) = macroTmpC;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   if(*inStr == '-') retSC = -retSC;\
   \
   macroRetStr;\
}) /*strToSL_base10str*/

/*-------------------------------------------------------\
| Fun13: strToS64_base10str
|  - Converts a base 10 c-string to an 64 bit signed int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retS64:
|    o 64 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI64 to hold the int64_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an int64_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToS64_base10str( \
   inStr, \
   retS64 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retS64) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(macroCDig = 0; macroCDig < 18; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retS64) = (retS64) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retS64) <= (longlong) 922337203685477580)\
      { /*If: This number is < the max 64 bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if(10 * (retS64) + *macroRetStr - 48 > 9)\
         { /*If: I have room for 10th digit*/\
            (retS64) = (retS64) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 64bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   if(*inStr == '-') retS64 = -retS64;\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) base10/*StrToS64*/

/*-------------------------------------------------------\
| Fun14: strToS32_base10str
|  - Converts a base 10 c-string to an 32 bit signed int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retS32:
|    o 32 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI32 to hold the int32_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an int32_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToS32_base10str( \
   inStr, \
   retS32 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retS32) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(macroCDig = 0; macroCDig < 9; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retS32) = (retS32) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retS32) <= (ulong) 214748364)\
      { /*If: This number is < the max 32 bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if(10 * (retS32) + *macroRetStr - 48 > 9)\
         { /*If: I have room for 10th digit*/\
            (retS32) = (retS32) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 32bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   if(*inStr == '-') retS32 = -retS32;\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) base10/*StrToS32*/

/*-------------------------------------------------------\
| Fun15: strToS16_base10str
|  - Converts a base 10 c-string to an 16 bit signed int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retS16:
|    o 16 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI16 to hold the int16_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an int16_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToS16_base10str( \
   inStr, \
   retS16 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retS16) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(macroCDig = 0; macroCDig < 4; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retS16) = (retS16) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retS16) <= (ulong) 3276)\
      { /*If: This number is < the max 16 bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if(10 * (retS16) + *macroRetStr - 48 > 9)\
         { /*If: I have room for 10th digit*/\
            (retS16) = (retS16) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 16bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   if(*inStr == '-') retS16 = -retS16;\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) base10/*StrToS16*/

/*-------------------------------------------------------\
| Fun16: strToS8_base10str
|  - Converts a base 10 c-string to an 8 bit signed int
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retS8:
|    o 8 bit unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI8 to hold the int8_t integer
| Note:
|  - This function will only convert the first number of
|    digits in an int8_t unsigned integer or till
|    non-numeric character
\-------------------------------------------------------*/
#define \
strToS8_base10str( \
   inStr, \
   retS8 \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   (retS8) = 0;\
   \
   if(*inStr == '-') ++macroRetStr;\
   \
   for(macroCDig = 0; macroCDig < 2; ++macroCDig)\
   { /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retS8) = (retS8) * 10 + *macroRetStr - 48;\
      else break;\
      \
      ++macroRetStr;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a 10th digit (could overflow)*/\
      if((retS8) <= 12)\
      { /*If: This number is < the max 8 bit size/10*/\
         /*I am not using a temporary variable here
         ` because I do not want this library to call
         ` stdint. I want the user to do that.
         */\
         if(10 * (retS8) + *macroRetStr - 48 > 9)\
         { /*If: I have room for 10th digit*/\
            (retS8) = (retS8) * 10 + *macroRetStr - 48;\
            ++macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: This number is < the max 8bit size/10*/\
   } /*If have a 10th digit (could overflow)*/\
   \
   if(*inStr == '-') retS8 = -retS8;\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*strToS8_base10str*/

/*-------------------------------------------------------\
| Fun17: revStrToUI_base10str
|  - Converts a base 10 c-string to an unsigned integer.
|  - In this case we start at the end of the c-string
| Input:
|  - inStr:
|    o C-string with base 10 number to convert
|  - retUI:
|    o an unisnged integer to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUI to hold the unsigned integer
| Note:
|  - This function will only convert the first number of
|    digits in an unsigned integer or till non-numeric
|    character
\-------------------------------------------------------*/
#define \
revStrToUI_base10str( \
   inStr, \
   retUI \
)({\
   char macroCDig = 0;\
   char *macroRetStr = (inStr);\
   unsigned int macroTmpUI = 0;\
   unsigned int powTenUI = 1;\
   (retUI) = 0;\
   \
   for(\
      macroCDig = 0;\
      macroCDig < defNumBase10DigInUI();\
      ++macroCDig\
   ){ /*Loop: Convert digits with no overflow concern*/\
      if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
         (retUI) = (retUI) + (*macroRetStr -48) * powTenUI;\
      else break;\
      \
      --macroRetStr;\
      powTenUI = powTenUI * 10;\
   } /*Loop: Convert digits with no overflow concern*/\
   \
   if(*macroRetStr - 58 < 0 && *macroRetStr - 47 > 0)\
   { /*If have a last digit (could overflow)*/\
      if((retUI) <= defMaxUI() / 10)\
      { /*If: number is < the max unsigned int size/10*/\
         macroTmpUI =\
            (retUI) + (*macroRetStr - 48) * powTenUI;\
         \
         if(macroTmpUI > 9)\
         { /*If: I have room for 10th digit*/\
            (retUI) = macroTmpUI;\
            --macroRetStr;\
         } /*If: I have room for 10th digit*/\
      } /*If: number is < the max unsigned int size/10*/\
   } /*If have a last digit (could overflow)*/\
   \
   macroRetStr; /*Let the user see were ended at*/\
}) /*revStrToUI_base10str*/

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
