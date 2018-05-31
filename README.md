[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/freenas/libhyve-remote.svg)](http://isitmaintained.com/project/freenas/libhyve-remote "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/freenas/libhyve-remote.svg)](http://isitmaintained.com/project/freenas/libhyve-remote "Percentage of issues still open")
[![GitHub forks](https://img.shields.io/github/forks/freenas/libhyve-remote.svg)](https://github.com/freenas/libhyve-remote/network)


# libhyve-remote <img src="https://raw.githubusercontent.com/araujobsd/hbhyve/master/logo/hbhyve.jpg" alt="libhyve-remote" height="82" width="82" align="right">

The libhyve-remote aims to abstract functionalities from other third party libraries such like libvncserver, freerdp and spice to be used in hypervisor implementation. With a basic data structure it is easy to implement any remote desktop protocol without dig into the protocol specification or third part libraries, you can check some of our examples.

We don't static link any third party library, instead we use dynamic linker and we load only functionalities that we believe is necessary to launch the service.

Our target is to abstract functionalities from libvncserver, freerdp and spice. Right now libhyve-remote only supports libvncserver. It is possible to launch a vnc server with different screen resolution as well as with authentication.

Also, due some software license restrictions, this library is implemented using dual license GPL version 2 and BSD 2-Clauses license.

### How do I get set up?

    make
    make install 

It will create and install `libhyverem.so` in `/usr/local/lib/`

### How do I use libhyve-remote?

    cd examples

### What libhyve-remote provides now?

Basically 5 main functions:
#### libvncserver ####
* vnc_enable_http(): That enables libvncserver to run via HTTP.
* vnc_enable_password(): Will enable vnc server password.
* vnc_init_server(): That prepares the vnc server to be started.
* vnc_event_loop(): Runs the vnc server.
* vnc_mark_rect_modified(): Tells to vncserver there is a modification on the screen.

### Imortant note. ### 
The libhyve-remote is a new project still under development, it is still in process of polishment and under constrution.
