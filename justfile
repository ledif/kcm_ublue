configure:
  rm -rf ~/kde/usr
  cmake -B build/ -DCMAKE_INSTALL_PREFIX=~/kde/usr

build:
  rm -rf ~/kde/usr
  cmake --build build/
  cmake --install build/

deps:
  dnf install kf6-kauth-devel kf6-kcmutils-devel kf6-kcodecs-devel kf6-kcolorscheme-devel kf6-kconfig-devel kf6-kconfigwidgets-devel kf6-kcoreaddons-devel kf6-ki18n-devel kf6-kservice-devel kf6-kwidgetsaddons-devel qt6-doc-devel.noarch qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qttools-devel
