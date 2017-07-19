#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include "rfbsrv.h"
#include "hyverem.h"

#include "vncserver.h"

#define LIBHYVEREM  "/usr/local/lib/libhyverem.so"

int main(void);

int
main(void) {
    // DLOPEN
    char *loader;
    void *shlib;

    struct server_softc *sc = malloc(sizeof(struct server_softc));
    sc->bind_port = 5900;
    sc->desktopName = "bhyve";
    sc->alwaysShared = true;
    sc->vs_width = 800;
    sc->vs_height = 600;
    sc->frameBuffer = (char *)malloc(sc->vs_width*sc->vs_height*4);

    vnc_init_server(sc);

    return (0);
}
