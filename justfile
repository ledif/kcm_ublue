build:
  podman build -t kcm_ublue_dev .
  podman run -it --volume $PWD:/app:Z -w /app kcm_ublue_dev just _containerized-build
  cat build/prefix.sh | sed "s|/app|$PWD|g" > prefix/env.sh
  rm -rf build

run:
  #!/bin/bash
  export QT_LOGGING_RULES="*.debug=true; qt.*.debug=false"
  source prefix/env.sh
  systemsettings kcm_ublue

_containerized-build:
  #!/bin/bash
  set -xeu
  rm -rf prefix build
  cd /app
  mkdir prefix
  cmake -B build -DCMAKE_INSTALL_PREFIX=/app/prefix/usr
  cmake --build build -j8
  cmake --install build