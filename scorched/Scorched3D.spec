Name: scorched3d
Version: 37
Release: 2
License: GPL
Group: Amusements/Games
Summary: A 3D version of the classic DOS game Scorched Earth
URL: http://www.scorched3d.co.uk
Source0: http://unc.dl.sourceforge.net/sourceforge/scorched3d/Scorched3D-%{version}-src.tar.gz
BuildRoot: %{_tmppath}/%{name}-buildroot

BuildRequires:  perl, automake, autoconf, SDL-devel, wxGTK-devel
BuildRequires:  SDL_mixer-devel, SDL_net-devel, freetype-devel
Requires: SDL, freetype, wxGTK, SDL_mixer, SDL_net

%description
Scorched 3D is a game based loosely on the classic DOS game Scorched
Earth "The Mother Of All Games".

%prep
%setup -q -n scorched

%build
# sh ./autogen.sh
export CC=gcc3
export CXX=g++3
%configure --datadir=%{_datadir}/scorched3d \
	--program-prefix=%{?_program_prefix:%{_program_prefix}}
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall datadir=$RPM_BUILD_ROOT%{_datadir}/scorched3d
rm -rf $RPM_BUILD_ROOT%{_datadir}/scorched3d/documentation
#install -d %{buildroot}%{_bindir}

#cat > %{buildroot}%{_bindir}/scorched3d <<EOF
#!/bin/sh
#pushd %{_libdir}/games/scorched3d > /dev/null
#./scorched3d $*
#popd > /dev/null
#EOF
#chmod 755 %{buildroot}%{_bindir}/scorched3d

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(664,root,games,755)
%doc documentation/*
%dir %{_datadir}/scorched3d
%{_datadir}/scorched3d/data
%attr(755,root,root) %{_bindir}/scorched3d

%changelog
* Sun Dec 7 2003 Gavin Camp gcamp@scorched3d.co.uk
- First build.

