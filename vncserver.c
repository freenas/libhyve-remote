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
#include <string.h>
#include "vncserver.h"
#include "libcheck.h"
#include "rfbsrv.h"

static int load_functions(void);

static int
load_functions(void) {
    char *loader = NULL;
    void *shlib;

    if (check_sharedlibs(LIBVNCSERVER) == 0) {
        rfbScreenInfoPtr (*get_screen)(int *argc, char **argv,
            int width, int height, int bitsPerSample, int samplesPerPixel,
            int bytesPerPixel);
        void (*start_vnc_server)(rfbScreenInfoPtr rfbScreen);
        void (*run_event_loop)(rfbScreenInfoPtr screeninfo, long usec,
            rfbBool runInBackground); 

        loader = strdup(LIBVNCSERVER);
        if (loader == NULL)
            err(1, "malloc");

        shlib = dlopen(loader, RTLD_NOW | RTLD_GLOBAL);
        if (!shlib) {
            dlclose(loader);
            free(loader);
            return (1);
        }

        get_screen = dlsym(shlib, "rfbGetScreen");
        start_vnc_server = dlsym(shlib, "rfbInitServerWithoutPthreadsAndZRLE");
        run_event_loop = dlsym(shlib, "rfbRunEventLoop");

        return (0);
    } else
        return (1);
}

int
init_server(struct server_handle *sc) {
    if (load_functions() == 0)
        sc->vs_screen = (struct _rfbScreenInfo *)malloc(sizeof(rfbScreenInfoPtr));

    return (0);
}
