dnl Process this file with autoconf to produce a configure script.

AC_INIT(README)
AC_CANONICAL_TARGET([]) 

dnl Setup for automake
AM_INIT_AUTOMAKE(scorched3d, 39)

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

dnl Checking for SDL
AC_MSG_CHECKING(for SDL)
SDL_VERSION=1.2.5
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found. Try http://www.libsdl.org/])
)

CFLAGS="$CFLAGS $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl checking for SDL_net
AC_MSG_CHECKING(for SDL_net lib)
have_SDLnet=yes
if test "x$enable_sdltest" = "xyes" ; then
	AC_TRY_COMPILE([
		#include <SDL/SDL.h>
		#include <SDL/SDL_net.h>

		int main(int argc, char *argv[])
		{ return 0; }
#undef  main
#define main K_and_R_C_main
		],[
		],[
		have_SDLnet=yes
		],[
		have_SDLnet=no
		])
fi

AC_MSG_RESULT($have_SDLnet)
if test x$have_SDLnet != xyes; then
    AC_MSG_ERROR([*** Can't find the SDL_net library Try: http://www.libsdl.org/projects/SDL_net])
fi
LIBS="$LIBS -lSDL_net"

AC_CHECK_LIB(fridge, beer, echo "Hmm?!",[
	echo "Warning: No beer found in fridge!";
	echo "We highly suggest that you rectify this situation immediately."
])

dnl Checks for OpenGL
AC_ARG_ENABLE(opengltest, 
	[  --disable-opengltest    Do not try to compile and run a test OpenGL program],
		    , enable_opengltest=yes)
AC_MSG_CHECKING(for OpenGL support)
have_opengl=yes
if test "x$enable_opengltest" = "xyes" ; then
	AC_TRY_COMPILE([
		#include <GL/gl.h>
		#include <GL/glu.h>
		],[
		],[
		have_opengl=yes
		],[
		have_opengl=no
	])
fi

AC_MSG_RESULT($have_opengl)
if test x$have_opengl != xyes; then
    AC_MSG_ERROR([*** Can't find the OpenGL library Try: http://www.opengl.org])
fi

if test `uname -s` != FreeBSD; then 
    OPENGL_LIBS="-lGL -lGLU -lpthread" 
else 
    OPENGL_LIBS="-lGL -lGLU" 
fi 
LIBS="$LIBS $OPENGL_LIBS"

dnl Checking for OpenAL
AC_ARG_WITH([openal-static],
            AC_HELP_STRING([--with-openal-static],
                           [enable static linking for openal (default no)]),
            [use_static_openal=${withval}],,)
AC_ARG_ENABLE(openaltest, 
	[  --disable-openaltest    Do not try to compile and run a test OpenAL program],
		    , enable_openaltest=yes)
AC_MSG_CHECKING(for OpenAL)
AC_PATH_PROG(OPENAL_CONFIG, openal-config, no)
have_openal=yes
if test x$OPENAL_CONFIG = xno; then
	echo "*** The openal-config script installed by OpenAL could not be found"
	echo "*** Make sure openal-config is in your path, or set the OPENAL_CONFIG"
	echo "*** environment variable to the full path to openal-config."
	have_openal=no
else

	if test x"$use_static_openal" = x"yes"; then
		LIBS="$LIBS /usr/local/lib/libopenal.a"
	else
		LIBS="$LIBS `$OPENAL_CONFIG --libs`"
	fi
	
	CFLAGS="$CFLAGS `$OPENAL_CONFIG --cflags`"
	CXXFLAGS="$CXXFLAGS `$OPENAL_CONFIG --cflags`"
fi

AC_MSG_CHECKING(for OpenAL compilation)
if test "x$enable_altest" = "xyes" ; then
	AC_TRY_COMPILE([
		#include <AL/al.h>
		#include <AL/alut.h>
		#include <AL/alc.h>
		],[
		],[
		have_openal=yes
		],[
		have_openal=no
	])
fi

AC_MSG_RESULT($have_openal)
if test x$have_openal != xyes; then
    AC_MSG_ERROR([*** Can't find the openal library. Try: http://www.openal.org/])
fi

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

dnl Floating point math
AC_MSG_CHECKING(for floating point math support)
have_fp=yes
AC_TRY_COMPILE([
	#include <math.h>
	],[
	float a = sinf(0.1f);
	],[
	have_fp=yes
	],[
	have_fp=no
])
AC_MSG_RESULT($have_fp)

if test x$have_fp != xyes; then
	CFLAGS="$CPPFLAGS -DNO_FLOAT_MATH"
	CPPFLAGS="$CPPFLAGS -DNO_FLOAT_MATH"
	CXXFLAGS="$CXXFLAGS -DNO_FLOAT_MATH"
fi

dnl Checks for mysql
AC_ARG_WITH([mysql],
            AC_HELP_STRING([--with-mysql],
                           [enable mysql support (default no)]),
            [use_mysql=${withval}],,)
AH_TEMPLATE(HAVE_MYSQL,[Define if you have libmysql])

if test x"$use_mysql" = x"yes"; then
	AC_MSG_CHECKING(for MySql support)
	have_mysql=no
	AC_TRY_COMPILE([
		#include <mysql/mysql.h>
		],[
		],[
		have_mysql=yes
	])
	AC_MSG_RESULT($have_mysql)

	if test x$have_mysql != xyes; then
		AC_MSG_ERROR([*** Can't find the MySql library Try: http://www.mysql.com])
	else
		AC_DEFINE(HAVE_MYSQL)
		MYSQLLIBS=`mysql_config --libs`
		LIBS="$LIBS $MYSQLLIBS"
	fi
fi

dnl Checks for pgsql
AC_ARG_WITH([pgsql],
            AC_HELP_STRING([--with-pgsql],
                           [enable pgsql support (default no)]),
            [use_pgsql=${withval}],,)
AH_TEMPLATE(HAVE_PGSQL,[Define if you have libpq])

if test x"$use_pgsql" = x"yes"; then
	AC_MSG_CHECKING(for PGSql support)
	have_pgsql=no
	AC_TRY_COMPILE([
		#include <pgsql/libpq-fe.h>
		],[
		],[
		have_pgsql=yes
	])
	AC_MSG_RESULT($have_pgsql)

	if test x$have_pgsql != xyes; then
		AC_MSG_ERROR([*** Can't find the PGSql library Try: http://www.postgresql.org])
	else
		AC_DEFINE(HAVE_PGSQL)
		LIBS="$LIBS -lpq"
	fi
fi

dnl Checks for static linking for wx windows
AC_ARG_WITH([wx-static],
            AC_HELP_STRING([--with-wx-static],
                           [enable static linking for wxwindows (default no)]),
            [use_static_wx=${withval}],,)

dnl Check for wxwindows
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.4.0, wxWin=1)
if test "$wxWin" != 1; then
        AC_MSG_ERROR([
                wxWindows must be installed on your system
                but the wx-config script couldn't be found.
                Please check that wx-config is in path, the directory
                where wxWindows libraries are installed (returned by
                'wx-config --libs' command) is in LD_LIBRARY_PATH or
                equivalent variable and wxWindows version is 2.4.0 or above
		Try http://www.wxwindows.org to download wxwindows.
        ])
fi
     
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
if test x"$use_static_wx" = x"yes"; then
	LIBS="$LIBS $WX_LIBS_STATIC"
else
	LIBS="$LIBS $WX_LIBS"
fi

dnl Check for FreeType2
AC_CHECK_FT2(7.0.1, ftype=1)
if test "$ftype" != 1; then
	AC_MSG_ERROR([
		FreeType2 must be installed on your system
		but freetype-config couldn't be found.
		Please check that freetype-config is in the path, the directory
		where the freetype libraries are installed (returned by
		'freetype-config --libs' command) is in LD_LIBRARY_PATH or
                equivalent variable and freetype version is 7.0.1 or above
                Try http://www.freetype.org to download freetype.
	])
fi

CFLAGS="$CFLAGS $FT2_CFLAGS"
CXXFLAGS="$CXXFLAGS $FT2_CFLAGS"
LIBS="$LIBS $FT2_LIBS"

# Checl for extra functions
AC_CHECK_FUNCS(vsnprintf snprintf)

# Directory setup stuff
dnl --with-docdir
AC_ARG_WITH(docdir,
	AC_HELP_STRING([--with-docdir=DIR],
	[Use DIR to store documentation files (default DATADIR)]),
	[with_docdir=$withval])
if test "x$with_docdir" = "x" ; then
	docdir='${datadir}'
else
	docdir=$with_docdir
fi
AC_SUBST(docdir)

AC_PREFIX_DEFAULT(/usr/local/games/scorched3d)
CFLAGS="$CFLAGS -DS3D_DOCDIR=\\\"${docdir}\\\" -DS3D_DATADIR=\\\"${datadir}\\\" -DS3D_BINDIR=\\\"${bindir}\\\""
CXXFLAGS="$CXXFLAGS -DS3D_DOCDIR=\\\"${docdir}\\\" -DS3D_DATADIR=\\\"${datadir}\\\" -DS3D_BINDIR=\\\"${bindir}\\\""

# extra fixed flags
CFLAGS="$CFLAGS -DdDOUBLE"
CXXFLAGS="$CXXFLAGS -DdDOUBLE"

# put here at end because of sdl.m4 not defining
AC_LANG_CPLUSPLUS

# Finally create all the generated files
AC_OUTPUT([
Makefile
src/Makefile
src/scorched/Makefile
])
