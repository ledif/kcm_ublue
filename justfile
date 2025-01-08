build:
  podman build -t kcm_ublue_dev .
  podman run -it --volume $PWD:/app:Z -w /app kcm_ublue_dev just containerized-build
  cat build/prefix.sh | sed "s|/app|$PWD|g" > prefix/build.sh
  rm -rf build

launch:
  #!/bin/bash
  . prefix/build.sh
  systemsettings kcm_ublue

containerized-build:
  #!/bin/bash
  rm -rf prefix build
  cd /app
  cmake -B build -DCMAKE_INSTALL_PREFIX=/app/prefix
  cmake --build build
  cmake --install build
