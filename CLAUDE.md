# CLAUDE.md

## Project Overview

This is a **KDE KCM (KConfig Module)** for [Universal Blue](https://universal-blue.org/) images, specifically Aurora (a Fedora Kinoite derivative). It provides a GUI in KDE System Settings for "rebasing" between different Aurora image variants (NVIDIA/Intel+AMD, developer experience, update streams).

## Tech Stack

- **Language**: C++17 with Qt6/KDE Frameworks 6
- **UI**: QML (Kirigami)
- **Build**: CMake with KDE ECM (Extra CMake Modules)
- **Testing**: Google Test (gtest)
- **Containerized builds**: Podman

## Project Structure

```
src/                    # C++ source and QML UI
├── ubluesettings.*     # Main KCM module, entry point
├── imagevariant.*      # Image variant model (NVIDIA, dev experience, streams)
├── rebasemanager.*     # Orchestrates rebase operations
├── rebaseservice.*     # Tracks systemd service state for rebases
├── rebasefilewatcher.* # Monitors /run/ublue-rebase for progress
├── systemdunitmonitor.*# DBus interface to systemd
├── updateservice.*     # Manages ublue-update.timer
└── ui/main.qml         # QML interface

system/usr/             # System files installed alongside KCM
├── libexec/ublue-rebase      # Shell script that performs the rebase
└── lib/systemd/system/       # ublue-rebase@.service template unit

tests/                  # Google Test unit tests
docs/design.md          # Architecture documentation with signal/slot diagram
```

## How Rebasing Works

See `docs/design.md` for the how rebasing works and signal/slot diagram.

## Development Commands

All commands use `just` (justfile):

```bash
just build        # Build in Podman container, run tests
just temp-install # Install to /usr (requires unlocked ostree)
just run          # Launch System Settings with debug logging
```

The build runs inside a Fedora 43 Podman container defined in `Containerfile`.

## Testing

Tests run automatically during `just build`.
