PACKAGE=	lib${LIB}
LIB=		hyverem
SHLIB_MAJOR=	1
SHLIB_MINOR=	0
WARNS?=		2
CFLAGS+=	-I${.CURDIR}/include -I/usr/local/include
SRCS=		libcheck.c vncserver.c
PREFIX?=	/usr/local
LIBDIR=		${PREFIX}/lib

.if defined(HAVE_DEBUG)
CFLAGS+=	-g
CFLAGS+=	-DHAVE_DEBUG
.endif

distclean: clean
	rm -f libhyverem.so.1

examples:
CFLAGS+=	-L${.CURDIR} -lhyverem
SUBDIR=		examples

bhyve-patch:
	mkdir -p /tmp/bhyve-remote/usr.sbin/
	ln -s /usr/src/share /tmp/bhyve-remote/
	cp -R /usr/src/usr.sbin/bhyve /tmp/bhyve-remote/usr.sbin/
	cp /usr/src/usr.sbin/Makefile* /tmp/bhyve-remote/usr.sbin/
	cp patches/bhyve_vncserver.patch /tmp/bhyve-remote/usr.sbin/bhyve/
	cd /tmp/bhyve-remote/usr.sbin/bhyve/ ; patch < bhyve_vncserver.patch ; make clean; make; make install

.include <bsd.lib.mk>
