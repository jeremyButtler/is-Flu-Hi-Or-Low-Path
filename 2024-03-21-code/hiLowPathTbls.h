/*#########################################################
# Name: hiLow
# Use:
#  - Holds look up tables for high/low path flu detection
# Libraries:
# C Standard Libraries:
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hiLowPathLookUpTbls TOT: Table Of Tables
'  - Table Of Contents for look up tables
'  o tbl-01 baseToCodeLkTbl:
'    - Table to convert bases to codes used in the codon
'      table.
'  o tbl-02 codonLkTbl:
'    - Table to convert three bases to codons
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Tbl-01 baseToCodeLkTbl:
|  - Table to convert bases to codes used in the codon
|    table.
\--------------------------------------------------------*/

static char baseToCodeLkTbl[] =
   {  /*baseToCodeLkTbl*/
      /*White space/invisible charactes block*/
      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

      /*symbol/number block*/
      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 

      2, /*A (65)*/
      4, /*B, (C/G/T) treat as N*/
      1, /*C*/
      4, /*D (AGT), treat as N*/
      8, 8,
      3, /*G*/
      4, /*H (ACT) treat as N*/
      8, 8,
      4, /*K (GT), treat as N*/
      8,
      4, /*M (AC), treat as N*/
      4, /*N (AGCT)*/
      8, 8, 8,
      4, /*R (AG), treat as N*/
      4, /*S (CG), treat as N*/
      0, /*T*/
      0, /*U*/
      4, /*V (ACG), treat as N*/
      8,
      4, /*X, I treat as an N, but is for amino acids*/
      8, 8,

      /*Special characters after upercase letters*/
      8, 8, 8, 8, 8, 8,

      /*lower case letters*/
      2, /*a (65)*/
      4, /*b, (c/g/t) treat as n*/
      1, /*c*/
      4, /*d (agt), treat as n*/
      8, 8,
      3, /*g*/
      4, /*h (act) treat as n*/
      8, 8,
      4, /*k (gt), treat as n*/
      8,
      4, /*m (ac), treat as n*/
      4, /*n (agct)*/
      8, 8, 8,
      4, /*r (ag), treat as n*/
      4, /*s (cg), treat as n*/
      0, /*t*/
      0, /*u*/
      4, /*v (acg), treat as n*/
      8,
      4, /*x, i treat as an n, but is for amino acids*/
      8, 8,

      /*Special characters after lowercase letters*/
      8, 8, 8, 8, 8, 8,
   }; /*baseToCodeLkTbl*/

/*--------------------------------------------------------\
| Tbl-02 codonLkTbl:
|  - Table to convert three bases to codons
\--------------------------------------------------------*/
static char codonLkTbl[5][5][5] =
   {  /*codonLkTbl*/
      { /*First element is an T*/
         {'f', 'f', 'l', 'l', 'x'}, /*2nd element is an T*/
         {'s', 's', 's', 's', 's'}, /*2nd element is an C*/
         {'y', 'y', '*', '*', 'x'}, /*2nd element is an A*/
         {'c', 'c', '*', 'w', 'x'}, /*2nd element is an G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is an N*/
      }, /*First element is an T*/

      { /*First element is an C*/
         {'l', 'l', 'l', 'l', 'l'}, /*2nd element is an T*/
         {'p', 'p', 'p', 'p', 'p'}, /*2nd element is an C*/
         {'h', 'h', 'q', 'q', 'x'}, /*2nd element is an A*/
         {'r', 'r', 'r', 'r', 'r'}, /*2nd element is an G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is an N*/
      }, /*First element is an C*/

      { /*First element is an A*/
         {'i', 'i', 'i', 'm', 'x'}, /*2nd element is an T*/
         {'t', 't', 't', 't', 't'}, /*2nd element is an C*/
         {'n', 'n', 'k', 'k', 'x'}, /*2nd element is an A*/
         {'s', 's', 'r', 'r', 'x'}, /*2nd element is an G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is an N*/
      }, /*First element is an A*/

      { /*First element is an G*/
         {'v', 'v', 'v', 'v', 'v'}, /*2nd element is an T*/
         {'a', 'a', 'a', 'a', 'a'}, /*2nd element is an C*/
         {'d', 'd', 'e', 'e', 'x'}, /*2nd element is an A*/
         {'g', 'g', 'g', 'g', 'g'}, /*2nd element is an G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is an N*/
      }, /*First element is an C*/

      { /*First element is an N*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is an T*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is an C*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is an A*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is an G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is an N*/
      }, /*First element is an N*/
   }; /*codonLkTbl*/

/*
   Ala A Alanine
   Arg R Arginine
   Asn N Asparagine
   Asp D Aspartic acid
   Cys C Cysteine
   Gln Q Glutamine
   Glu E Glutamic acid
   Gly G Glycine
   His H Histidine
   Ile I Isoleucine
   Lys K Lysine
   Leu L Luecine
   Met M Methionine
   Phe F Phenylalanine
   Pro P Proline
   Ser S Serine
   Thr T Threonine
   Trp W Tryptophan
   Try Y Tyrosine
   Val V Valine
*/

/*Table
     T         C        A        G
  +--------+--------+--------+--------+
  | TTT  F | TCT  S | TAT  Y | TGT  C |
T | TTC  F | TCC  S | TAC  Y | TGC  C | T
  | TTA  L | TCA  S | TAA  * | TGA  * |
  | TTG  L | TCG  S | TAG  * | TGG  W |
  +--------+--------+--------+--------+
  | CTT  L | CCT  P | CAT  H | CGT  R |
C | CTC  L | CCC  P | CAC  H | CGC  R | C
  | CTA  L | CCA  P | CAA  Q | CGA  R |
  | CTG  L | CCG  P | CAG  Q | CGG  R |
  +--------+--------+--------+--------+
  | ATT  I | ACT  T | AAT  N | AGT  S |
A | ATC  I | ACC  T | AAC  N | AGC  S | A
  | ATA  I | ACA  T | AAA  K | AGA  R |
  | ATG  M | ACG  T | AAG  K | AGG  R |
  +--------+--------+--------+--------+
  | GTT  V | GCT  A | GAT  D | GGT  G |
G | GTC  V | GCC  A | GAC  D | GGC  G | G
  | GTA  V | GCA  A | GAA  E | GGA  G |
  | GTG  V | GCG  A | GAG  E | GGG  G |
  +--------+--------+--------+--------+
*/
