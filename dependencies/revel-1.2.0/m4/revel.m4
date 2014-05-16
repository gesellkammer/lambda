dnl AM_PATH_REVEL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for Revel, and define REVEL_CFLAGS, REVEL_LIBS and REVEL_PREFIX.
dnl
AC_DEFUN([AM_PATH_REVEL],
[dnl 

  AC_ARG_WITH(revel-prefix,[  --with-revel-prefix=PFX Prefix where Revel is installed (optional)],
            REVEL_PREFIX="$withval", REVEL_PREFIX="")
  AC_ARG_ENABLE(reveltest, [  --disable-reveltest       Do not try to compile and run a test Revel program],
		    , enable_reveltest=yes)


  if test "x$REVEL_PREFIX" = x ; then
    REVEL_LIBS="-lrevel -lxvidcore"
    REVEL_CFLAGS=""
  else
    REVEL_LIBS="-L${REVEL_PREFIX}/lib -lrevel -lxvidcore"
    REVEL_CFLAGS="-I${REVEL_PREFIX}/include"
  fi

  min_revel_version=ifelse([$1], ,1.0.0,$1)
  AC_MSG_CHECKING(for Revel - version >= $min_revel_version)
  no_revel=""


  if test "x$enable_reveltest" = "xyes" ; then
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $REVEL_CFLAGS"
    LIBS="$LIBS $REVEL_LIBS"
dnl
dnl Now check if the installed Revel is sufficiently new.
dnl
    rm -f conf.reveltest
    AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <revel.h>

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
  system ("touch conf.reveltest");
  */
  { FILE *fp = fopen("conf.reveltest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_revel_version");
  if (sscanf(tmp_version, "%d.%d.%d", &min_major, &min_minor, &min_micro) != 3) {
     printf("%s, bad version string\n", "$min_revel_version");
     exit(1);
   }

  actual_micro = REVEL_VERSION & 0x0000FF;
  actual_minor = REVEL_VERSION & 0x00FF00;
  actual_major = REVEL_VERSION & 0xFF0000;

    /* Quickie functionality test. */
    {
    int encHand;
    Revel_Error revErr;
    Revel_Params revParams;

    revErr = Revel_CreateEncoder(&encHand);
    if (revErr != REVEL_ERR_NONE)
      {
	printf("Revel Test Error = %d!\n", revErr);
	return -1;
      }
    revParams.width = revParams.height = 100;
    revParams.quality = 1.0f;
    revParams.codec = REVEL_CD_XVID;
    revParams.frameRate = 29.97f;
    revErr = Revel_EncodeStart(encHand, "reveltest.avi", &revParams);
    if (revErr != REVEL_ERR_NONE)
      {
	printf("Revel Test Error = %d!\n", revErr);
	return -1;
      }
    revErr = Revel_EncodeEnd(encHand);
    if (revErr != REVEL_ERR_NONE)
      {
	printf("Revel Test Error = %d!\n", revErr);
	return -1;
      }
    Revel_DestroyEncoder(encHand);
    }

   if ((actual_major > min_major) ||
      ((actual_major == min_major) && (actual_minor > min_minor)) ||
      ((actual_major == min_major) && (actual_minor == min_minor) && (actual_micro >= min_micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** Found Revel version %d.%d.%d, but the minimum version\n",
	 actual_major, actual_minor, actual_micro);
      printf("*** of Revel required is %d.%d.%d. Please upgrade to a more\n",
	 min_major, min_minor, min_micro);
      printf("*** recent version of Revel, or use the --with-revel-prefix\n");
      printf("*** option to specify the path to a more recent version.\n");
      return 1;
    }
}

],, no_revel=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
     CFLAGS="$ac_save_CFLAGS"
     LIBS="$ac_save_LIBS"
   fi


  if test "x$no_revel" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
    AC_MSG_RESULT(no)
    if test -f conf.reveltest ; then
     :
    else
       echo "*** Could not run Revel test program, checking why..."
       CFLAGS="$CFLAGS $REVEL_CFLAGS"
       LIBS="$LIBS $REVEL_LIBS"
       AC_TRY_LINK([
#include <stdio.h>
#include <revel.h>

int main(int argc, char *argv[])
{ return 0; }
#undef  main
#define main K_and_R_C_main
],      [ return 0; ],
      [ echo "*** The test program compiled, but did not run. This usually means"
        echo "*** that the run-time linker is not finding Revel or finding the wrong"
        echo "*** version of Revel. If it is not finding Revel, you'll need to set your"
        echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
        echo "*** to the installed location  Also, make sure you have run ldconfig if that"
        echo "*** is required on your system"
        echo "***"
        echo "*** If you have an old version installed, it is best to remove it, although"
        echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
      [ echo "*** The test program failed to compile or link. See the file config.log for the"
        echo "*** exact error that occured. This usually means Revel was incorrectly installed"
        echo "*** or that you have moved Revel since it was installed." ])
        CFLAGS="$ac_save_CFLAGS"
        LIBS="$ac_save_LIBS"
    fi
    REVEL_CFLAGS=""
    REVEL_LIBS=""
    ifelse([$3], , :, [$3])
  fi

AC_SUBST(REVEL_PREFIX)
AC_SUBST(REVEL_LIBS)
AC_SUBST(REVEL_CFLAGS)
rm -f conf.reveltest
rm -f reveltest.avi
])