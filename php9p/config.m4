dnl $Id$
dnl config.m4 for extension ixp

PHP_ARG_ENABLE(ixp, whether to enable 9p support,
[  --enable-ixp           Enable 9p support])

if test "$PHP_IXP" != "no"; then
  AC_DEFINE(HAVE_IXP, 1, [Whether you have IXP])
  
  libixp_sources="libixp/src/client.c \
    libixp/src/convert.c libixp/src/error.c \
	libixp/src/intmap.c libixp/src/message.c \
    libixp/src/request.c libixp/src/rpc.c \
    libixp/src/server.c libixp/src/socket.c \
    libixp/src/thread.c libixp/src/transport.c \
	libixp/src/util.c"
 
  PHP_IXP_CFLAGS="-I@ext_builddir@/libixp -I@ext_builddir@/libixp/src"

  PHP_NEW_EXTENSION(ixp, ixp.c $libixp_sources, $ext_shared,,$PHP_IXP_CFLAGS)
  dnl PHP_INSTALL_HEADERS([$ext_builddir/libixp/ixp.h])
  PHP_ADD_BUILD_DIR($ext_builddir/libixp/src, 1)
fi
