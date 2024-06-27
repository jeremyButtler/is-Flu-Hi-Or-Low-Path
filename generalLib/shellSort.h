/*########################################################
# Name: gen-shellShort
#   - Holds functions for doing shell sorts on arrays.
#   - Currently is numeric arrays only
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - Header guards
'  o .h fun01: binSearchNumeric
'    - Does an binary search on an array of numeric items
'    - This is here because it is linked with sorting
'  o .h fun02: binSearchRange
'     - Searches for an value inside an range 
'  o .h fun03: swapVal
'     - Swaps to values (numeric/character not string)
'  o .h fun04: arySortNumeric
'    - Sorts the input array from least to greatest with
'      shell sort. This uses numeric input.
'  o .h fun05: sortTwoNumAry_shellsort
'    - Sorts the first input array from least to greatest
'      and keeps the second input arrar in order with the
'      first array. This is set up for numeric input using
'      shell short. Both arrays must be the same size.
'  o .h fun06: threeArySortNumeric
'    - Sorts three arrays by the first input array (as
'      numeric) with shell short. All three arrays must be
'      the same size.
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Header guards
\-------------------------------------------------------*/

#ifndef GENERAL_SHELL_SHORT_H
#define GENERAL_SHELL_SHORT_H

/*-------------------------------------------------------\
| Fun01: binSearchNumeric
|  - Does an binary search on an array of numeric items
| Input:
|  - numAryPtr:
|    o Pointer to an array of numeric values
|  - qrySI:
|    o number to look up
|  - lenAryUI:
|    o length of qrySIPtr (index 1)
| Output:
|  - Returns:
|    o The index of qrySI in arySIPtr
|    o -1 if qrySI is not in arySIPtr
\-------------------------------------------------------*/
#define \
binSearchNumeric(\
   numAryPtr,\
   qrySI,\
   lenAryUI\
)({\
   unsigned int midMacUI = 0;\
   unsigned int rightHalfMacUI = (lenAryUI) - 1;\
   unsigned int leftHalfMacUI = 0;\
   \
   while(leftHalfMacUI <= rightHalfMacUI)\
   { /*Loop: Search for the querys index*/\
      midMacUI = (leftHalfMacUI + rightHalfMacUI) >> 1;\
     \
     if((qrySI) > (numAryPtr)[midMacUI])\
         leftHalfMacUI = midMacUI + 1;\
      \
     else if((qrySI) < (numAryPtr)[midMacUI])\
         rightHalfMacUI = midMacUI - 1;\
     \
     else \
        break; /*Found the query*/ \
   } /*Loop: Search for the querys index*/\
   \
   /*See if the query was found*/\
   if(midMacSI < lenAryUI) \
   { /*If: the lineage may have been found*/ \
      midMacSI |= \
         (signed int) \
         -( \
               (qrySI) \
            != (spoligoSTAryPtr)[midMacSI].codeUL \
          ); \
      /*goes to -1 if the codes are not equal*/ \
   } /*If: the lineage may have been found*/ \
   \
   else \
      midMacSI = -1; /*query not found*/ \
   \
   midMacSI;\
}) /*binSearchNumeric*/

/*-------------------------------------------------------\
| Fun02: binSearchRange
|  - Searches for an value inside an range 
| Input:
|  - numAryPtr:
|    o Pointer to an array of numeric values
|  - startSI:
|    o Start of the range to look for
|  - endSI:
|    o End of the range to look for
|  - lenAryUI:
|    o length of qrySIPtr (index 1)
| Output:
|  - Returns:
|    o The first index in the range
|    o -1 if no value is in the range
\-------------------------------------------------------*/
#define \
binSearchRange(\
   numAryPtr,\
   startSI,\
   endSI,\
   lenAryUI\
)({\
   signed int midMacSI = 0;\
   signed int rightHalfMacSI = (lenAryUI) - 1;\
   signed int leftHalfMacSI = 0;\
   \
   while(leftHalfMacSI <= rightHalfMacSI)\
   { /*Loop: Search for the querys index*/\
      midMacSI = (leftHalfMacSI + rightHalfMacSI) >> 1;\
     \
     if((startSI) > (numAryPtr)[midMacSI])\
         leftHalfMacSI = midMacSI + 1;\
      \
     else if((endSI) < (numAryPtr)[midMacSI])\
         rightHalfMacSI = midMacSI - 1;\
     \
     else \
        break; /*Found the query*/\
   } /*Loop: Search for the querys index*/\
   \
   /*See if the query was found*/ \
   midMacSI |= ( -((long) (midMacSI >= (lenAryUI))) ); \
   \
   /*Find the nearest value still in the range*/\
   while( \
         midMacSI > 0 \
      && (numAryPtr)[midMacSI - 1] >= (startSI) \
   ) --midMacSI; \
   \
   midMacSI; \
}) /*binSearchRange*/

