Name: scorched3d
Version: 37.1
Release: 1
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
%configure --datadir=%{_datadir}/scorched3d \
	--program-prefix=%{?_program_prefix:%{_program_prefix}} --with-wx-static
make -j2 %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall datadir=$RPM_BUILD_ROOT%{_datadir}/scorched3d

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(664,root,games,755)
%doc documentation/*
%dir %{_datadir}/scorched3d
%{_datadir}/scorched3d/data
%{_datadir}/scorched3d/documentation
%attr(755,root,root) %{_bindir}/scorched3d

%changelog
* Fri Apr 2 2004 Edward Rudd 
- Fixed data dir location and permissions
* Sun Dec 7 2003 Gavin Camp gcamp@scorched3d.co.uk
- First build.

