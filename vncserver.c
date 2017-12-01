/*
 * Copyright (c) 2017 by Marcelo Araujo <araujo@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification, immediately at the beginning of the file.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * the GNU Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <sysexits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "vncserver.h"
#include "libcheck.h"

#define DPRINTF(params) if (hyverem_debug) printf params
#define WPRINTF(params) printf params

static int hyverem_debug = 0;
static char *keys[0x400];
struct vncserver_handler *srv = NULL;
struct vnc_http_proxy *hp = NULL;
pthread_cond_t wait_cond = NULL;
static char *vnc_password = NULL;

/* Shared functions from libvncserver. */
rfbScreenInfoPtr (*get_screen)(int *argc, char **argv,
    int width, int height, int bitsPerSample, int samplesPerPixel,
    int bytesPerPixel);
void (*start_vnc_server)(rfbScreenInfoPtr rfbScreen);
void (*run_event_loop)(rfbScreenInfoPtr screeninfo, long usec,
    rfbBool runInBackground);
void (*mark_rect_asmodified)(rfbScreenInfoPtr rfbScreen, int x1, int y1,
    int x2, int y2);
void (*vnc_password_check)(rfbClientPtr cl, const char *response, int len);

/*
 * Just output the IP of the client connected in this server and return accept.
 */
static enum rfbNewClientAction
vncserver_newclient(rfbClientPtr cl) {
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    unsigned int ipv4;

    getpeername(cl->sock, (struct sockaddr *)&addr, &len);
    ipv4 = ntohl(addr.sin_addr.s_addr);
    DPRINTF(("Client connected from ip %d.%d.%d.%d\n",
            (ipv4>>24)&0xff, (ipv4>>16)&0xff, (ipv4>>8)&0xff, ipv4&0xff));

    if (wait_cond)
        pthread_cond_signal(&wait_cond);

    return (RFB_CLIENT_ACCEPT);
}

/*
 * Dummy fallback keyboard interface in case the consumer doesn't provide it.
 */
static void
dokey_fallback(rfbBool down, rfbKeySym key) {
    WPRINTF(("[hyverem]: %s: %s (0x%x)",
              down ? "down" : "up", keys[key&0x3ff] ? keys[key&0x3ff] : "", (unsigned int)key));
}

/*
 * Dummy fallback for mouse interface in case the consumer doesn't provide it.
 */
static void
doptr_fallback(int button, int x, int y) {
    if (button) {
        WPRINTF(("[hyverem]: mouse button mask 0x%x at %d, %d", button,
                  x, y));
    }
}

/*
 * Enable HTTP proxy server.
 */
