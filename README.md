# libhyverem #

The libhyverem is intended to abstract functionalities from other third part libraries such like libvncserver and freerdp protocol implementation.
The main purpose is to act as a proxy between those libraries and be agnostic of the protocol implementation.

Also due some software license restrictions, this library is implemented using dual license GPL version 2 and BSD 2-Clauses license.

### What is this repository for? ###

This is a POC to create an abstraction layer from third part libraries such like libvncserver and freerdp for a more license friendly purpose library.
The intend of this library is to be agnostic of protocol implementation and act as a proxy between itself and other third part libraries implementation.

### How do I get set up? ###

* cmake ./CMakeLists.txt 
* make ; make install

It will create and install libhyverem.so at /usr/local/lib/

### How do I use libhyverem? ###
* cd examples

### What libhyverem provides now? ###
Basically 3 main functions:
* vnc_enable_http(): That enables libvncserver to run via HTTP.
* vnc_init_server(): That prepares the vnc server to be started.
* vnc_event_loop(): Runs the vnc server.
