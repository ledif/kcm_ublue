build:
  podman build -t kcm_ublue_dev .
  podman run -it --volume $PWD:/app:Z -w /app kcm_ublue_dev just _containerized-build

run:
  #!/bin/bash
  export QT_LOGGING_RULES="*.debug=true; qt.*.debug=false"
  systemsettings kcm_ublue

install:
  #!/bin/bash
  set -xeu

  if getenforce | grep -q Enforcing; then
    echo "SELinux and systemd-sysext don't play well here. Set SELInux to permissive first."
    exit 1
  fi

  sudo install -d -m 0755 -o 0 -g 0 /run/extensions/
  sudo restorecon -RFv /run/extensions/
  sudo install -m 644 -o 0 -g 0 kcm_ublue.raw /run/extensions/kcm_ublue.raw
  rm kcm_ublue.raw
  sudo systemctl restart systemd-sysext.service
  systemd-sysext status
  sudo systemctl daemon-reload
  sudo rm -f /run/ublue-rebase
  #sudo systemctl restart polkit

_containerized-build:
  #!/bin/bash
  set -xeu
  rm -rf prefix build
  mkdir prefix
  cmake -B build -DCMAKE_INSTALL_PREFIX=./prefix/usr
  cmake --build build -j8
  (cd build && ctest --output-on-failure)
  cmake --install build

  mkdir prefix/usr/lib64/qt6
  mv prefix/usr/lib64/plugins prefix/usr/lib64/qt6

  cp -r system/usr/* prefix/usr

  rm -f kcm_ublue.raw
  mkfs.erofs -zlz4 kcm_ublue.raw prefix
