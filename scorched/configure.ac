dnl Process this file with autoconf to produce a configure script.

AC_INIT(README)
AC_CANONICAL_TARGET([]) 

dnl Setup for automake
AM_INIT_AUTOMAKE(scorched3d, 0.35.1)

dnl Check for tools
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Check for SDL
SDL_VERSION=1.2.5
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found. Try http://www.libsdl.org/])
)

CFLAGS="$CFLAGS $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl Check for SDL_mixer lib
have_SDLmixer=no
have_opengl=no  
AC_TRY_COMPILE([
   	#include <SDL/SDL.h>
	#include <SDL/SDL_mixer.h>
],[
],[
have_SDLmixer=yes
])

if test x$have_SDLmixer != xyes; then
    AC_MSG_ERROR([*** Can't find the SDL_mixer library. Try: http://www.libsdl.org/projects/SDL_mixer])
fi
LIBS="$LIBS -lSDL_mixer"

dnl Check for SDL_net lib
have_SDLnet=no
have_opengl=no  
AC_TRY_COMPILE([
   	#include <SDL/SDL.h>
	#include <SDL/SDL_net.h>
],[
],[
have_SDLnet=yes
])

if test x$have_SDLnet != xyes; then
    AC_MSG_ERROR([*** Can't find the SDL_net library Try: http://www.libsdl.org/projects/SDL_net])
fi
LIBS="$LIBS -lSDL_net"

dnl Checks for OpenGL
AC_MSG_CHECKING(for OpenGL support)
have_opengl=no  
AC_TRY_COMPILE([
   	#include <GL/gl.h>
	#include <GL/glu.h>
	
],[
],[
have_opengl=yes
])
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

dnl Check for wxwindows
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.4.0, wxWin=1)
if test "$wxWin" != 1; then
        AC_MSG_ERROR([
                wxWindows must be installed on your system
                but wx-config script couldn't be found.
                Please check that wx-config is in path, the directory
                where wxWindows libraries are installed (returned by
                'wx-config --libs' command) is in LD_LIBRARY_PATH or
                equivalent variable and wxWindows version is 2.4.0 or above
		Try http://www.wxwindows.org
        ])
fi
     
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

dnl Define install prefix 
if test x$prefix = xNONE; then
	prefix=/usr/local/games
fi
AC_DEFINE_UNQUOTED(PKGDIR, "$prefix/$PACKAGE/", [install directory])

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
