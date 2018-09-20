# STSLib_Interface
Software created using C++ designed to interface with the functionality in the STSLib library developed by Chuan Ma.

This software was developed by Jay Jaewon Yoo as part of his work with Professor Walter Murray Wonham's research team. This is designed to interface with the functionality in the STSLib library developed by Chuan Ma. STSLib is a powerful library capable of computing large-scale discrete event systems but much of the functionality is difficult to use and some of the functionality is simply inaccessible. Furthermore, the documentation is sparse at best. Developing the Interfacing Software involved understanding STSLib and creating new classes that not only access the functions that are normally inaccessible, but also implement new functionality such as individual state isolation that enables greater analysis using STSLib. The most notable function of the interfacing software is that it enables the user to apply an event to a single state or entire system and the software will compute the resultant state or system. The software also recognizes which events are illegal and cannot be applied given a certain state of the system. The software is planned to be used for academic purposes in graduate level courses related to discrete event systems.

STSLib can be found here: https://github.com/chuanma/STSLib

This is currently a work in progress. 

# Installation
## Linux
1. Install STSLib found at https://github.com/chuanma/STSLib.
2. Install CMake.
3. Move all the files in STSLib/STSLib from the library linked in step 1 to the intended working directory.
4. Download all the files from this repository and move them all to the same working directory as the other moved files. 
5. Create a directory called build within the working directory. 
6. Move into build and open Terminal from the directory.
7. Type "cmake .." into the Terminal.
8. Type "make" into the Terminal.

## Windows or macOS
To use the software in a non Unix-like operating system, please install the corresponding host binary of VirtualBox (found https://www.virtualbox.org/), prepare it for Linux and then follow the Linux installations.
- A Windows full port may be developed if there is enough time. 

# How to Use
