Name:           kcm_ublue
Version:        0.5.0
Release:        1%{?dist}
Summary:        KCM for KDE-based Universal Blue images

URL:            https://github.com/ledif/kcm_ublue
Source:         https://github.com/ledif/kcm_ublue/archive/refs/tags/v0.5.0.zip
License:        Apache-2.0

BuildRequires:  cmake
BuildRequires:  gcc-g++
BuildRequires:  extra-cmake-modules
BuildRequires:  kf6-kauth-devel
BuildRequires:  kf6-kcmutils-devel
BuildRequires:  kf6-kcodecs-devel
BuildRequires:  kf6-kcolorscheme-devel
BuildRequires:  kf6-kconfig-devel
BuildRequires:  kf6-kconfigwidgets-devel
BuildRequires:  kf6-kcoreaddons-devel
BuildRequires:  kf6-ki18n-devel
BuildRequires:  kf6-kservice-devel
BuildRequires:  kf6-kwidgetsaddons-devel
BuildRequires:  qt6-doc-devel
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtdeclarative-devel
BuildRequires:  qt6-qttools-devel

%description
KDE Configuration Module (KCM) for Aurora.

%prep
%autosetup -n %{name}-%{version}

%build
  mkdir build
  cmake -B build -DCMAKE_INSTALL_PREFIX=%{buildroot}
  cmake --build build

%install
  cmake --install build

  mkdir %{buildroot}/lib64/qt6
  mv %{buildroot}/lib64/plugins %{buildroot}/lib64/qt6

%files

%{_libdir}/qt6/plugins/plasma/kcms/systemsettings/kcm_ublue.so
%{_datadir}/applications/kcm_ublue.desktop
%{_datadir}/polkit-1/actions/org.ublue.rebase.policy
%{_datadir}/polkit-1/rules.d/21-ublue-rebase.rules
%{_datadir}/polkit-1/rules.d/22-ublue-rebase-systemd.rules
%{_prefix}/lib/systemd/system/ublue-rebase@.service
%{_libexecdir}/libexec/ublue-rebase


%changelog
%autochangelog
