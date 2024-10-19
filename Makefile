# $Id: Makefile,v 1.5 2005/07/07 14:02:39 andrew Exp $

PROG=	settitle
SRCS=	${PROG}.c ${PROG}.h
LDADD=
MAN1=	${PROG}.1

PREFIX?=	/usr/local
BINDIR=	${PREFIX}/bin
MANDIR=	${PREFIX}/share/man/man

HAVE_PROGNAME=	true

.if defined(HAVE_PROGNAME)
CFLAGS+= -DHAVE_PROGNAME
.else
SRCS+=	progname.c progname.h
.endif

.if defined(DEBUG)
WARNS=7
WFORMAT=7
DEBUG_FLAGS+=-O0 -g
.endif

CFLAGS+= -I${PREFIX}/include -L${PREFIX}/lib

.include <bsd.prog.mk>
