# Installation Instructions
```
/******************************************************************************
Copyright 2022 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/
```
# Requirements
- GNU C++ compiler 4.9 or higher OR Intel Compilers 2018 or higher
- FFTW libraries (for dynamic library build, the fftw3.so lib is needed)
- GSL libraries

# Optional requirements
- MPI compilers

If not in the path, the variable FFTW_HOME and GSL_HOME must be defined.

# Basic installation of MBX
Installation is as simple as running

```
autoreconf -fi
./configure
make && make install
```

# MPI compilation (For LAMMPS use only)
MPI compilation is needed when using MBX with LAMMPS
If the MPI compilers and libraries are not in the default location,
a variable MPI_HOME needs to be defined.

```
autoreconf -fi
./configure --enable-mpi CXX=mpiicpc
make && make install
```

# Unittesting
It is recommended to first install the code without MPI and optimizations and run 
the unittests. To do so:
```
./configure --disable-optimization CXX=icpc
make check
```
All tests should pass.
