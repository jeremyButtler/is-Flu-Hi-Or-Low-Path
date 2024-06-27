from setuptools import setup
from setuptools import Extension

descriptionStr="\
Checks the P1 to P6 before the HA1/HA2 cleavege site to\n\
see if the HA gene is from a hi or low path flu. See the\n\
docstring for the isHiOrLowPathHA function for details.\n\
"

srcFilesStr = [
   "pyIsHiLowPathFlu.c"
]

compileFlags = [
   "-O0",
   "-g",
   "-static",
   "--std=c99"
]

setup(
   name = "checkFluHiOrLowPath",
   version = "20231101",
   description = descriptionStr,
   author = "https://github.com/jeremybuttler/hiOrLowPathFlu",
   ext_modules=[
      Extension(
         "checkFluHiOrLowPath",
         srcFilesStr,
         extra_compile_args = compileFlags
   )]
); # setup function
