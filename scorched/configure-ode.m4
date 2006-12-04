dnl Checking for ODE
AC_MSG_CHECKING(for ODE library)
AC_PATH_PROG(ODE_CONFIG, ode-config, no)
if test x$ODE_CONFIG = xno; then
	echo "*** The ode-config script installed by ODE could not be found"
	echo "*** Make sure ode-config is in your path, or set the ODE_CONFIG"
	echo "*** environment variable to the full path to ode-config."

	AC_MSG_ERROR([*** Can't find the openal library. Try: installing from http://www.ode.org/])
else

	ODE_LIBS="`$ODE_CONFIG --libs`"
	ODE_CFLAGS="`$ODE_CONFIG --cflags`"

	AC_MSG_RESULT(yes)
fi

AC_SUBST(ODE_LIBS)
AC_SUBST(ODE_CFLAGS)
