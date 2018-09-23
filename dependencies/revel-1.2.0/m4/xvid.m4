dnl AM_PATH_XVID([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for XviD, and define XVID_CFLAGS, XVID_LIBS and XVID_PREFIX.
dnl
AC_DEFUN([AM_PATH_XVID],
[dnl 

  AC_ARG_WITH(xvid-prefix,[  --with-xvid-prefix=PFX Prefix where XviD is installed (optional)],
            XVID_PREFIX="$withval", XVID_PREFIX="")
  AC_ARG_ENABLE(xvidtest, [  --disable-xvidtest       Do not try to compile and run a test XviD program],
		    , enable_xvidtest=yes)


  if test "x$XVID_PREFIX" = x ; then
    XVID_LIBS="-lxvidcore"
    XVID_CFLAGS=""
  else
    XVID_LIBS="-L${XVID_PREFIX}/lib -lxvidcore"
    XVID_CFLAGS="-I${XVID_PREFIX}/include"
  fi

  min_xvid_version=ifelse([$1], ,1.0.0,$1)
  AC_MSG_CHECKING(for XviD - version >= $min_xvid_version)
  no_xvid=""


  if test "x$enable_xvidtest" = "xyes" ; then
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $XVID_CFLAGS"
    LIBS="$LIBS $XVID_LIBS"
dnl
dnl Now check if the installed XviD is sufficiently new.
dnl
    rm -f conf.xvidtest
    AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xvid.h>

char* my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int min_major, min_minor, min_micro;
  int actual_major, actual_minor, actual_micro;
  char *tmp_version;

  /* This hangs on some systems (?)
  system ("touch conf.xvidtest");
  */
  { FILE *fp = fopen("conf.xvidtest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_xvid_version");
  if (sscanf(tmp_version, "%d.%d.%d", &min_major, &min_minor, &min_micro) != 3) {
     printf("%s, bad version string\n", "$min_xvid_version");
     exit(1);
   }

  actual_micro = XVID_VERSION_PATCH(XVID_VERSION);
  actual_minor = XVID_VERSION_MINOR(XVID_VERSION);
  actual_major = XVID_VERSION_MAJOR(XVID_VERSION);
  

   if ((actual_major > min_major) ||
      ((actual_major == min_major) && (actual_minor > min_minor)) ||
      ((actual_major == min_major) && (actual_minor == min_minor) && (actual_micro >= min_micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** Found XviD version %d.%d.%d, but the minimum version\n",
	 actual_major, actual_minor, actual_micro);
      printf("*** of XviD required is %d.%d.%d. Please upgrade to a more\n",
	 min_major, min_minor, min_micro);
      printf("*** recent version of XviD, or use the --with-xvid-prefix\n");
      printf("*** option to specify the path to a more recent version.\n");
      return 1;
    }
}

],, no_xvid=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
     CFLAGS="$ac_save_CFLAGS"
     LIBS="$ac_save_LIBS"
   fi


  if test "x$no_xvid" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
    AC_MSG_RESULT(no)
    if test -f conf.xvidtest ; then
     :
    else
       echo "*** Could not run XviD test program, checking why..."
       CFLAGS="$CFLAGS $XVID_CFLAGS"
       LIBS="$LIBS $XVID_LIBS"
       AC_TRY_LINK([
#include <stdio.h>
#include <xvid.h>

int main(int argc, char *argv[])
{ return 0; }
#undef  main
#define main K_and_R_C_main
],      [ return 0; ],
      [ echo "*** The test program compiled, but did not run. This usually means"
        echo "*** that the run-time linker is not finding XviD or finding the wrong"
        echo "*** version of XviD. If it is not finding XviD, you'll need to set your"
        echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
        echo "*** to the installed location  Also, make sure you have run ldconfig if that"
        echo "*** is required on your system"
        echo "***"
        echo "*** If you have an old version installed, it is best to remove it, although"
        echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
      [ echo "*** The test program failed to compile or link. See the file config.log for the"
        echo "*** exact error that occured. This usually means XviD was incorrectly installed"
        echo "*** or that you have moved XviD since it was installed." ])
        CFLAGS="$ac_save_CFLAGS"
        LIBS="$ac_save_LIBS"
    fi
    XVID_CFLAGS=""
    XVID_LIBS=""
    ifelse([$3], , :, [$3])
  fi

AC_SUBST(XVID_PREFIX)
AC_SUBST(XVID_LIBS)
AC_SUBST(XVID_CFLAGS)
rm -f conf.xvidtest
])