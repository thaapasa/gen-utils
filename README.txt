General Utils
=============================================================================
This is a library of useful general utilities for Java and C++, written
and collected by Tuukka Haapasalo.

Contents:
-----------------------------------------------------------------------------
1. Directory layout
2. General Utils library compilation
3. Linking your own project with the library
-----------------------------------------------------------------------------



1. Directory layout
=============================================================================
This project uses the source directory layout suggested by the Maven build 
tool for Java, that is:   

- The sources can be found at src/main/java (for Java) and src/main/cpp 
  (for C++).
- Test files are under src/test/java and src/test/cpp, respectively.

Compiled files do not yet follow the Maven standard, but rather:

- Compiled Java classes go to bin
- Compiled C++ object files go to the same directory as the C++ source 
  files

In addition, the C++ include files are located in the "include" directory.
 
 
 
2. General Utils library compilation
=============================================================================

Java
-----------------------------------------------------------------------------
There is an ant build script for building the Java classes. It should work...
The build file is located in build/build.xml, so you must use 
"ant -f build/build.xml" to build the Java classes.
The default target "dist" builds the distribution package (a Java archive) 
under the "dist" directory. 

(I always compile the classes using Eclipse, and then run the "dist" target
from Eclipse's Ant view. That at least works...). 

There is a property template file build.properties.tmpl in the build 
directory, but you do not need to worry about that. It is for my personal 
use, I use it to copy the created JAR file directly to other projects that 
use the general utils library. 

C++
-----------------------------------------------------------------------------
There is a Makefile that should compile the C++ utils library in both Windows
and Linux, although I have not done extensive testing with different build 
environments. Recent Linuxes with gcc-4.4+ and recent MinGW installations in
Windows (as of spring 2011) should work, although you might have to tweak
something. 

Upon successful compilation, the library file libutil.a is created in the
"dist" directory. I use static linking because this is still so small a
library.



3. Linking your own project with the library
=============================================================================

Java
-----------------------------------------------------------------------------
Just copy the created general-utils.jar somewhere and add it to your 
own program's class path, then add normal Java imports. That's it.

C++ 
-----------------------------------------------------------------------------
Add the "include" directory of the general utils library in your build
parameters; for example

> g++ -O3 -Wall -Wno-deprecated mysource.cpp -o mysource.o  \
       -I/path/to/genutils/include 

(The general utils library uses the hashmap extension and the no-deprecated 
 switch suppresses warnings for using the hashmap extension.)

Then, include the necessary utility files:

#include "util/Array.hpp"
#include "util/Matrix.hpp"

Always use the path that has the util-prefix, DO NOT add include/util to
your include path; otherwise the includes in the general utils library do 
not work.

For linking, add the utils-library using -lutil and either copy the created
libutil.a file to your directory or add the "dist" directory to your linking
path:

> g++ mysource.o -L/path/to/genutils/dist -lutil

In Windows, you may have to add "-Wl,--enable-auto-import" to your linking
flags to tell the linker to enable auto importing. This suppresses the 
warning messages the compiler otherwise gives. 
