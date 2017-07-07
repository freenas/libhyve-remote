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

#include <string.h>
#include <sysexits.h>
#include <dlfcn.h>
#include <errno.h>
#include <err.h>
#include "rfbsrv.h"
#include "libcheck.h"

static int check_supportedlibs(char *lib_name);

/*
 * Check if the library is one of those supported.
 */
static int
check_supportedlibs(char *lib_name) {
    if (strncmp(lib_name, LIBFREERDP, sizeof(LIBFREERDP)) == 0 ||
            strncmp(lib_name, LIBVNCSERVER, sizeof(LIBVNCSERVER)) == 0)
        return 0;
    else
        err(EX_IOERR, "%s this is a non supported lib.", lib_name);
}

/*
 * Check if the library is present in the system trying to open as a shared
 * library.
 */
int
check_sharedlibs(char *lib_name) {
    char *loader;
    void *shlib;

    loader = NULL;

    if (check_supportedlibs(lib_name) == 0) {
        loader = strdup(lib_name);
        if (loader == NULL)
            err(EX_OSERR, "malloc error");

        shlib = dlopen(loader, RTLD_LAZY);
        if (!shlib) {
            dlclose(shlib);
            free(loader);
            return (1);
        } else
            return (0);
    }

    return (1);
}
