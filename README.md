# KCM for Universal Blue images

![Screenshot_20250124_212246](https://github.com/user-attachments/assets/843b5d78-9205-43c5-8074-d9dd02315e9e)

## Installation

### Via layering

```bash
curl -L https://copr.fedorainfracloud.org/coprs/ledif/kcm_ublue/repo/fedora-41/ledif-kcm_ublue-fedora-41.repo | \
  sudo tee /etc/yum.repos.d/_copr_ledif_kcm_ublue.repo
dnf install kcm_ublue
```

### Via sysext

> [!WARNING] 
> Installing a sysext with SELinux enabled can lead to an unstable system. Before installing this extension, please first verify that SELinux is set to permissive in `/etc/selinux/config`. If it is set to "enforcing," change it to "permissive" and reboot before continuing.

These instructions will create a temporary installation using a [sysext](https://www.freedesktop.org/software/systemd/man/latest/systemd-sysext.html) placed in `/run/extensions`.
These changes are ephemeral and will not persist across reboots.
 
Download and install the `kcm_ublue` system extension:
```bash
wget https://github.com/ledif/kcm_ublue/releases/download/aurora-41-x86-64/kcm_ublue.raw
sudo install -d -m 0755 -o 0 -g 0 /run/extensions/
sudo restorecon -RFv /run/extensions/
sudo install -m 644 -o 0 -g 0 kcm_ublue.raw /run/extensions/kcm_ublue.raw
sudo systemctl restart systemd-sysext.service
```

Verify that the sysext is loaded:

```
> systemd-sysext status
HIERARCHY EXTENSIONS    SINCE                      
/opt      none          -                          
/usr      kcm_ublue   Sun 2025-01-12 13:24:25 CST
```

Then, either manually launch the System Settings application and scroll to the bottom, or launch it directly with `systemsettings kcm_ublue`.

## Development 

Tested on Aurora 41 Developer Experience.

To build and run:
```bash
just build
just install
just run
```

This will create a [sysext](https://www.freedesktop.org/software/systemd/man/latest/systemd-sysext.html) in `/run/extensions`. This is an ephemeral installation and will be removed upon reboot.
