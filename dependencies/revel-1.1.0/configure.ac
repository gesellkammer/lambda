AC_INIT(src/revelcore.cpp)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(revel,1.1.0)
AM_CONFIG_HEADER(config.h)
AC_PROG_CC
AC_LANG_C

jb_PREFIX_EXPAND([includedir])
jb_PREFIX_EXPAND([libdir])

test "$jb_expanded_includedir" != /usr/include &&
  test -d $jb_expanded_includedir &&
    CFLAGS="-I$jb_expanded_includedir $CFLAGS"

test -d $jb_expanded_libdir && LIBS="-L$jb_expanded_libdir $LIBS"

CPPFLAGS=$CFLAGS

AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

dnl Check for XviD
dnl -------------
XVID_VERSION=1.0.1
AM_PATH_XVID($XVID_VERSION,:,
   AC_MSG_ERROR([*** XviD version $XVID_VERSION not found!]))

dnl Consolidate all the myriad paths
AM_CFLAGS="$AM_CFLAGS $XVID_CFLAGS"
AC_SUBST(AM_CFLAGS)

AM_LDFLAGS="$AM_LDFLAGS $XVID_LIBS"
AC_SUBST(AM_LDFLAGS)

AC_PROG_RANLIB
AC_PROG_INSTALL
AC_CONFIG_FILES(Makefile m4/Makefile src/Makefile)
AC_OUTPUT
