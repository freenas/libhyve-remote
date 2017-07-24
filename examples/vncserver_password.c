#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hyverem.h"
#include "vncserver.h"

int main(void);

int
main(void) {
    struct server_softc *sc = NULL;

    sc = malloc(sizeof(struct server_softc));
    sc->bind_port = 5959;
    sc->desktopName = "Test_Password";
    sc->password = "secret";

    vnc_init_server(sc);
    vnc_event_loop(-1, FALSE);

    return (0);
}
