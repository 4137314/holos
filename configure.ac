########################################################################
# configure.ac for the HOLOS project - GNU Autotools configuration
# This file follows GNU standards and is processed by autoconf.
# Each macro and section is commented for clarity and portability.
########################################################################

# Initialize the package: name, version, contact email
AC_INIT([holos], [0.1], [holos@mail.com])

# Check for a source file to ensure we are in the correct directory
AC_CONFIG_SRCDIR([src/astro/celestial_mechanics.c])

# Generate a config.h header with configuration macros
AC_CONFIG_HEADERS([config.h])

# Initialize automake with options
# 'foreign' allows a less strict structure than GNU standard
# 'subdir-objects' allows object files (.o) in subdirectories
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Check for a C compiler
AC_PROG_CC

# Check for the GSL library (GNU Scientific Library)
# PKG_CHECK_MODULES sets GSL_CFLAGS and GSL_LIBS
PKG_CHECK_MODULES([GSL], [gsl], [], [AC_MSG_ERROR([GSL library not found])])

# Check for the readline library (for CLI input)
# AC_CHECK_LIB sets READLINE_LIBS if found
AC_CHECK_LIB([readline], [readline], [READLINE_LIBS="-lreadline"], [AC_MSG_ERROR([readline library not found])])
AC_SUBST([READLINE_LIBS])

# Check for the GNUnet library (for P2P features)
# PKG_CHECK_MODULES sets GNUNET_CFLAGS and GNUNET_LIBS
PKG_CHECK_MODULES([GNUNET], [gnunetutil])

# Generate the Makefile from Makefile.am/Makefile.in
AC_CONFIG_FILES([Makefile])
AC_OUTPUT