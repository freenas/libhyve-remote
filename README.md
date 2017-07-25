# libhyve-remote

The libhyve-remote aims to abstract functionalities from other third party libraries such like libvncserver, freerdp and spice. With a basic data structure it is easy to implement any remote desktop protocol without dig into the protocol specification or third part libraries, you can check some of our examples.

We don't static link any third party library, instead we use dynamic linker and we load only functionalities that we believe is necessary to launch the service.

Our target is to abstract functionalities from libvncserver, freerdp and spice. Right now libhyve-remote only supports libvncserver. It is possible to launch a vnc server with different screen resolution as well as with authentication.

Also, due some software license restrictions, this library is implemented using dual license GPL version 2 and BSD 2-Clauses license.

### How do I get set up?

    make
    make install 

It will create and install libhyve-remote in `/usr/lib/`

### How do I use libhyve-remote?

    cd examples

### What libhyve-remote provides now?

Basically 3 main functions:
* vnc_enable_http(): That enables libvncserver to run via HTTP.
* vnc_init_server(): That prepares the vnc server to be started.
* vnc_event_loop(): Runs the vnc server.
