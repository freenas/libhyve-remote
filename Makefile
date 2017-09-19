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
	cp patches/bhyve_vncserver.patch /usr/src/usr.sbin/bhyve/
	cd /usr/src/usr.sbin/bhyve/ ;  patch < bhyve_vncserver.patch ; make clean; make; make install

.include <bsd.lib.mk>
