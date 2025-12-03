FROM registry.fedoraproject.org/fedora:43
RUN dnf install -y gcc-g++ cmake extra-cmake-modules just gtest-devel \
          kf6-kauth-devel kf6-kcmutils-devel kf6-kcodecs-devel kf6-kcolorscheme-devel kf6-kconfig-devel kf6-kconfigwidgets-devel kf6-kcoreaddons-devel kf6-ki18n-devel kf6-kservice-devel kf6-kwidgetsaddons-devel qt6-doc-devel qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qttools-devel
