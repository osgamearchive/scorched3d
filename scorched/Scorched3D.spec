Name: scorched3d
Version: 37
Release: 1
License: GPL
Group: Amusements/Games
Summary: A 3D version of the classic DOS game Scorched Earth
URL: http://www.scorched3d.co.uk
Source0: http://unc.dl.sourceforge.net/sourceforge/scorched3d/Scorched3D-%{version}-src.tar.gz
BuildRoot: %{_tmppath}/%{name}-buildroot

BuildRequires:  perl, automake, autoconf, SDL, SDL-devel, wxGTK, wxGTK-devel
BuildRequires:  SDL_mixer, SDL_mixer-devel, SDL_net, SDL_net-devel
BuildRequires:  freetype, freetype-devel

%description
Scorched 3D is a game based loosely on the classic DOS game Scorched
Earth "The Mother Of All Games".

%prep
%setup -q -n scorched

%build
# sh ./autogen.sh
%configure 
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall -j2

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,games,755)
%{_datadir}/data
%{_datadir}/doc
%attr(755,root,games)
%{_bindir}/scorched3d

%changelog
* Sun Dec 7 2003 Gavin Camp gcamp@scorched3d.co.uk
- First build.

