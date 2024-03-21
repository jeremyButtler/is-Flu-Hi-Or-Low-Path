# Use:

Checks to see if a flu HA sequence belongs to a high path
  flu or a low path flu strain.

This program checks patterns that other people have found
  (Zhirnov, Ikizler, and Wright 2002 from
   Walker et al., 1992; Kalthoff, Globig, Beer 2009) to
  distinguish between high path and low path flu strains.
  Each pattern starts from the P1' site (first amino acid
  in HA2) and ends with the P3 site (P1'-P1-P2-P3). The
  output tells if the strain was a high (high\_path) or
  low (low\_path) strain. The output also prints out the
  P1 to P6 amino acids and if there was a Phenylalanine or
  Tryosine at the P2. Sun et al., 2010 found a
  Phenylalanine or Tryosine at the P2 position made at
  least one low path H1N1 more virulant.

List of patterns used to find if a strain is high path:

- P1'-R-R
- P1'-R-K
- P1'-R-X-R
- P1'-R-X-K

The one warning I will give is that this program is only as
  good as the patterns I have chosen to check. If I am
  wrong on a pattern or missed a pattern, then the results
  will be wrong. So, double check the P1 to P6 amino acids
  printed out to make sure they line up with the high/low
  path result.

Also, I should note that I am not a flu person. I just
  heard people might be interested in a program like this.
  And that there used to be one, but it disappeared.

This code is licensed under two licenses, the unlicense
  (public domain license) or MIT in cases were the
  unlicense can not be used. You are free to choose the
  license you want.

## Updates

At some point I need to update alnSeq. I have it a few
  versions past the version here.

*20240321*: I fixed a minor error in my codon table 
  "hiLowPathTbls.h" were Phenylalanine was confused with
   Luecine (one case). I also changed this code to
   compile with the c89 standard instead of c99 and added
   in an install option for Mac `make mac` (no idea if it
   works). I also removed `-static` from the python
   library.

## References

Kalthoff D, Globig A, Beer M. (Highly pathogenic) avian
  influenza as a zoonotic agent.  Vet Microbiol.
  2010 Jan 27;140(3-4):237-45.
  doi: 10.1016/j.vetmic.2009.08.022. Epub 2009 Aug 26.
  PMID: 19782482.

Sun X, Tse LV, Ferguson AD, Whittaker GR. Modifications to
  the hemagglutinin cleavage site control the virulence of
  a neurotropic H1N1 influenza virus. J Virol. 2010
  Sep;84(17):8683-90. doi: 10.1128/JVI.00797-10.
  Epub 2010 Jun 16. PMID: 20554779; PMCID: PMC2919019.

Walker JA, Sakaguchi T, Matsuda Y, Yoshida T,
  Kawaoka Y. Location and character of the cellular enzyme
  that cleaves the hemagglutinin of a virulent avian
  influenza virus. Virology. 1992 Sep;190(1):278-87.
  doi: 10.1016/0042-6822(92)91214-f. PMID: 1529533.

Zhirnov OP, Ikizler MR, Wright PF. Cleavage of influenza a
  virus hemagglutinin in human respiratory epithelium is
  cell associated and sensitive to exogenous antiproteases.
  J Virol. 2002 Sep;76(17):8682-9.
  doi: 10.1128/jvi.76.17.8682-8689.2002.
  PMID: 12163588; PMCID: PMC136409.

# Building isHiLowPathFlu

## Building Standalone

```
make
sudo make install
```

## Building the python library

```
# Global
sudo make python

# Local
make pythonlocal
```

# Using this isHiLowPathFlu

## Using the standalone

This program has three options, but only one of the three
  options is needed. The only required option is the fasta
  file (-fasta) to get the HA sequence from. This program
  will align each sequence in the fasta file to an
  consensus of the last 3 bases of H1 and the first 54
  bases of the HA2 gene
  ("arrGGNHTNYHNrGNGCNDWHrYNrKNYKBAT"). However, this is
  only as good as my consensus, so it will likely fail at
  times. To fix this issue you can upload a feature table
  or input the position of the first base (index 1) in the
  HA2 gene (P1' position).

If you are inputting a feature table or the first base in
  HA2, then only one HA sequence is processed. You also
  will need to mark the HA sequence in the fasta file with
  **HA** in the header (Line with an **>**), otherwise
  this program will not extract the HA sequence.

The second option is either to use a feature table to get
  the HA2 starting position from (-tbl) or to manually
  input the HA2 starting position (-HA2-Start)
  (as index 1). The feature table should be made and
  downloaded from 
  [https://www.ncbi.nlm.nih.gov/genomes/FLU/annotation/](
   https://www.ncbi.nlm.nih.gov/genomes/FLU/annotation/).
  Just make sure you correct any errors in the HA sequence
  before hand.

```
isHiLowPathFlu -fasta HA.fasta;

# or

isHiLowPathFlu -fasta HA.fasta -tbl HA-feature-table.tbl;

# or

isHiLowPathFlu -fasta HA.fasta -HA2Start 1026;
```

## Using this code in C

There are two main .h files in this code you will needed
  to know about. The first is hiLowPathFun.h, which
  contains the functions to get the amino acids and
  determines if a result matches a pattern. The second is
  getHA2Start.h, which is used to get the HA2 starting
  position from a feature table and to find the HA sequence
  in a fasta file.

All the other .h files are dependencies these two .h files
  depend on. These files have the dependencies they used
  listed in the first comment block; under the Libraries
  section at the start of each file.

### hiLowPathFun.h

hiLowPathFun.h has a function named getP1ToP6AA (Fun-03),
  which gets the P1 to P6 amino acid sequences from the HA
  sequence. Its input is the HA sequence and the position
  of the first base (as index 0) in the HA2 sequence. Its
  return is an c-string with six amino acids (P1 to P6; in
  lower case).

  `getP1ToP6AA(HA_sequence_str, HA2_start_UL);`

You can put the c-string from getP1ToP6AA into
  isHiOrLowPath(cString) (Fun-01) or isP2PheTryMut(cString)
  (Fun-02). isHiOrLowPath returns a 1 if the c-string has
  a high path sequence and 0 if it has a low path sequence.
  isP2PheTryMut returns a 1 if P2 is a Phenylalanine or an
  Tyrosine and 0 if it is not.

  ```
  if(getHA2Start(p1_to_p6_amino_acids_str) == 1)
     return "high_path";

  if(isP2PheTryMut(p1_to_p6_amino_acids_str) == 1)
     return "p2_is_an_phe_or_try";
  ```

### getHA2Start.h

getHA2Start.h has three functions, getHA2Start (Fun-01),
  which gets the HA2 start position from a feature table,
  getHaSeq (Fun-02), which gets an HA sequence from
  a fasta file, and findHA2Start (Fun-03), which finds the
  HA2 gene by aligning it to a consensus. 

The getHA2Start function (Fun-01) takes in a path to a
  feature table and extracts and returns the starting
  position of the HA2 peptide, if it is present. If their
  is no HA2 peptide, it returns 0.

  `H2_start_position_UL = getHA2Start(feature_table_Str);`
   
The getHaSeq function (Fun-02) takes in a fasta file and
  extracts the first sequence it finds with an "HA" in the
  header ">" entry. It changes the position in the file
  to the next sequence after the extracted HA sequence.
  The return value is an pointer to an seqStruct
  (Struct-01 seqStruct.h) or 0 if no sequence was
  extracted. This seqStruct needs to be freed with
  `freeSeqST(seqStruct, 1);` (Fun-07 seqStruct.h) when you
  are finished with the structure/program. You can access
  or pass the sequence in the seqStruct to other functions
  with seqStruct->seqCStr.

  `getHaSeq(path_to_fasta_str);`

The findHA2Start function (Fun-03) takes in the sequence
  (c-string), the length of the sequence (index 1), a
  pointer to the variable to hold the HA2 position on the
  sequence, and a pointer to the variable to hold the first
  mapped base in the consensus. The returned starting
  positions are in index 0. Also, the HA2 starting position
  on the sequence will included the last three bases of
  HA1 (the P1 position). So, make sure to add three to the
  starting position before calling getP1ToP6AA.

The return value for findHA2Start is the score of the
  alignment if it was kept. However, the score will be 0 if
  it is under 40. The maximum score for the current
  consensus is 63. It is -1 if there was a memory error.

Two problems that might pop up is that a non-HA sequence
  might align to my consensus or that my consensus may map
  to a different position (not start of HA2). To reduce
  this down I would recommend discarding any sequence that
  does not map to the entire consensus.
  
  ```
  score = findHA2Start(sequence, sequence_length, HA2_start, first_consensus_base);`

  if(score < 0)
     Handle memory errors here

  if(score == 0)
     Handle unmapped alignments here

  if(first_consensus_base > 0)
     Handle mistaken alignments here

     /*If first_consensus_base is over 3, then you do not
     `  have the P1' position. If over 0 suggests you may
     `  missed or have an incomplete P1 position.
     */

  HA2_start += 3; /*Move to the P1' position'

  getP1ToP6AA(sequence, HAS_start);
  ```

## Using this code in python

One note I should make. I am assuming if you are coding
  this that you know what index 0 means. If you do not
  know what index 0 is, it means that you are starting your
  count at 0 instead of 1. So, the first item is 0, the
  second item is 1, the third item is 2, ..., and the nth
  item is n - 1.

First make sure you build the library (`make pythonlocal`
  or `sudo make python`). Otherwise, you will not be able
  to use this library in python.

In python import the library and the one function or two
  functions you will use
  `from checkFluHiOrLowPath import hiOrLowPathHA` or
  Then provide the sequence and the position of the first
  base in the H2A segment (at index 0) or the path to the
  feature table to the function.

The return value will be a list with the first element
  (ret[0]) being True if high path or False if low path.
  The third element (ret[2]) is the P1 to P6 amino acid
  sequence. The second element (ret[1]) is True if P2 is an
  Phenylalanine or an Tyrosine.

You can try to find the HA2 starting position with
  findHA2StartPos, which is a wrapper for findHA2Start
  (Fun-03) in the getHA2Start.h code. Import findHA2 start
  with `from checkFluHiOrLowPath import findHA2StartPos`.
  The only parameter in the this function is the sequence.
  The return value is a list with the first element as
  the starting position at index 0 (ret[0]) and the second
  element as the score (ret[1]). The starting position is
  set to -1 if the sequence had a low score and -2 if the
  of the P1 position could not be mapped.

One warning about findHA2Start is that it is dependent on
  how well I made the HA2 consensus sequence. So, it can
  detect false positives, may get the wrong position
  (assign another position to the HA2 position), or may
  even think the sequence is not an HA gene. This


```
from checkFluHiOrLowPath import hiOrLowPathHA, findHA2StartPos

# Alternative 1 (Use the HA2 start position)

retAry = hiOrLowPathHA(seq = HA_sequence, HA2Start = HA2_Position);

# Alternative 2 (Use a feature table)

retAry = hiOrLowPathHA(seq = HA_sequence, tbl = "/path/to/HA2_Feature_table");

# Alternative 3 (Find the HA2 start position)

HA2PosAry = findHA2StartPos(sequence);

if(HA2PosAry[0] > 0):
   retAry = hiOrLowPathHA(seq = sequence, HA2Start = HA2PosAry[0]);

print(retAry[0]); # Is this a high path sequence
print(retAry[3]); # ammino acids at P1 to P6
```

You can check pythonPkg/test.py for a hard coded example of
  how a script might look. You also have two .fasta files
  and their feature tables (.tbl) in pythonPkg you can test
  out.

# Thanks

- To my dad how continues to be a source of encouragement
  and support.
- To Eric Bortz (one of my mentors) for mentioning that
  this might be a good idea to code up. And also provided
  insight into how to determine if a flu was high path or
  low path.
- NCBI genbank for having a resource were I could grab a
  couple example flu genomes from.
- Who knows how many answered questions on stack overflow
  or web tutorials people have posted up. I never asked
  the questions, but I benefit from other people asking.
  Thanks to all the people who took time to answer a
  question, ask a question, or post a tutorial.

# Funding

- Will add in later.
