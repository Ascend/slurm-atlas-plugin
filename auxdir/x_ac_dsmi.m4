##*****************************************************************************
##  AUTHOR:
##    Yinping Ma <mayinping@pku.edu.cn>, Chun Fan <fanchun@pku.edu.cn>
##
##  SYNOPSIS:
##    X_AC_NVML
##
##  DESCRIPTION:
##    Determine if Ascend's DSMI API library exists
###*****************************************************************************
#
#
AC_DEFUN([X_AC_DSMI],
[
  AC_ARG_WITH(
    [dsmi],
    AS_HELP_STRING(--without-dsmi, Do not build NVIDIA DSMI-related code),
    []
  )

  if [test "x$with_dsmi" = xno]; then
     AC_MSG_WARN([support for dsmi disabled])
  else
    # -I/usr/local/Ascend/driver/kernel/inc/driver is the main location. Others are just in case
    dsmi_includes="-I/usr/local/Ascend/driver/kernel/inc/driver "
    # Check for DSMI header and library in the default locations
    AC_MSG_RESULT([])
    cppflags_save="$CPPFLAGS"
    CPPFLAGS="$dsmi_includes $CPPFLAGS"
    AC_CHECK_HEADER([dsmi_common_interface.h], [ac_dsmi_h=yes], [ac_dsmi_h=no])
    CPPFLAGS="$cppflags_save"
    if test "$ac_dsmi_h" = "yes"; then
      DSMI_LIBS="-L/usr/local/Ascend/driver/lib64/driver/ -ldrvdsmi_host"
      DSMI_CPPFLAGS="$dsmi_includes"
      AC_DEFINE(HAVE_DSMI, 1, [Define to 1 if DSMI library found])
    else
      AC_MSG_WARN([unable to locate libdrvdsmi_host.so and/or dsmi_common_interface.h])
    fi
    AC_SUBST(DSMI_LIBS)
    AC_SUBST(DSMI_CPPFLAGS)
  fi
  AM_CONDITIONAL(BUILD_DSMI, test "$ac_dsmi_h" = "yes")
])
