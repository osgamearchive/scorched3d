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
    GL_LIBS="-lGL -lGLU -lpthread" 
else 
    GL_LIBS="-lGL -lGLU" 
fi 

AC_SUBST(GL_LIBS)