/*-------------------------------------------------------\
| Fun03: swapVal
|   - Swaps to values (numeric/character not string)
|   - This swap trick was taken from the standfords bit
|     hacking guide
| Input:
|   - firstVal:
|     o First value to swap
|   - secVal:
|     o second value to swap
| Output:
|   - Modifies:
|     o firstVal to be secVal
|     o secVal to be firstVal
\-------------------------------------------------------*/
#define \
swapVal( \
   firstVal, \
   secVal \
){ \
   (firstVal) ^= (secVal); \
   (secVal) ^= (firstVal); \
   (firstVal) ^= (secVal); \
} /*swapVal*/

/*-------------------------------------------------------\
| Fun04: arySortNumeric
|  - Sorts the input array from least to greatest with
|    shell sort. This uses numeric input.
| Input:
|  - firstAry:
|    o First array, this is the array to sort
|  - startUL:
|    o First element to start sorting at
|  - endUL:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o First array to be sorted form least to greatest
\-------------------------------------------------------*/
#define \
arySortNumeric( \
   firstAry, \
   startUL, \
   endUL \
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '  - Sorts the input array from least to greatest with
   '    shell sort. This uses numeric input.
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun04 sec01:
   '    - Variable declerations
   '  o fun04 sec02:
   '    - Find the number of rounds to sort for
   '  o fun04 sec03:
   '    - Sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun04 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Number of elements to sort*/\
  unsigned long numElmMacUL = (endUL) - (startUL);\
  long swapMacL = 0;\
  \
  /*Number of sorting rounds*/\
  unsigned long subMacUL = 0;\
  unsigned long nextElmMacUL = 0;\
  unsigned long lastElmMacUL = 0;\
  unsigned long elmOnMacUL = 0;\
  \
  /*Get arrays to sort from the matrix (for sanity)*/\
  \
  /*Variables to incurment loops*/\
  unsigned long ulIndexMac = 0;\
  unsigned long ulElmMac = 0;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun04 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/\
  subMacUL = 1; /*Initialzie first array*/\
  \
  while(subMacUL < numElmMacUL - 1)\
     subMacUL = (3 * subMacUL) + 1;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun04 Sec03:
  ^  - Sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  while(subMacUL > 0)\
  { /*loop trhough all sub arrays sort the subarrays*/\
    for( \
       ulIndexMac = 0;\
       ulIndexMac <= subMacUL;\
       ++ulIndexMac\
    ){ /*For each element in the subarray*/\
      for(ulElmMac = ulIndexMac;\
          ulElmMac + subMacUL <= endUL;\
          ulElmMac += subMacUL\
      ){ /*Loop; swap each nth element of the subarray*/\
        nextElmMacUL = ulElmMac + subMacUL;\
        \
        if((firstAry)[ulElmMac]>(firstAry)[nextElmMacUL])\
        { /*If I need to swap an element*/\
          swapVal( \
             (firstAry)[ulElmMac], \
             (firstAry)[nextElmMacUL] \
          ); \
          \
          lastElmMacUL = ulElmMac;\
          elmOnMacUL = ulElmMac;\
          \
          while(lastElmMacUL >= subMacUL)\
          { /*loop; move swapped element back*/\
            lastElmMacUL -= subMacUL;\
            \
            if(  (firstAry)[elmOnMacUL]\
               > (firstAry)[lastElmMacUL]\
            ) break; /*Positioned the element*/\
            \
            \
            swapVal( \
               (firstAry)[elmOnMacUL], \
               (firstAry)[lastElmMacUL] \
            ); \
            \
            elmOnMacUL = lastElmMacUL;\
          } /*loop; move swapped element back*/\
        } /*If I need to swap elements*/\
      } /*Loop; swap each nth element of the subarray*/\
    } /*For each element in the subarray*/\
    \
    subMacUL = (subMacUL - 1) / 3; /*Move to next round*/\
  } /*loop through all sub arrays to sort the subarrays*/\
} /*arySortNumeric*/

