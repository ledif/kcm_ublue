FROM ghcr.io/ublue-os/fedora-toolbox:latest
RUN sudo dnf install -y gcc-g++ cmake cmake extra-cmake-modules \
          kf6-kauth-devel kf6-kcmutils-devel kf6-kcodecs-devel kf6-kcolorscheme-devel kf6-kconfig-devel kf6-kconfigwidgets-devel kf6-kcoreaddons-devel kf6-ki18n-devel kf6-kservice-devel kf6-kwidgetsaddons-devel qt6-doc-devel qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qttools-devel
RUN rm -rf /app/prefix /app/build && \
    (cd /app && cmake -B build -DCMAKE_INSTALL_PREFIX=/app/prefix && \
    cmake --build build && \
    cmake --install build)


