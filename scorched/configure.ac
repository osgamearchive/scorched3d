dnl Process this file with autoconf to produce a configure script.

AC_INIT(README)
AC_CANONICAL_TARGET([]) 

dnl Setup for automake
AM_INIT_AUTOMAKE(scorched3d, 41)

dnl Check for tools
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

LDFLAGS="-L/usr/X11R6/lib -L/usr/X11R6 -L/usr/local/lib"
CPPFLAGS="-I/usr/X11R6/include -I/usr/X11R6 -I/usr/local/include"

if test `uname` == Darwin; then
LDFLAGS="$LDFLAGS -lmx"
CPPFLAGS="$CPPFLAGS -D__DARWIN__"
fi

AC_CHECK_LIB(fridge, beer, echo "Hmm?!",[
	echo "Warning: No beer found in fridge!";
	echo "We highly suggest that you rectify this situation immediately."
])

AC_ARG_ENABLE(serveronly, 
	[  --enable-serveronly Enable server only compilation],
		    , enable_serveronly=no,
		    enable_serveronly=yes)

# Include the other configuration files
if test "x$enable_serveronly" = "xno" ; then
m4_include([configure-gl.m4])
m4_include([configure-al.m4])
m4_include([configure-wx.m4])
m4_include([configure-ogg.m4])
m4_include([configure-ft.m4])
S3D_SUBDIRS="scorched scorchedc scorcheds"
else
S3D_SUBDIRS="scorcheds"
fi
m4_include([configure-sdl.m4])
m4_include([configure-flt.m4])
m4_include([configure-sql.m4])

AC_SUBST(S3D_SUBDIRS)

dnl Checks for Profiling
AC_MSG_CHECKING(for profiling enabled)
AC_ARG_ENABLE(profiling, 
	[  --enable-profiling    Enable gprof code profiling],
		    , enable_profiling=no,
		    enable_profiling=yes)
if test "x$enable_profiling" = "xyes" ; then
	LIBS="$LIBS -pg"
	CFLAGS="$CFLAGS -pg"
	CPPFLAGS="$CPPFLAGS -pg"
	CXXFLAGS="$CXXFLAGS -pg"
fi
AC_MSG_RESULT($enable_profiling)

# FIXEME
dnl Checking for Zlib Png
LIBS="$LIBS -lz -lpng"

# Check for extra functions
AC_CHECK_FUNCS(vsnprintf snprintf vasprintf asprintf)

# Check for extra headers
AC_CHECK_HEADERS(unistd.h)

# Directory setup stuff
dnl --with-docdir
AC_ARG_WITH(docdir,
	AC_HELP_STRING([--with-docdir=DIR],
	[Use DIR to store documentation files (default DATADIR)]),
	[with_docdir=$withval])
if test "x$with_docdir" = "x" ; then
	docdir='${datadir}/documentation'
else
	docdir=$with_docdir
fi
AC_SUBST(docdir)

AC_PREFIX_DEFAULT(/usr/local/games/scorched3d)
CFLAGS="$CFLAGS -DS3D_DOCDIR=\\\"${docdir}\\\" -DS3D_DATADIR=\\\"${datadir}\\\" -DS3D_BINDIR=\\\"${bindir}\\\""
CXXFLAGS="$CXXFLAGS -DS3D_DOCDIR=\\\"${docdir}\\\" -DS3D_DATADIR=\\\"${datadir}\\\" -DS3D_BINDIR=\\\"${bindir}\\\""

# put here at end because of sdl.m4 not defining
AC_LANG_CPLUSPLUS

# Finally create all the generated files
AC_OUTPUT([
Makefile
src/Makefile
src/scorched/Makefile
src/scorchedc/Makefile
src/scorcheds/Makefile
])
