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

*20240627*: Updated the waterman aligner and changed some
  of the coding style. The complied program name is
  shorter (getHaPath). The python library support has not
  been updated and so is no longer present.
  See 2024-03-21-code for the old version.

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

## Mac/building non-static

```
make mac
sudo make install
```

## Building static

```
make
sudo make install
```

# Using getHaPath:

## Using the standalone

This program has three options, but only one of the three
  options is needed. The only required option is the fasta
  file (-fa) to get the HA sequence from. This program
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
getHaPath -fa HA.fasta;

# or

getHaPath -fa HA.fasta -tbl HA-feature-table.tbl;

# or

getHaPath -fa HA.fasta -HA2Start 1026;
```

## Using this code in C

There are two main .h files in this code you will needed
  to know about. The first is haPath.c/h, which
  contains the functions to get the amino acids and
  determines if a result matches a pattern. The second is
  haStart.h, which is used to get the HA2 starting
  position from a feature table and to find the HA sequence
  in a fasta file.

All the other files are dependencies these two .h files
  depend on. Or in the case of some files in generalLib,
  stuff I was to lazy to delete (copied from getDIids).

### haStart.c/h

haStart.c/h is set up to find the starting position of
  the HA2 ORF in the sequence. You can use find_haStart
  (fun03 haStart.c/h) to find the starting position using
  a Waterman Smith alignment.

- Input:
  - seqStruct with sequence to get get HA2 postion
    - free with freeHeap_seqST(); fun09 memwater/seqST.c/h
    - read in fasta sequence with getFqSeq_seqST()
      (fun03 memwater/seqST.c/h)
      - Input: file and seqStruct
      - returns 0 for success
      - returns 1 (def_EOF_seqST) for EOF
      - other values are errors
  - alnSet structure with the alignment settings
  - variable to hold start of HA2 ORF in sequence
  - variable to hold first HA2 mapped base in sequence

### haPath.c/h

haPath.c/h converts the HA2 P1 to P6 positions into amino
  acids with getP1_P6AA_haPath (fun03).

- Input:
  - seqStruct with the HA sequence and position
  - array to hold P1 to P6 positions (length is 7)
  - first base (index 0) in HA2
    - use find_haStart (fun02 haStart.c/h) to get this

- Output:
  - adds P1 to P6 aa sequences to input P1 to P6 array

You can then use the P1 to P6 array and find_haPath (fun02
  haPath.h) to detect hi path HA sequences.

- Input:
  - P1 to P6 array used with getP1_P6AA_haPath

- Output:
  - returns 1 if was a high path sequence

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