/*-------------------------------------------------------\
| Fun05: sortTwoNumAry_shellsort
|  - Sorts the first input array from least to greatest
|    and keeps the second input arrar in order with the
|    first array. This is set up for numeric input using
|    shell short. Both arrays must be the same size.
| Input:
|  - firstAry:
|    o First array, this is the array to sort
|  - secAry:
|    o Second array to keep sorted with first array
|  - startUL:
|    o First element to start sorting at
|  - endUL:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o First array to be sorted form least to greatest
|    o secAry to be kept in order with firstAry
\-------------------------------------------------------*/
#define \
sortTwoNumAry_shellsort( \
   firstAry, \
   secAry, \
   startUL, \
   endUL \
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '  - Sorts the first input array from least to greatest
   '    and keeps the second input arrar in order with the
   '    first array.
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun05 sec01:
   '    - Variable declerations
   '  o fun05 sec02:
   '    - Find the number of rounds to sort for
   '  o fun05 sec03:
   '    - Sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun05 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Number of elements to sort*/\
  unsigned long numElmMacUL = (endUL) - (startUL);\
  \
  /*Number of sorting rounds*/\
  unsigned long subMacUL = 0;\
  unsigned long nextElmMacUL = 0;\
  unsigned long lastElmMacUL = 0;\
  unsigned long elmOnMacUL = 0;\
  \
  /*Get arrays to sort from the matrix (for sanity)*/\
  \
  /*Variables to incurment loops*/\
  unsigned long ulIndexMac = 0;\
  unsigned long ulElmMac = 0;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun05 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/\
  subMacUL = 1; /*Initialzie first array*/\
  \
  while(subMacUL < numElmMacUL - 1)\
     subMacUL = (3 * subMacUL) + 1;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun05 Sec03:
  ^  - Sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  while(subMacUL > 0)\
  { /*loop trhough all sub arrays sort the subarrays*/\
    for( \
       ulIndexMac = 0;\
       ulIndexMac <= subMacUL;\
       ++ulIndexMac\
    ){ /*For each element in the subarray*/\
      for(ulElmMac = ulIndexMac;\
          ulElmMac + subMacUL <= endUL;\
          ulElmMac += subMacUL\
      ){ /*Loop; swap each nth element of the subarray*/\
        nextElmMacUL = ulElmMac + subMacUL;\
        \
        if((firstAry)[ulElmMac]>(firstAry)[nextElmMacUL])\
        { /*If I need to swap an element*/\
          swapVal( \
             (firstAry)[ulElmMac], \
             (firstAry)[nextElmMacUL] \
          ); \
          \
          swapVal( \
             (secAry)[ulElmMac], \
             (secAry)[nextElmMacUL] \
          ); \
          \
          lastElmMacUL = ulElmMac;\
          elmOnMacUL = ulElmMac;\
          \
          while(lastElmMacUL >= subMacUL)\
          { /*loop; move swapped element back*/\
            lastElmMacUL -= subMacUL;\
            \
            if(  (firstAry)[elmOnMacUL]\
               > (firstAry)[lastElmMacUL]\
            ) break; /*Positioned the element*/\
            \
            \
            swapVal( \
               (firstAry)[elmOnMacUL], \
               (firstAry)[lastElmMacUL] \
            ); \
            \
            swapVal( \
               (secAry)[elmOnMacUL], \
               (secAry)[lastElmMacUL] \
            ); \
            \
            elmOnMacUL = lastElmMacUL;\
          } /*loop; move swapped element back*/\
        } /*If I need to swap elements*/\
      } /*Loop; swap each nth element of the subarray*/\
    } /*For each element in the subarray*/\
    \
    subMacUL = (subMacUL - 1) / 3; /*Move to next round*/\
  } /*loop through all sub arrays to sort the subarrays*/\
} /*sortTwoNumAry_shellsort*/

