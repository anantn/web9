dnl $Id$
dnl config.m4 for extension 9p

PHP_ARG_ENABLE(9p, whether to enable 9p support,
[  --enable-9p           Enable 9p support])

if test "$PHP_9P" != "no"; then
  AC_DEFINE(HAVE_9P, 1, [Whether you have 9P])
  
  libixp_sources="libixp/src/client.c \
    libixp/src/convert.c libixp/src/intmap.c \
    libixp/src/message.c libixp/src/request.c \
    libixp/src/server.c libixp/src/socket.c \
    libixp/src/transport.c libixp/src/util.c"
 
  PHP_9P_CFLAGS="-I@ext_builddir@/libixp -I@ext_builddir@/libixp/src"

  PHP_NEW_EXTENSION(9p, 9p.c $libixp_sources, $ext_shared,,$PHP_9P_CFLAGS)
  PHP_INSTALL_HEADERS([$ext_builddir/libixp/ixp.h])
  PHP_ADD_BUILD_DIR($ext_builddir/libixp/src, 1)
fi
