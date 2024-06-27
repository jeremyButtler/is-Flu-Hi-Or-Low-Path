/*########################################################
# Name: ntToTwoBit
#   - had tables to convert nucleodtides to two bits
#   - the third bit holds a error bit that marks anonymous
#     base or non-nucleotide characters. it can be ingored
#     after checks
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards and defined variables
'   o tbl01: ntToTwoBit
'     - table to convert bases to four bit (a/t/g/c only)
'       values to two bits with the 3rd bit saved for an
'       error/anonymous base
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and defined variables
\-------------------------------------------------------*/

#ifndef NUCLEOTIDE_TO_TWO_BIT_H
#define NUCLEOTIDE_TO_TWO_BIT_H

#define a_twoBit 0
#define c_twoBit 1
#define g_twoBit 2
#define t_twoBit 3
#define err_threeBit 1 << 2

/*-------------------------------------------------------\
| Tbl-01: ntToTwoBit
|  - table to convert bases to four bit (a/t/g/c only)
|    values to two bits with the 3rd bit saved for an
|    error/anonymous base
|    o 1st bit is A (a_fourBit)
|    o 2nd bit is C (c_fourBit)
|    o 3rd bit is G (g_fourBit)
|    o 4th bit is T/U (t_fourBit)
\-------------------------------------------------------*/
static
unsigned char ntToTwoBit[] =
{  /*ntToTwoBit*/
   /*White space/invisible charactes block*/
   err_threeBit, /*0   = Null character*/

   err_threeBit, /*1   = Start of Heading*/
   err_threeBit, /*2   = Start of Text*/
   err_threeBit, /*3   = End of Text*/
   err_threeBit, /*4   = End of Transmission*/
   err_threeBit, /*5   = Enquiry*/
   err_threeBit, /*6   = Acknowledge*/
   err_threeBit, /*7   = Bell*/
   err_threeBit, /*8   = Backspace*/

   err_threeBit, /*9   =  tab (horizontal)*/
   err_threeBit, /*10  = New line*/

   err_threeBit, /*11  = Vertical Tab (not key)*/
   err_threeBit, /*12  = Form Feed*/

   err_threeBit, /*13  = Carriage Return*/

   err_threeBit, /*14  = Shift Out*/
   err_threeBit, /*15  = Shift In*/
   err_threeBit, /*16  = Data Link Escape*/
   err_threeBit, /*17  = Device Control One*/
   err_threeBit, /*18  = Device Control Two*/
   err_threeBit, /*19  = Device Contol Three*/
   err_threeBit, /*20  = Device Control Four*/
   err_threeBit, /*21  = Negative Acknowledge*/
   err_threeBit, /*22  = Synchronous Idle*/
   err_threeBit, /*23  = End Transmission Block*/
   err_threeBit, /*24  = Cancle*/
   err_threeBit, /*25  = End of medium*/
   err_threeBit, /*26  = Substitute*/
   err_threeBit, /*27  = escape*/
   err_threeBit, /*28  = File Separator*/
   err_threeBit, /*29  = Group Separator*/
   err_threeBit, /*30  = Record Separator*/
   err_threeBit, /*31  = Unit Separator*/

   /*symbol/number block*/
   err_threeBit, /*32  = space*/
   err_threeBit, /*33  = !*/
   err_threeBit, /*34  = "*/
   err_threeBit, /*35  = #*/
   err_threeBit, /*36  = $*/
   err_threeBit, /*37  = %*/
   err_threeBit, /*38  = &*/
   err_threeBit, /*39  = '*/
   err_threeBit, /*40  = (*/
   err_threeBit, /*41  = )*/
   err_threeBit, /*42  = **/
   err_threeBit, /*43  = +*/
   err_threeBit, /*44  = ,*/
   err_threeBit, /*45  = -*/
   err_threeBit, /*46  = .*/
   err_threeBit, /*47  = /*/
   err_threeBit, /*48  = 0*/
   err_threeBit, /*49  = 1*/
   err_threeBit, /*50  = 2*/
   err_threeBit, /*51  = 3*/
   err_threeBit, /*52  = 4*/
   err_threeBit, /*53  = 5*/
   err_threeBit, /*54  = 6*/
   err_threeBit, /*55  = 7*/
   err_threeBit, /*56  = 8*/
   err_threeBit, /*57  = 9*/
   err_threeBit, /*58  = :*/
   err_threeBit, /*59  = ;*/
   err_threeBit, /*60  = <*/
   err_threeBit, /*61  = =*/
   err_threeBit, /*62  = >*/
   err_threeBit, /*63  = ?*/
   err_threeBit, /*64  = @*/

   /*Uppercase letters*/
   a_twoBit,   /*65  = A*/

   err_threeBit, /*66=B; C/G/T*/

   c_twoBit,   /*67  = C*/


   err_threeBit, /*68=D; A/G/T*/
   err_threeBit, /*69  = E not nucleotide*/
   err_threeBit, /*70  = F not nucleotide*/

   g_twoBit,     /*71  = G*/

   err_threeBit, /*72=H; A/G/T*/
   err_threeBit, /*73  = I not nucleotide*/
   err_threeBit, /*74  = J not nucleotide*/
   err_threeBit, /*75=K; G/T*/
   err_threeBit, /*76  = L not nucleotide*/
   err_threeBit, /*77=M; A/C*/

   err_threeBit, /*78=N; any base*/
   err_threeBit, /*79  = O not nucleotide*/
   err_threeBit, /*80  = P not nucleotide*/
   err_threeBit, /*81  = Q not nucleotide*/
   err_threeBit, /*82=R A/G*/
   err_threeBit, /*83=S C/G*/

   t_twoBit,   /*84  = T*/
   t_twoBit,   /*85  = U*/

   err_threeBit, /*86  = V (ACG), treat as N*/
   err_threeBit, /*87=W A\T*/
   err_threeBit, /*88=X; any aa*/
   err_threeBit,   /*89=Y C/T*/
   err_threeBit, /*90  = Z not nucleotide*/

   /*Special characters after upercase letters*/
   err_threeBit, /*91  = [*/
   err_threeBit, /*92  = \*/
   err_threeBit, /*93  = ]*/
   err_threeBit, /*94  = ^*/
   err_threeBit, /*95  = _*/
   err_threeBit, /*96  = `*/

   /*lower case letters*/
   a_twoBit,   /*97=a*/

   err_threeBit, /*98=b; C/G/T*/

   c_twoBit,   /*99  = c*/

   err_threeBit, /*100=d; A/G/T*/
   err_threeBit, /*101 = e not nucleotide*/
   err_threeBit, /*102 = f not nucleotide*/
   g_twoBit,   /*103 = g*/
   err_threeBit, /*104=h; A/G/T*/
   err_threeBit, /*105 = i not nucleotide*/
   err_threeBit, /*106 = j not nucleotide*/
   err_threeBit, /*107=k; G/T*/
   err_threeBit, /*108  = l not nucleotide*/
   err_threeBit, /*109=m; A/C*/

   err_threeBit, /*110=n; any base*/

   err_threeBit, /*111 = o not nucleotide*/
   err_threeBit, /*112 = p not nucleotide*/
   err_threeBit, /*113 = q not nucleotide*/
   err_threeBit, /*114=r A/G*/
   err_threeBit, /*115=s C/G*/
   t_twoBit,     /*116 = t*/
   t_twoBit,     /*117 = u*/

   
   err_threeBit, /*118 = v (ACG), treat as N*/
   err_threeBit, /*119=w A\T*/
   err_threeBit, /*120 = x*/
   err_threeBit,   /*121=y C/T*/
   err_threeBit, /*122 = z not nucleotide*/

   /*Special characters after lowercase letters*/
   err_threeBit, /*123 = {*/
   err_threeBit, /*124 = |*/
   err_threeBit, /*125 = }*/
   err_threeBit, /*126 = ~*/
   err_threeBit, /*127 = Del*/
}; /*ntToTwoBit*/

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the publikc domain
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
