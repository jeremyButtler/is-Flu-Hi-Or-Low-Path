/*#########################################################
# Name: isHiPathFlu
# Use:
#  - Determines if a flu strain is high or low path by the
#    P1 to P6 amino acids in the HA gene
#  - This is functions for the python wrapper.
# Libraries
#  - "hiLowPathFun.h"      (No .c file)
#  - "getHA2Start.h"       (No .c file)
#  o "dataTypeShortHand.h" (No .c file)
#  o "hiLowPathTbls.h"     (No .c file)
#  o "cStrToNumberFun.h"   (No .c file)
#  o "seqStruct.h"         (No .c file)
# C Stanard Libraries:
#  - <Python.h>
#  o <stdlib.h>
#  o <sdtio.h>
#  o <stdint.h>
#  o <string.h>
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' pyIsHiLowPathFlu SOF: Start Of File
'  - Python wrapper for isHiLowPathFlu
'  o header:
'    - Has includes and default variables
'  o fun-01 isHiOrLowPathFlu:
'    - Determines if flu strain is a hi or low path strain
'  o struct-01 isHiLowPathFluST:
'    - Python object to hold informaton about each function
'      in this script
'  o struct-02 isHiLowPathModule:
'    - Python object holding the information for this
'      library
'  o fun-02 PyInit_fluHiLowPath:
'    - Python function to initialize this libarary
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Has includes and default variables
\--------------------------------------------------------*/

#define PY_SSIZE_T_CLEAN /*Allows s# in pyArg_ParseTuple**/
#include <Python.h>
#include "../hiLowPathFun.h"
#include "../getHA2Start.h"

/*--------------------------------------------------------\
| Name: isHiOrLowPath
| Use:
|  - Tells if a flu HA sequence is hi or low path
|  - Also returns the P1 to P6 amino acids
| Input:
|  - seq: [seq="" or 1st paramater; Required]
|    o HA sequence to check if is hi or low path
|  - tbl: [tbl="" or second parameter; Required]
|    o Feature table to get the start of the HA2 gene from
|    o This can be ignored/disabled by using HA2Start
|  - HA2Start: [HA2Start=0 or third parameter]
|    o Position of the first base in the HA2 gene
|    o This disables tbl.
|    o This needs to be as index 0 (length - 1)
| Output:
|   - Returns:
|     o A list with three elements
|       - 1st element (ret[0]) is true if the HA sequence
|         was from a hi path flu strain
|       - 2nd element (ret[1]) is true if the P2 position
|         was a phe or try
|       - 3rd element (ret[2]) is a string with the P1
|         (first) to P6 (last) amino acids
\--------------------------------------------------------*/
static PyObject *hiOrLowPathHA(
   PyObject *self, /*The function*/
   PyObject *args, /*User input*/
   PyObject *kw    /*Key words provied with args*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: isHiOrLowPathFlu
   '  - Determines if flu strain is a hi or low path strain
   '  o fun-01 sec-01:
   '    - Variable declerations
   '  o fun-01 sec-02:
   '    - Get and check user input
   '  o fun-01 sec-03:
   '    - Get/make sure have the HA2 starting position
   '  o fun-01 sec-04:
   '    - Get the P1 to P6 amino acid sequence
   '  o fun-01 sec-05:
   '    - Check high path/low path & other P1 to P6 markers
   '  o fun-01 sec-06:
   '    - Clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *seqStr = 0;
   char *featureTblStr = 0;
   ulong HA2StartUL = 0;
   ulong errUL = 0; /*Gets errors from ParseTuple*/

   char *p1ToP6AaStr = 0;

   char hiPathBl = 0;
   char p2VirulBl = 0; /*is a phe or try*/

   FILE *testFILE = 0;

   PyObject *retObj;

   static char *keywordsAry[]={"seq","tbl","HA2Start",NULL};

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^  - Get and check user input
   ^  o fun-01 sec-02 sub-01:
   ^    - get user input
   ^  o fun-01 sec-02 sub-02:
   ^    - check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /******************************************************\
   * Fun-01 Sec-02 Sub-01:
   *  - get user input
   \******************************************************/

   errUL =
      PyArg_ParseTupleAndKeywords(
         args,
         kw,
         "s|sk",
         keywordsAry,
         &seqStr,
         &featureTblStr,
         &HA2StartUL
   ); /*Get the input values*/

   /******************************************************\
   * Fun-01 Sec-02 Sub-02:
   *  - check user input
   \******************************************************/

   if(!errUL || seqStr == 0)
   { /*If: the minimum input was not provided*/
      PyErr_SetString(
         PyExc_ValueError,
         "No sequence (seq=value) provied\n"
      );
      return NULL;
   } /*If: the minimum input was not provided*/

   if(HA2StartUL == 0 && featureTblStr == 0)
   { /*If: there is no way to get HA2 starting position*/
         PyErr_SetString(
           PyExc_ValueError,
           "Input feature table (tbl=) or HA2 starting position (HA2Start=)\n"
         );

         return NULL;
   } /*If: there is no way to get HA2 starting position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^  - Get/make sure have the HA2 starting position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(HA2StartUL == 0)
   { /*Else If: user did not provide the HA2 start position*/
      testFILE = fopen(featureTblStr, "r");

      if(testFILE == 0)
      { /*If: the freature table could not be opened*/
         PyErr_SetString(
           PyExc_ValueError,
           "Feature table (tbl=) could not be opened\n"
         );

         return NULL;
      } /*If: the feature table could not be opened*/

      fclose(testFILE);
      testFILE = 0;

      HA2StartUL = getHA2Start(featureTblStr);

      if(HA2StartUL == 0)
      { /*If: I could not extract HA2 starting position*/
       PyErr_SetString(
         PyExc_ValueError,
         "Could not extract HA2 starting position\n"
       );
       return NULL;
      } /*If: I could not extract HA2 starting position*/

   } /*If: user did not provide the HA2 start position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^  - Get the P1 to P6 amino acid sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    p1ToP6AaStr = getP1ToP6AA(seqStr, HA2StartUL);

    if(p1ToP6AaStr == 0)
    { /*If: had a memory error*/
       PyErr_NoMemory();
       return NULL;
    } /*If: had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^  - Check high path/low path & other P1 to P6 markers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    hiPathBl = isHiOrLowPath(p1ToP6AaStr);
    p2VirulBl = isP2PheTryMut(p1ToP6AaStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-06:
   ^  - Clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    retObj =
       Py_BuildValue("[OOs]",            /*O=pyton object*/
          hiPathBl ? Py_True: Py_False,  /*To python bool*/
          p2VirulBl ? Py_True: Py_False, /*To python bool*/
          p1ToP6AaStr
    );

    free(p1ToP6AaStr);
    return retObj;
} /*isHiOrLowPathFlu*/

