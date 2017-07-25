#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include "hyverem.h"
#include "vncserver.h"

static void mouse(uint8_t button, int x, int y);
static void keyboard(int down, uint32_t key);

static void
mouse(uint8_t button, int x, int y) {
    if (button) {
        printf("Mouse button 0x%x at position %d, %d\n", button,
                x, y);
    }
}

static void
keyboard(int down, uint32_t key) {
    if (down)
        printf("Key pressed: %d\n", down);
}

int
main(void) {
    struct server_softc *sc = malloc(sizeof(struct server_softc));
    sc->bind_port = 5900;
    sc->desktopName = "Test";
    sc->alwaysShared = true;
    sc->vs_width = 800;
    sc->vs_height = 600;
    sc->frameBuffer = (char *)malloc(sc->vs_width * sc->vs_height * 4);
    sc->redShift = 5;
    sc->greenShift = 5;
    sc->blueShift = 5;
    sc->ptr_handler = mouse;
    sc->kbd_handler = keyboard;

    vnc_enable_http("/tmp/webclients/", true);
    vnc_init_server(sc);
    vnc_event_loop(-1, FALSE);

    return (0);
}
