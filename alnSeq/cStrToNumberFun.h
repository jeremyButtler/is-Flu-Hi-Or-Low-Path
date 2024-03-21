/*#########################################################
# Name: cStrToNumberFun
# Use:
#  - These are my numeric functions to convert numbers to
#    specific data types. 
#  - This allows me to avoid issues of overflows form the
#    base c functions, wich always convert to unsigned
#    longs or longs.
# Libraries:
#   - "dataTypeShortHand.h"
# C Standard Libraries:
#   - <stdint.h>
# Note:
#  - This uses set sizes for each data type & so is a bit
#    less universall than the base functions.
#  - Currently this is set up only for base 10.
#########################################################*/

#ifndef CSTRTONUMBERFUN_H
#define CSTRTONUMBERFUN_H

#include "dataTypeShortHand.h" /*unsigned var to uvar*/
#include <stdint.h> /*uintx_t variables*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cStrToNumberFun SOF: Start Of Header
'  - Functions to converts c-strings to numbers
'  o fun-01 cStrToUInt:
'    - converst a base 10 c-string to an uint32_t
'  o fun-02 cStrToUSht:
'    - converst a base 10 c-string to an uint16_t
'  o fun-03 cStrToUChar:
'    - converst a base 10 c-string to an uint8_t
'  o fun-04 backwarsCStrToUInt:
'    - converst a backwards (lowest value first) base 10
'      c-string to an uint32_t
'  o fun-05 uCharToCStr:
'    - Converts an uint8_t to an c-string
'  o fun-06 cStrToInt16:
'    - converst a base 10 c-string to an int16_t
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Name: strToULBase10 (Fun-07:)
| Use:
|  - Converts a base 10 c-string to an unsigned long
| Input:
|  - numericStr:
|    o C-string with base 10 number to convert
|  - retUL:
|    o unsigned long to hold the returned number
| Output:
|  - Returns:
|    o pionter to character after last converted number
|  - Modifies:
|    o retUL to hold the converted number
| Note:
|  - This function will only convert the first number of
|    digits in an unsigned long or till a non-numeric
|    character
\--------------------------------------------------------*/
static char * strToULBase10(
    char *numericStr,    /*C-string with base 10 number*/
    ulong *retUL         /*Holds converted number*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: cStrToUInt
   '  - converst a base 10 c-string to an uint32_t
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    ulong tmpUL = 0;
    *retUL = 0;
    uchar ucDig = 0;

    for(
       ucDig = 0;
       ucDig < ((sizeof(ulong) << 3) / 3.333333333) - 1;
      ++ucDig
    ) { /*Convert digits with no overflow concern*/
       /* Logic behind: ((sizeof(ulong)<<3)/3.333333333)+1
       `  o sizeof(ulong) << 3:
       `    - This gives the number of bits (powers of two)
       `  o / 3.3333333...:
       `    - This converts powers of 2 to powers of 10
       `  o -1:
       `    - This undercounts by two digits.
       `    - The ones digit is missed without -1.
       `    - The second digit is lost with - 1.
       */

        if(*numericStr - 58 < 0 && *numericStr - 47 > 0)
            *retUL = *retUL * 10 + *numericStr - 48;
        else return numericStr;

        ++numericStr;
    } /*Convert digits with no overflow concern*/

    /*Convert last two digits, which could overflow*/
    /*2nd to last digit*/
    if(*numericStr - 58 < 0 && *numericStr - 47 > 0)
    { /*If have one or two more numbers*/
        tmpUL = (10 * *retUL + *numericStr - 48);

        if(tmpUL < *retUL) return numericStr;

        else
        { /*Else: I can fit in one more number*/
            *retUL = tmpUL;
            ++numericStr;
        } /*Else: I can fit in one more number*/
    } /*If have one or two more numbers*/

    else return numericStr;

    /*10th digit*/
    if(*numericStr - 58 < 0 && *numericStr - 47 > 0)
    { /*If have one more number*/
        tmpUL = (10 * *retUL + *numericStr - 48);

        if(tmpUL < *retUL) return numericStr;

        else
        { /*Else: I can fit in one more number*/
            *retUL = tmpUL;
            ++numericStr;
        } /*Else: I can fit in one more number*/
    } /*If have one more number*/

    return numericStr;
} /*cStrToUInt*/

/*This is just an branchless attempt. I need to benchmark
` to see if it is faster. I suspect it will be slower. It
` has to convert the entire number, not a portion
*/
#define bitStrToULBase10(base10Str, retStr, retUL)\
   char keepBl = 0;\
   ulong tmpUL = 0;\
   (retStr) = (base10Str);\
   uchar ucDig = 0;\
   \
   for(\
       ucDig = 0;\
       ucDig < ((sizeof(ulong) << 3) / 3.333333333) - 1;\
      ++ucDig\
   ){ /*Convert safe digits in string*/\
      /* Logic behind: ((sizeof(ulong)<<3)/3.333333333)+1
      `  o sizeof(ulong) << 3:
      `    - This gives the number of bits (powers of two)
      `  o / 3.3333333...:
      `    - This converts powers of 2 to powers of 10
      `  o -1:
      `    - This undercounts by two digits.
      `    - The ones digit is missed without -1.
      `    - The second digit is lost with - 1.
      */\
      keepBl &= (*(retStr) > 47) & (*(retStr) < 58);\
      (retUL)=((-keepBl) & (10 * (retUL) + *(retStr)-47));\
      (retStr) += keepBl;\
   } /*Convert safe digits in string*/\
   \
   /*At this point I need to worry about overflows*/\
   /*See if I have a 2nd to last digit to convert*/\
   keepBl &= (*(retStr) > 47) & (*(retStr) < 58);\
   tmpUL = ((-keepBl) & (10 * (retUL) + *(retStr) - 47));\
   keepBl &= ((retUL) < tmpUL);\
   (retUL) = ((retUL) & (keepBl-1)) | (tmpUL & (-keepBl));\
   (retStr) += keepBl;\
   \
   /*See if I have a last digit to convert*/\
   keepBl &= (*(retStr) > 47) & (*(retStr) < 58);\
   tmpUL = ((-keepBl) & (10 * (retUL) + *(retStr) - 47));\
   keepBl &= ((retUL) < tmpUL);\
   (retUL) = ((retUL) & (keepBl-1)) | (tmpUL & (-keepBl));\
   (retStr) += keepBl;\
}
  
#endif
