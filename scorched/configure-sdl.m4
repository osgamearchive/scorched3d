dnl Checking for SDL
AC_MSG_CHECKING(for SDL)
SDL_VERSION=1.2.5
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found. Try http://www.libsdl.org/])
)

dnl checking for SDL_net
AC_MSG_CHECKING(for SDL_net lib)
have_SDLnet=yes
if test "x$enable_sdltest" = "xyes" ; then

	ac_save_CFLAGS="$CFLAGS"
	ac_save_LIBS="$LIBS"
	CFLAGS="$CFLAGS $SDL_CFLAGS"
	LIBS="$SDL_LIBS $LIBS"

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

	CFLAGS="$ac_save_CFLAGS"
	LIBS="$ac_save_LIBS"
fi

AC_MSG_RESULT($have_SDLnet)
if test x$have_SDLnet != xyes; then
    AC_MSG_ERROR([*** Can't find the SDL_net library Try: http://www.libsdl.org/projects/SDL_net])
fi
SDL_LIBS="$SDL_LIBS -lSDL_net"