/*-------------------------------------------------------\
| Fun06: threeArySortNumeric
|  - Sorts three arrays by the first input array (as
|    numeric) with shell short. All three arrays must be
|    the same size.
| Input:
|  - firstAry:
|    o First array, this is the array to sort
|  - secAry:
|    o Second array to keep sorted with first array
|  - thirdAry:
|    o Third arra to keep sorted with the first array
|  - startUL:
|    o First element to start sorting at
|  - endUL:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o First array to be sorted form least to greatest
|    o secAry to be kept in order with firstAry
|    o thirdAry to be kept in order with firstAry
\-------------------------------------------------------*/
#define \
threeArySortNumeric( \
   firstAry, \
   secAry, \
   thirdAry,\
   startUL, \
   endUL \
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '  - Sorts three arrays by the first input array (as
   '    numeric) with shell short. All three arrays must
   '    be the same size.
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun06 sec01:
   '    - Variable declerations
   '  o fun06 sec02:
   '    - Find the number of rounds to sort for
   '  o fun06 sec03:
   '    - Sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun06 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Number of elements to sort*/\
  unsigned long numElmMacUL = (endUL) - (startUL);\
  long swapMacL = 0;\
  \
  /*Number of sorting rounds*/\
  unsigned long subMacUL = 0;\
  unsigned long nextElmMacUL = 0;\
  unsigned long lastElmMacUL = 0;\
  unsigned long elmOnMacUL = 0;\
  \
  /*Get arrays to sort from the matrix (for sanity)*/\
  \
  /*Variables to incurment loops*/\
  unsigned long ulIndexMac = 0;\
  unsigned long ulElmMac = 0;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun06 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/\
  subMacUL = 1; /*Initialzie first array*/\
  \
  while(subMacUL < numElmMacUL - 1)\
     subMacUL = (3 * subMacUL) + 1;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun06 Sec03:
  ^  - Sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  while(subMacUL > 0)\
  { /*loop trhough all sub arrays sort the subarrays*/\
    for( \
       ulIndexMac = 0;\
       ulIndexMac <= subMacUL;\
       ++ulIndexMac\
    ){ /*For each element in the subarray*/\
      for(ulElmMac = ulIndexMac;\
          ulElmMac + subMacUL <= endUL;\
          ulElmMac += subMacUL\
      ){ /*Loop; swap each nth element of the subarray*/\
        nextElmMacUL = ulElmMac + subMacUL;\
        \
        if((firstAry)[ulElmMac]>(firstAry)[nextElmMacUL])\
        { /*If I need to swap an element*/\
          swapVal( \
             (firstAry)[ulElmMac], \
             (firstAry)[nextElmMacUL] \
          ); \
          \
          swapVal( \
             (secAry)[ulElmMac], \
             (secAry)[nextElmMacUL] \
          ); \
          \
          swapVal( \
             (thirdAry)[ulElmMac], \
             (thirdAry)[nextElmMacUL] \
          ); \
          \
          lastElmMacUL = ulElmMac;\
          elmOnMacUL = ulElmMac;\
          \
          while(lastElmMacUL >= subMacUL)\
          { /*loop; move swapped element back*/\
            lastElmMacUL -= subMacUL;\
            \
            if(  (firstAry)[elmOnMacUL]\
               > (firstAry)[lastElmMacUL]\
            ) break; /*Positioned the element*/\
            \
            \
            swapVal( \
               (firstAry)[elmOnMacUL], \
               (firstAry)[lastElmMacUL] \
            ); \
            \
            swapVal( \
               (secAry)[elmOnMacUL], \
               (secAry)[lastElmMacUL] \
            ); \
            \
            swapVal( \
               (thirdAry)[elmOnMacUL], \
               (thirdAry)[lastElmMacUL] \
            ); \
            \
            elmOnMacUL = lastElmMacUL;\
          } /*loop; move swapped element back*/\
        } /*If I need to swap elements*/\
      } /*Loop; swap each nth element of the subarray*/\
    } /*For each element in the subarray*/\
    \
    subMacUL = (subMacUL - 1) / 3; /*Move to next round*/\
  } /*loop through all sub arrays to sort the subarrays*/\
} /*threeArySortNumeric*/

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