/*--------------------------------------------------------\
| Struct-01 isHiLowPathFluST:
|  - Python object to hold informaton about each function
|    in this script
\--------------------------------------------------------*/
static PyMethodDef isHiLowPathFluST[]=
{
   {
      "hiOrLowPathHA",
      hiOrLowPathHA,
      METH_VARARGS | METH_KEYWORDS,
      "\
       hiOrLowPathHA(seq=HA_sequence,HA2Start=HA2_Start)\n\
       Use:\n\
        - Checks to see if a flu strain is high path or\n\
          low path.\n\
        - Checks if flu is high path be comparing to a\n\
          few known hi path flu P1 to P6 amino acid\n\
          sequences\n\
          o P1'-R-R\n\
          o P1'-R-K\n\
          o P1'-R-X-K\n\
          o P1'-R-X-R\n\
       Input:\n\
        - seq: [seq=""; Required]\n\
          o HA sequence to check if is hi or low path\n\
        - tbl: [tbl=""; Required]\n\
          o Feature table to get the start of the HA2\n\
            gene from.\n\
          o This can be ignored by using HA2Start\n\
        - HA2Start: [HA2Start=0]\n\
          o Position of the first base in the HA2 gene\n\
          o This disables tbl.\n\
          o This needs to be as index 0 (length - 1)\n\
       Output:\n\
        - Returns:\n\
          o A list with three elements\n\
            - The first element (ret[0]) is true if the\n\
              HA sequence was from a hi path flu strain\n\
            - The second element (ret[1]) is true if the\n\
              P2 position was a phe or try\n\
            - The third element is a string with the\n\
              P1 (first) to P6 (last) amino acids\n\
       " /*Documenation*/
   },

   {NULL, NULL, 0, NULL} /*Empty/NULL entry*/
}; /*isHiLowPathFluST*/

/*--------------------------------------------------------\
| Struct-02: isHiLowPathModule
|  - Python object holding the information for this library
\--------------------------------------------------------*/
static struct PyModuleDef isHiLowPathModule = {
   PyModuleDef_HEAD_INIT,
   "checkFluHiOrLowPath",
   "fluHiLowPath determines if a flu strain is high or\n\
    low path. See the doc string for isHiOrLowPathHA for\n\
    more details.\n\
   ",
   -1,
   isHiLowPathFluST
};

/*--------------------------------------------------------\
| Fun-02: PyInit_fluHiLowPath
|  - Python function to initialize this libarary
\--------------------------------------------------------*/
PyMODINIT_FUNC PyInit_checkFluHiOrLowPath(void)
   {return PyModule_Create(&isHiLowPathModule);}
