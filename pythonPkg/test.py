from checkFluHiOrLowPath import hiOrLowPathHA

# Set up initial (hard coding) variables
lowPathFileStr = "MT406777.fasta";
highPathFileStr = "LC316691.fasta";

# Starting position of the low path H2A gene
lowPathHA2PosUL = 1064;#index 0; 1st base is 0 instead of 1

# Feature table to use for the high path gene
highPathTblStr = "LC316691.tbl";

# Will hold the sequence
lowPathSeqStr = "";
highPathSeqStr = "";

# Open the files
lowPathFILE = open(lowPathFileStr);
highPathFILE = open(highPathFileStr);

# Get the headers
lowPathHeadStr = lowPathFILE.readline();
lowPathHeadStr = lowPathHeadStr.replace("\n", "");
lowPathHeadStr = lowPathHeadStr.replace(">", "");

highPathHeadStr = highPathFILE.readline();
highPathHeadStr = highPathHeadStr.replace("\n", "");
highPathHeadStr = highPathHeadStr.replace(">", "");

# Get the sequences (only one sequence per file)
lowPathSeqStr = lowPathFILE.readlines();
lowPathSeqStr = ''.join(lowPathSeqStr).replace("\n", "");

highPathSeqStr = highPathFILE.readlines();
highPathSeqStr = ''.join(highPathSeqStr).replace("\n", "");

# Print out the header for the output
print(
   "{:11s}".format("Header"),
   "Hi/low",
   "phe/try",
   "P1_to_P6",
   sep = "\t"
); # Print out the header

# Run program on the low path example
retAry = hiOrLowPathHA(
      seq = lowPathSeqStr,
      HA2Start = lowPathHA2PosUL
); # Run the low path sequence

print(
   f"{lowPathHeadStr:11s}",
   f"{retAry[0]}".format(True, False),
   f"{retAry[1]}".format(True, False),
   f"{retAry[2]:6s}",
   sep = "\t"
);

# Run program on the high path example
retAry = hiOrLowPathHA(
      seq = highPathSeqStr,
      tbl = highPathTblStr
); # Fun the high path result

print(
   f"{highPathHeadStr:11s}",
   f"{retAry[0]}".format(True, False),
   f"{retAry[1]}".format(True, False),
   f"{retAry[2]:6s}",
   sep = "\t"
); # Print out the high path results
