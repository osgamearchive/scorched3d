Name: scorched3d
Version: 36.2
Release: 1
License: GPL
Group: Amusements/Games
Summary: A 3D version of the classic DOS game Scorched Earth
URL: http://www.scorched3d.co.uk
Source0: http://unc.dl.sourceforge.net/sourceforge/scorched3d/Scorched3D-36.2-src.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

BuildRequires:  perl, automake, autoconf, SDL-devel, wxGTK-devel
BuildRequires:  SDL_mixer-devel, SDL_net-devel

%description
Scorched 3D is a game based loosely on the classic DOS game Scorched
Earth "The Mother Of All Games".

%prep
%setup -q -n scorched

%build
sh ./autogen.sh
%configure --prefix=%{_libdir}/games
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall prefix=$RPM_BUILD_ROOT%{_libdir}/games

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(777,root,root,-)
%{_libdir}/games/scorched3d

%changelog
* Sun Dec 7 2003 Gavin Camp gcamp@scorched3d.co.uk
- First build.