int
vnc_enable_http(char *webdir, bool enable) {
    struct stat sb;

    hp = malloc(sizeof(struct vnc_http_proxy));
    if (!hp)
        errx(EX_OSERR, "error to allocate memory for http");

    if (stat(webdir, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        hp->webdir = webdir;
        hp->enable = enable;
    } else
        err(EX_UNAVAILABLE, "webdir: %s does not exist", webdir);

    DPRINTF(("WEB: %s ENB: %d\n", hp->webdir, hp->enable));
    return (0);
}

/*
 * Update the client screen with new information.
 */
int
vnc_mark_rect_modified(struct server_softc *sc, int x1, int y1, int x2, int y2) {

    mark_rect_asmodified(srv->vs_screen, 0, 0, sc->vs_width, sc->vs_height);
    DPRINTF(("x1: %d\t y1: %d\t x2: %d\t y2: %d\n", x1, y1, x2, y2));

    return (0);
}

int
vnc_enable_password(char *password) {
    if (password) {
        vnc_password = password;
        return 0;
    } else
        return 1;
}

/*
 * Load from LIBVNCSERVER shared library the functions that are necessary
 * to provide a minimum functionality for this library.
 */
static int
load_functions(void) {
    char *loader = NULL;
    void *shlib;

    if (check_sharedlibs(LIBVNCSERVER) == 0) {
        loader = strdup(LIBVNCSERVER);
        if (loader == NULL)
            errx(EX_SOFTWARE, "not possible to load %s", LIBVNCSERVER);

        shlib = dlopen(loader, RTLD_NOW | RTLD_GLOBAL);
        if (!shlib) {
            goto free_lib;
        }

        get_screen = dlsym(shlib, "rfbGetScreen");
        start_vnc_server = dlsym(shlib, "rfbInitServerWithPthreadsAndZRLE");
        run_event_loop = dlsym(shlib, "rfbRunEventLoop");
        mark_rect_asmodified = dlsym(shlib, "rfbMarkRectAsModified");
        vnc_password_check = dlsym(shlib, "rfbCheckPasswordByList");

        if(!start_vnc_server) {
            WPRINTF(("[hyverem]: Failed to load rfbInitServerWithPthreadsAndZRLE\n"));
            goto free_lib;
        } else if (!run_event_loop) {
            WPRINTF(("[hyverem]: Failed to load rfbRunEventLoop\n"));
            goto free_lib;
        } else if (!mark_rect_asmodified) {
            WPRINTF(("[hyverem]: Failed to load rfbMarkRectAsModified\n"));
            goto free_lib;
        } else if (!vnc_password_check) {
            WPRINTF(("[hyverem]: Failed to load rfbCheckPasswordByList\n"));
            goto free_lib;
        }
        return (0);
    }
    return (1);
free_lib:
    dlclose(loader);
    free(loader);
    return (1);
}

/*
 * Launch VNC server with a loop.
 */
void vnc_event_loop(int time, bool bol) {
        DPRINTF(("start vnc_event_loop\n"));
        if (srv->vs_screen)
            run_event_loop(srv->vs_screen, time, bol);
        else
            errx(EX_SOFTWARE, "there is no data to show for the client");
}

/*
 * Initialize the VNC SERVER with data received from server_softc structure.
 */
int
vnc_init_server(struct server_softc *sc) {
    static const char *passwordList[0];
    passwordList[0] = NULL;

    if (load_functions() == 0) {
        srv = malloc(sizeof(struct vncserver_handler));
        srv->vs_screen = (struct _rfbScreenInfo *)malloc(sizeof(rfbScreenInfoPtr));
        if (!srv->vs_screen) {
            free(srv);
            errx(EX_OSERR, "error to allocate memory for vs_screen");
        }

        if (sc) {
            srv->vs_screen = (*get_screen)(NULL, NULL, sc->vs_width,
                sc->vs_height, 8, 3, 4);
            srv->vs_screen->desktopName = sc->desktopName;
            srv->vs_screen->alwaysShared = sc->alwaysShared;
            srv->vs_screen->serverFormat.redShift = sc->redShift;
            srv->vs_screen->serverFormat.greenShift = sc->greenShift;
            srv->vs_screen->serverFormat.blueShift = sc->blueShift;
            srv->vs_screen->frameBuffer = sc->frameBuffer;
            srv->vs_screen->screenData = sc;
            srv->vs_screen->port = sc->bind_port;

            if (sc->hostname && strlen(sc->hostname) > 0)
                srv->vs_screen->listenInterface = inet_addr(sc->hostname);
            else
                srv->vs_screen->listenInterface = htonl(INADDR_LOOPBACK);

            srv->vs_screen->ipv6port = sc->bind_port;
            srv->vs_screen->newClientHook = vncserver_newclient;
        } else
            errx(EX_DATAERR, "there is no data to initialize the server");

        if (sc->kbd_handler && sc->ptr_handler) {
            srv->vs_screen->kbdAddEvent = (void *)sc->kbd_handler;
            srv->vs_screen->ptrAddEvent = (void *)sc->ptr_handler;
        } else {
            WPRINTF(("[hyverem]: Keyboard and mouse functions not provided.\n"));
            srv->vs_screen->kbdAddEvent = (void *)dokey_fallback;
            srv->vs_screen->ptrAddEvent = (void *)doptr_fallback;
        }

        if (vnc_password) {
            passwordList[0] = vnc_password;
            srv->vs_screen->authPasswdData = (void *)passwordList;
            srv->vs_screen->passwordCheck = (void *)vnc_password_check;
        }

        if (hp) {
            srv->vs_screen->httpDir = hp->webdir;
            srv->vs_screen->httpEnableProxyConnect = hp->enable;
        }

        if (sc->vs_conn_wait == 1) {
            wait_cond = sc->vs_cond;
        }

        DPRINTF(("Bind port: %d for guest: %s\n", sc->bind_port, sc->desktopName));
        start_vnc_server(srv->vs_screen);

        return (0);
    }
    return (1);
}
