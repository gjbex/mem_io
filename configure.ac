AC_INIT([mem_io], [1.4], [geertjan.bex@uhasselt.be])
AM_INIT_AUTOMAKE([-Wall -Werror foreign tar-pax])
AC_PROG_CC([gcc])
AC_CHECK_HEADERS([err.h errno.h float.h libgen.h limits.h math.h \
                  stdbool.h stdio.h stdlib.h string.h sys/stat.h \
                  sys/types.h unistd.h])
AC_CHECK_HEADERS([hiredis/hiredis.h], [hiredis_hdr_found=yes])
AC_SEARCH_LIBS([redisCommand], [hiredis], [hiredis_lib_found=yes],
               [hiredis_lib_found=no])
AC_SEARCH_LIBS([log10], [m], [],
               [AC_MSG_ERROR([unable to find log10 function])])
PKG_CHECK_MODULES([hiredis], [hiredis])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT
