FROM ghcr.io/ublue-os/fedora-toolbox:latest
RUN dnf install -y \
  clang-format \
  cmake \
  erofs-utils \
  extra-cmake-modules \
  gcc-g++ \
  just \
  gtest-devel \
  kf6-kauth-devel \
  kf6-kcmutils-devel \
  kf6-kcodecs-devel \
  kf6-kcolorscheme-devel \
  kf6-kconfig-devel \
  kf6-kconfigwidgets-devel \
  kf6-kcoreaddons-devel \
  kf6-ki18n-devel \
  kf6-kservice-devel \
  kf6-kwidgetsaddons-devel \
  qt6-doc-devel \
  qt6-qtbase-devel \
  qt6-qtdeclarative-devel \
  qt6-qttools-devel
