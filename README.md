Template Model Builder (TMB)
============================

[![Build Status](https://travis-ci.org/kaskr/adcomp.svg?branch=master)](https://travis-ci.org/kaskr/adcomp)

TMB is an R package with functionality similar to ADMB.
It requires R at least version 3.0.0 and development tools needed to install R packages from source.
It is recommended to install TMB into your local R package library (if you do not yet have a local R package library, create one by running ```install.packages("")``` from the R prompt and follow the instructions).
The package is installed from the command line by entering the adcomp folder and typing

```shell
make install
```

To build the user manual type

```shell
make pdf
```

Once the package is successfully installed it can be tested by starting R and running

```R
library(TMB)
runExample(all=TRUE)
```

To build API-Function reference see instructions [here](dox/README.md).
Once the documentation is built open dox/html/index.html in your web browser.
Use the search field to find functions and their documentation.

It is recommended to test that models can be changed, re-compiled and re-loaded without problems:

```R
setwd("tmb_syntax")
source("test_reload.R")
```

If everything works as intended this script should display "TRUE".
The script has only been observed to fail for certain combinations of Linux and the gcc compiler, see below.

Alternative platforms
=====================

Windows
-------
Tested to work on 32/64 bit R with latest [Rtools](http://cran.r-project.org/bin/windows/Rtools/). 64 bit R is recommended.

_Install instructions_

1. Start 64 bit R and change working directory to the (cloned or unzipped) ```adcomp``` folder.

2. From R run: ```source("install_windows.R")```

The required Rtools will be downloaded and installed. Note that the PATH variable need not be changed by the installer or otherwise edited. The PATH will be automatically set for each TMB session.

_Status_

- Parallel user templates work. However, changing the number of threads from R does not work.
- Filenames and folders with spaces works _on systems supporting 8.3 filenames_. Systems that do not support 8.3 filenames may have problems running the windows install script, unless R is installed in a folder without spaces.
- -Wall flag disabled by default.

Mac OS X
--------
Tested to work with both llvm-gcc-4.2 and clang. Fortran compiler [libraries](http://cran.r-project.org/bin/macosx/tools) must be installed. According to [R admin manual](http://www.cran.r-project.org/doc/manuals/R-admin.html#OS-X) "the OpenMP support in this version of gcc is problematic, and the alternative, clang, has no OpenMP support". So, parallel templates with OS X will require a different compiler installed.

On Mavericks (Mac OS 10.9) the Fortran compiler on the CRAN website does not work. More information and the solution can be found [here](http://www.thecoatlessprofessor.com/programming/rcpp-rcpparmadillo-and-os-x-mavericks-lgfortran-and-lquadmath-error). The easiest solution is to install the appropriate fortran libraries from r.research.att via the command line in Terminal:

```
curl -O http://r.research.att.com/libs/gfortran-4.8.2-darwin13.tar.bz2
sudo tar fvxz gfortran-4.8.2-darwin13.tar.bz2 -C /
```

Linux specific notes
====================

Metis orderings
---------------
For large 3D random field models the ordering algorithms shipping with R's Matrix package are far from optimal. To get better orderings available run the following in the terminal:

* sudo apt-get install libsuitesparse-metis-3.1.0

This will install a more complete version of CHOLMOD with more orderings available. Then install TMB like this:

* make install-metis

For a quick example of how to use it start R, load TMB and run

* runExample("ar1xar1")

Issues with library unloading
-----------------------------
On recent versions of gcc the following problem may be encountered: When the user cpp file is changed, re-compiled and re-loaded, the changes do not take place. To see if you are affected by this issue, assuming your compiled DLL is called "mymodel.so", try running:

* readelf -s mymodel.so | grep UNIQUE

If this gives any output it is not possible to unload the library, and R will have to be restarted every time the model is re-compiled.
There are at least two alternative solutions to this problem:

1. Use gcc with compilation flag ```-fno-gnu-unique``` (version 4.8.3 and newer): Add ```CXX = g++ -fno-gnu-unique``` to a file ```~/.R/Makevars``` (create it if it doesn't exist).

2. Use the clang compiler instead of gcc: Install clang and add ```CXX = clang++``` to a file ```~/.R/Makevars``` (create it if it doesn't exist).

Note: If you have precompiled TMB using ```precompile()``` prior to (1) or (2) you must repeat the precompilation with the new compiler settings.
