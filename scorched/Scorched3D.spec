Name: scorched3d
Version: 36.2
Release: 1
License: GPL
Group: Amusements/Games
Summary: A 3D version of the classic DOS game Scorched Earth
URL: http://www.scorched3d.co.uk
Source0: http://unc.dl.sourceforge.net/sourceforge/scorched3d/Scorched3D-%{version}-src.tar.gz
BuildRoot: %{_tmppath}/%{name}-buildroot

BuildRequires:  perl, automake, autoconf, SDL-devel, wxGTK-devel
BuildRequires:  SDL_mixer-devel, SDL_net-devel

%description
Scorched 3D is a game based loosely on the classic DOS game Scorched
Earth "The Mother Of All Games".

%prep
%setup -q -n scorched

%build
# sh ./autogen.sh
%configure --prefix=%{_libdir}/games
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall prefix=$RPM_BUILD_ROOT%{_libdir}/games
install -d %{buildroot}%{_bindir}

cat > %{buildroot}%{_bindir}/scorched3d <<EOF
#!/bin/sh
pushd %{_libdir}/games/scorched3d > /dev/null
./scorched3d $*
popd > /dev/null
EOF
chmod 755 %{buildroot}%{_bindir}/scorched3d
%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,games)
%{_libdir}/games/scorched3d
%{_bindir}/scorched3d

%changelog
* Sun Dec 7 2003 Gavin Camp gcamp@scorched3d.co.uk
- First build.

