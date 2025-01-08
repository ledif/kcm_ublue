build:
  podman build --volume $PWD:/app:Z -t kcm_ublue_dev .
  cat build/prefix.sh | sed "s|/app|$PWD|g" > prefix/build.sh
  rm -rf build

launch:
  #!/bin/bash
  . prefix/build.sh
  systemsettings kcm_ublue
