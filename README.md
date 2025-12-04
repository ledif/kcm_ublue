# KCM for Universal Blue images

![Screenshot_20250124_212246](https://github.com/user-attachments/assets/843b5d78-9205-43c5-8074-d9dd02315e9e)

## Installation

This is now included in Aurora by default. The following instructions are to install the latest development version.

## Development 

Tested on Aurora 43 Developer Experience.

To build and run:
```bash
just build
just temp-install
just run
```

This will unlock ostree and install the plugin in `/usr`. This is an ephemeral installation and will be removed upon reboot.