## Process with automake to produce Makefile.in

pkgdir = $(prefix)/@PACKAGE@

SUBDIRS = src

OTH_SUBDIRS = 				\
		data			\
		documentation		\
		scripts			\
		setup			\
		visualc			\
		generic			\
		borland

WIN_DIST =				\
		SDL.dll			\
		SDL_mixer.dll		\
		SDL_net.dll		\
		MSVCP70.DLL		\
		MSVCR70.dll		\
		scorched.sln		

UNIX_DIST =				\
		autogen.sh		\
		COMPILING


EXTRA_DIST = $(WIN_DIST) $(UNIX_DIST) $(OTH_SUBDIRS) 

install-exec-local:
	$(INSTALL) -d -m 0755 $(pkgdir)
	cp -rp $(srcdir)/src/scorched/scorched3d $(pkgdir)

install-data-local:
	$(INSTALL) -d -m 0755 $(pkgdir)/data 
	cp -rp $(srcdir)/data $(pkgdir)
	cp -rp $(srcdir)/README $(pkgdir)
	cp -rp $(srcdir)/documentation $(pkgdir)

uninstall-local:
#	rm -rf $(pkgdir)/*

dist-hook:
	cp -rp $(srcdir)/data $(distdir)

