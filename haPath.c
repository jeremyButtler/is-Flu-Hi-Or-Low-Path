/*########################################################
# Name: hiPath
# Use:
#  - Holds functions used to determine if a flu strain is
#    a high path or low path strain
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hiLowPathFun SOF: Start Of File
'  - Holds functions to find if flu strain is hi/low path
'  o Header:
'    - defined variable and guards
'  o .c tbl01 baseToCodeLkTbl:
'    - Table to convert bases to codes used in the codon
'      table.
'  o .c tbl02 aaTbl_haPath:
'    - Table to convert three bases to codons
'  o fun01: find_haPath
'    - determines if the cleavege sight is an high or low
'      path cleave site.
'  o fun02: P2PheTryMut_haPath
'    - determines if the P2 site is an phe or try. Was
'      found to be more virulent in H1N1.
'  o fun03: getP1_P6AA_haPath:
'    - Get the cleavege site (P1 to P6) amino acid
'      sequence from the flu HA gene.
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "haPath.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define t_codon_tbl 0
#define c_codon_tbl 1
#define a_codon_tbl 2
#define g_codon_tbl 3
#define n_codon_tbl 4
#define err_codon_tbl 8

/*-------------------------------------------------------\
| Tbl01 baseToCodeLkTbl:
|  - Table to convert bases to codes used in the codon
|    table.
\-------------------------------------------------------*/
unsigned char ntToIndexTbl_haPath[] =
   {  /*baseToCodeLkTbl*/
      /*White space/invisible charactes block*/
      err_codon_tbl, /*0   = Null character*/

      err_codon_tbl, /*1   = Start of Heading*/
      err_codon_tbl, /*2   = Start of Text*/
      err_codon_tbl, /*3   = End of Text*/
      err_codon_tbl, /*4   = End of Transmission*/
      err_codon_tbl, /*5   = Enquiry*/
      err_codon_tbl, /*6   = Acknowledge*/
      err_codon_tbl, /*7   = Bell*/
      err_codon_tbl, /*8   = Backspace*/

      err_codon_tbl, /*9   =  tab (horizontal)*/
      err_codon_tbl, /*10  = New line*/

      err_codon_tbl, /*11  = Vertical Tab (not key)*/
      err_codon_tbl, /*12  = Form Feed*/

      err_codon_tbl, /*13  = Carriage Return*/

      err_codon_tbl, /*14  = Shift Out*/
      err_codon_tbl, /*15  = Shift In*/
      err_codon_tbl, /*16  = Data Link Escape*/
      err_codon_tbl, /*17  = Device Control One*/
      err_codon_tbl, /*18  = Device Control Two*/
      err_codon_tbl, /*19  = Device Contol Three*/
      err_codon_tbl, /*20  = Device Control Four*/
      err_codon_tbl, /*21  = Negative Acknowledge*/
      err_codon_tbl, /*22  = Synchronous Idle*/
      err_codon_tbl, /*23  = End Transmission Block*/
      err_codon_tbl, /*24  = Cancle*/
      err_codon_tbl, /*25  = End of medium*/
      err_codon_tbl, /*26  = Substitute*/
      err_codon_tbl, /*27  = escape*/
      err_codon_tbl, /*28  = File Separator*/
      err_codon_tbl, /*29  = Group Separator*/
      err_codon_tbl, /*30  = Record Separator*/
      err_codon_tbl, /*31  = Unit Separator*/

      /*symbol/number block*/
      err_codon_tbl, /*32  = space*/
      err_codon_tbl, /*33  = !*/
      err_codon_tbl, /*34  = "*/
      err_codon_tbl, /*35  = #*/
      err_codon_tbl, /*36  = $*/
      err_codon_tbl, /*37  = %*/
      err_codon_tbl, /*38  = &*/
      err_codon_tbl, /*39  = '*/
      err_codon_tbl, /*40  = (*/
      err_codon_tbl, /*41  = )*/
      err_codon_tbl, /*42  = **/
      err_codon_tbl, /*43  = +*/
      err_codon_tbl, /*44  = ,*/
      err_codon_tbl, /*45  = -*/
      err_codon_tbl, /*46  = .*/
      err_codon_tbl, /*47  = /*/
      err_codon_tbl, /*48  = 0*/
      err_codon_tbl, /*49  = 1*/
      err_codon_tbl, /*50  = 2*/
      err_codon_tbl, /*51  = 3*/
      err_codon_tbl, /*52  = 4*/
      err_codon_tbl, /*53  = 5*/
      err_codon_tbl, /*54  = 6*/
      err_codon_tbl, /*55  = 7*/
      err_codon_tbl, /*56  = 8*/
      err_codon_tbl, /*57  = 9*/
      err_codon_tbl, /*58  = :*/
      err_codon_tbl, /*59  = ;*/
      err_codon_tbl, /*60  = <*/
      err_codon_tbl, /*61  = =*/
      err_codon_tbl, /*62  = >*/
      err_codon_tbl, /*63  = ?*/
      err_codon_tbl, /*64  = @*/

      /*Uppercase letters*/
      a_codon_tbl,   /*65  = A*/
      n_codon_tbl,   /*66  = B, (C/G/T) treat as N*/
      c_codon_tbl,   /*67  = C*/
      n_codon_tbl,   /*68  = D (AGT), treat as N*/
      err_codon_tbl, /*69  = E not nucleotide*/
      err_codon_tbl, /*70  = F not nucleotide*/
      g_codon_tbl,   /*71  = G*/
      n_codon_tbl,   /*72  = H (ACT) treat as N*/
      err_codon_tbl, /*73  = I not nucleotide*/
      err_codon_tbl, /*74  = J not nucleotide*/
      n_codon_tbl,   /*75  = K (GT), treat as N*/
      err_codon_tbl, /*76  = L not nucleotide*/
      n_codon_tbl,   /*77  = M (AC), treat as N*/
      n_codon_tbl,   /*78  = N (AGCT)*/
      err_codon_tbl, /*79  = O not nucleotide*/
      err_codon_tbl, /*80  = P not nucleotide*/
      err_codon_tbl, /*81  = Q not nucleotide*/
      n_codon_tbl,   /*82  = R (AG), treat as N*/
      n_codon_tbl,   /*83  = S (CG), treat as N*/
      t_codon_tbl,   /*84  = T*/
      t_codon_tbl,   /*85  = U*/
      n_codon_tbl,   /*86  = V (ACG), treat as N*/
      n_codon_tbl,   /*87  = W (AT), treat as N*/
      n_codon_tbl,   /*88  = X is N for amino acid*/
      n_codon_tbl,   /*89  = Y (CT), treat as N*/
      err_codon_tbl, /*90  = Z not nucleotide*/

      /*Special characters after upercase letters*/
      err_codon_tbl, /*91  = [*/
      err_codon_tbl, /*92  = \*/
      err_codon_tbl, /*93  = ]*/
      err_codon_tbl, /*94  = ^*/
      err_codon_tbl, /*95  = _*/
      err_codon_tbl, /*96  = `*/

      /*lower case letters*/
      a_codon_tbl,   /*97  = a*/
      n_codon_tbl,   /*98  = b, (C/G/T) treat as N*/
      c_codon_tbl,   /*99  = c*/
      n_codon_tbl,   /*100 = d (AGT), treat as N*/
      err_codon_tbl, /*101 = e not nucleotide*/
      err_codon_tbl, /*102 = f not nucleotide*/
      g_codon_tbl,   /*103 = g*/
      n_codon_tbl,   /*104 = h (ACT) treat as N*/
      err_codon_tbl, /*105 = i not nucleotide*/
      err_codon_tbl, /*106 = j not nucleotide*/
      n_codon_tbl,   /*107 = k (GT), treat as N*/
      err_codon_tbl, /*108 = l not nucleotide*/
      n_codon_tbl,   /*109 = m (AC), treat as N*/
      n_codon_tbl,   /*110 = n (AGCT)*/
      err_codon_tbl, /*111 = o not nucleotide*/
      err_codon_tbl, /*112 = p not nucleotide*/
      err_codon_tbl, /*113 = q not nucleotide*/
      n_codon_tbl,   /*114 = r (AG), treat as N*/
      n_codon_tbl,   /*115 = s (CG), treat as N*/
      t_codon_tbl,   /*116 = t*/
      t_codon_tbl,   /*117 = u*/
      n_codon_tbl,   /*118 = v (ACG), treat as N*/
      n_codon_tbl,   /*119 = w (AT), treat as N*/
      n_codon_tbl,   /*120 = x is N for amino acid*/
      n_codon_tbl,   /*121 = y (CT), treat as N*/
      err_codon_tbl, /*122 = z not nucleotide*/

      /*Special characters after lowercase letters*/
      err_codon_tbl, /*123 = {*/
      err_codon_tbl, /*124 = |*/
      err_codon_tbl, /*125 = }*/
      err_codon_tbl, /*126 = ~*/
      err_codon_tbl, /*127 = Del*/
   }; /*baseToCodeLkTbl*/

