# KCM for Universal Blue images

![Screenshot_20250112_005607](https://github.com/user-attachments/assets/108de805-4aa3-4877-ad3e-246d5a13e403)

## Installation


> [!WARNING] 
> The following instructions grab the system extension from an external Cloudflare bucket. In the future, this repository
> will create and push signed images. In the meantime, only install arbitrary extensions if you trust the author.

These instructions will create a temporary installation using a [sysext](https://www.freedesktop.org/software/systemd/man/latest/systemd-sysext.html) placed in `/run/extensions`.
These changes are ephemeral and will not persist across reboots.
 
Download and install the `kcm_ublue` system extension:
```bash
wget https://pub-969fbc86b5f24e4d81c6d022e8fd8dde.r2.dev/kcm_ublue.raw 
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
