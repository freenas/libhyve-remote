# libhyverem #

The libhyverem is intended to abstract functionalities from other third part libraries such like libvncserver and freerdp protocol implementation.
The main purpose is to act as a proxy between those libraries and be agnostic of the protocol implementation.

Also due some software license restrictions, this library is implemented using dual license GPL version 2 and BSD 2-Clauses license.

### What is this repository for? ###

This is a POC to create an abstraction layer from third part libraries such like libvncserver and freerdp for a more license friendly purpose library.
The intend of this library is to be agnostic of protocol implementation and act as a proxy between itself and other third part libraries implementation.

### How do I get set up? ###

* cmake ./CMakeLists.txt 
* make

It will create 3 different files:
1) test_it that basically is a POC to test the shared library itself.
2) main is test purpose binary that does nothing. :)
3) libhyverem that implements the abstraction layer between other libraries.