/*--------------------------------------------------------\
| Tbl02 aaTbl_haPath:
|  - Table to convert three bases to codons
\--------------------------------------------------------*/
char aaTbl_haPath[5][5][5] =
   {  /*codonLkTbl*/
      { /*First element is an T*/
         {'f', 'f', 'l', 'l', 'x'}, /*2nd element is T*/
         {'s', 's', 's', 's', 's'}, /*2nd element is C*/
         {'y', 'y', '*', '*', 'x'}, /*2nd element is A*/
         {'c', 'c', '*', 'w', 'x'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an T*/

      { /*First element is an C*/
         {'l', 'l', 'l', 'l', 'l'}, /*2nd element is T*/
         {'p', 'p', 'p', 'p', 'p'}, /*2nd element is C*/
         {'h', 'h', 'q', 'q', 'x'}, /*2nd element is A*/
         {'r', 'r', 'r', 'r', 'r'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an C*/

      { /*First element is an A*/
         {'i', 'i', 'i', 'm', 'x'}, /*2nd element is T*/
         {'t', 't', 't', 't', 't'}, /*2nd element is C*/
         {'n', 'n', 'k', 'k', 'x'}, /*2nd element is A*/
         {'s', 's', 'r', 'r', 'x'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an A*/

      { /*First element is an G*/
         {'v', 'v', 'v', 'v', 'v'}, /*2nd element is T*/
         {'a', 'a', 'a', 'a', 'a'}, /*2nd element is C*/
         {'d', 'd', 'e', 'e', 'x'}, /*2nd element is A*/
         {'g', 'g', 'g', 'g', 'g'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an G*/

      { /*First element is an N*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is T*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is C*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is A*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an N*/
   }; /*codonLkTbl*/

/*-------------------------------------------------------\
| Fun03: getP1_P6AA_haPath
| Use:
|  - Find the amino acids for the P1 to P6 (cleavege site)
|    positions in the HA gene
| Input:
|  - HaSeqStr:
|    - String with the sequence of the HA gene
|  - p1ToP6AA:
|    - string to hold p1 to p6 aa sequence
|    - length of 7 (6 aa + '\0')
|  - startHAUL:
|    - Starting point of the HA2 segement in the HA gene
| Output:
|  - Modifies:
|    o p1ToP6Str to hold P1 (index 0) to P6 (index 5)
|      ammino acids.
\-------------------------------------------------------*/
void
getP1_P6AA_haPath(
   char *HaSeqStr,   /*Ha sequence to precess*/
   char *p1ToP6Str,  /*will have P1 to P6 aa sequence*/
   unsigned long startHaUL   /*Ha2 gene start (index 0)*/
){
   unsigned char uc1 = 0;
   unsigned char uc2 = 0;
   unsigned char uc3 = 0;
   unsigned char ucAA = 0;

   p1ToP6Str[def_numPSites_haPath] = '\0'; /*to C-string*/

   /*Move to the last base in the P1 codon*/
   HaSeqStr = HaSeqStr + startHaUL - 1;

   for(
      ucAA = 0;
      ucAA < def_numPSites_haPath;
      ++ucAA
   ){ /*Loop: Get P2 to P6 amino acids*/
      uc3= ntToIndexTbl_haPath[(unsigned char) *HaSeqStr];
      --HaSeqStr;

      uc2= ntToIndexTbl_haPath[(unsigned char) *HaSeqStr];
      --HaSeqStr;

      uc1= ntToIndexTbl_haPath[(unsigned char) *HaSeqStr];
      --HaSeqStr;

      p1ToP6Str[ucAA] = aaTbl_haPath[uc1][uc2][uc3];
   } /*Loop: Get P2 to P6 amino acids*/
} /*getP1_P6AA_haPath*/
