build:
  podman build -t kcm_ublue-build .
  podman run -it --volume $PWD:/app:Z -w /app --name kcm_ublue --rm kcm_ublue-build just _containerized-build

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

release-new-version version:
  #!/bin/bash
  set -xeuo pipefail
  #test -z "$(git status --porcelain)" || (echo "Working copy is unclean. Please stash changes" && exit 1)

  if [[ "{{ version }}" == *"v"* ]]; then
    echo "Version string must not include 'v'"
    exit 1
  fi

  git fetch origin
  git checkout main
  git merge --ff-only origin/main
  sed -i 's/majmin_ver_kcm [[:digit:]].[[:digit:]].[[:digit:]]/majmin_ver_kcm {{ version }}/' kcm_ublue.spec
  git add kcm_ublue.spec
  git commit -m "chore: bump version to {{ version }}"
  git tag v{{ version }}
  git push origin main
  git push origin main v{{ version }}

  gh release create v{{ version }} --generate-notes

create-ublue-packages-pr version:
  #!/bin/bash
  set -xeuo pipefail

  if [[ "{{ version }}" == *"v"* ]]; then
    echo "Version string must not include 'v'"
    exit 1
  fi

  notes=$(gh release view v{{ version }} --json body --template '{{{{.body}}}}')
  tmpdir=$(mktemp -d --tmpdir=/tmp kcm_ublue.XXXXX)
  cd $tmpdir
  gh repo clone ublue-os-packages
  cd ublue-os-packages
  gh repo sync ledif/ublue-os-packages -b main

  branch="kcm_ublue_{{ version }}"
  git checkout -b "$branch"
  sed -i 's/majmin_ver_kcm [[:digit:]].[[:digit:]].[[:digit:]]/majmin_ver_kcm {{ version }}/' ublue/kcm_ublue/kcm_ublue.spec
  git add ublue/kcm_ublue/kcm_ublue.spec
  git commit -m "chore: bump kcm_ublue to {{ version }}"
  git push origin "$branch"
  gh pr create --title "chore: bump kcm_ublue to {{ version }}" --body $notes
