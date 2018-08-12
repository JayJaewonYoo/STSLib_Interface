# STSLib_Interface
Software created using C++ designed to interface with the functionality in the STSLib library developed by Chuan Ma.

Software created using C++ designed to interface with the functionality in the STSLib library developed by Chuan Ma. STSLib is a powerful library capable of computing large-scale discrete event systems but much of the functionality is difficult to use and some of the functionality is simply inaccessible. Furthermore, the documentation is sparse at best. Developing the Interfacing Software involved understanding STSLib and creating new classes that not only access the functions that are normally inaccessible, but also implement new functionality such as individual state isolation that enables greater analysis using STSLib. The most notable function of the interfacing software is that it enables the user to apply an event to a single state or entire system and the software will compute the resultant state or system. The software also recognizes which events are illegal and cannot be applied given a certain state of the system. The software is planned to be used for academic purposes in graduate level courses related to discrete event systems. This software was developed by Jay Jaewon Yoo as part of Professor Walter Murray Wonham’s research team. 

STSLib can be found here: https://github.com/chuanma/STSLib

This is currently a work in progress. 
