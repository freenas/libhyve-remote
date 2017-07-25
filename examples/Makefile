PACKAGE=        examples
WARNS?=		2
LIBADD+=	${.CURDIR}/../libhyverem.so.1
CFLAGS+=	-I${.CURDIR}/../includes -L${.CURDIR}/../ -lhyverem

.if defined(HAVE_DEBUG)
CFLAGS+=	-g
CFLAGS+=	-DHAVE_DEBUG
.endif

clean:
	rm -f simple_output simple_output_http vncserver_password

all: simple_output simple_output_http \
	vncserver_password

simple_output: simple_output.c
	$(CC) $(CFLAGS) simple_output.c -o simple_output

simple_output_http: simple_output_http.c
	$(CC) $(CFLAGS) simple_output_http.c -o simple_output_http

vncserver_password: vncserver_password.c
	$(CC) $(CFLAGS) vncserver_password.c -o vncserver_password


distclean: clean
	-rm -f libhyverem.so

.include <bsd.lib.mk>
