build:
  podman build -t kcm_ublue_dev .
  podman run -it --volume $PWD:/app:Z -w /app kcm_ublue_dev just _containerized-build
  cat build/prefix.sh | sed "s|/app|$PWD|g" > prefix/env.sh
  rm -rf build

run:
  #!/bin/bash
  . prefix/env.sh
  export QT_LOGGING_RULES="*.debug=true; qt.*.debug=false"
  systemsettings kcm_ublue

install:
  #!/bin/bash
  set -x
  sudo mkdir -p /run/extensions
  sudo cp *.raw /run/extensions


_containerized-build:
  #!/bin/bash
  set -xeu
  rm -rf prefix build
  cd /app
  mkdir prefix
  cmake -B build -DCMAKE_INSTALL_PREFIX=/app/prefix/usr
  cmake --build build -j8
  cmake --install build
  mkfs.erofs -zlz4 ublue_kcm-41.20250105.0-x86_64.raw prefix
