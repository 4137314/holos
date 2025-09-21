AC_INIT([holos], [0.1], [holos@mail.com])
AC_CONFIG_SRCDIR([src/astro/celestial_mechanics.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Controllo compilatore C
AC_PROG_CC

# Controllo libreria GSL
PKG_CHECK_MODULES([GSL], [gsl], [], [AC_MSG_ERROR([GSL library not found])])

# Genera Makefile
AC_CONFIG_FILES([Makefile])
AC_OUTPUT