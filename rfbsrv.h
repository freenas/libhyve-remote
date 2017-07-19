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

#include <rfb/rfb.h>

#ifndef GC_H
#define GC_H
struct gc_image {
    int     vgamode;
    int     width;
    int     height;
    uint32_t    *data;
};
#endif

#ifndef HANDLER_H
#define HANDLER_H
struct vncserver_handler {
    struct gc_image *vs_gc;
    pthread_t       vs_tid;
    rfbScreenInfoPtr vs_screen;
    int             vs_width;
    int             vs_height;
    int             vs_conn_wait;
    int             vs_sending;
    pthread_mutex_t vs_mtx;
    pthread_cond_t  vs_conf;
    int             vs_hw_crc;
    uint32_t        vs_crc;
    uint32_t        vs_crc_tmp;
    int             vs_crc_width;
    int             vs_crc_height;
};
#endif
