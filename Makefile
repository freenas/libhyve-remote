PACKAGE=	lib${LIB}
LIB=		hyverem
SHLIB_MAJOR=	1
SHLIB_MINOR=	0
WARNS?=		2
CFLAGS+=	-I${.CURDIR}/includes
SRCS=		libcheck.c vncserver.c

.if defined(HAVE_DEBUG)
CFLAGS+=	-g
CFLAGS+=	-DHAVE_DEBUG
.endif

distclean: clean
	rm -f libhyverem.so.1

examples:
CFLAGS+=	-L${.CURDIR} -lhyverem
SUBDIR=		examples

.include <bsd.lib.mk>
