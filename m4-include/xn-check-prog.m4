# xn-check-prog.m4 - use program given in env var, otherwise detect program
# serial 1
dnl
AC_DEFUN([XN_CHECK_PROG], [dnl
AC_ARG_VAR([$1], [$2][ ][$3])dnl
AS_VAR_IF([$1], [], [dnl
AC_PATH_PROG([$1], [$2], [no])dnl
])
AM_CONDITIONAL([HAVE_PROG_][$1], [test "x$$1" != xno])
])dnl
dnl
dnl Local Variables:
dnl mode: autoconf
dnl End:
