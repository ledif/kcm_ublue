# Design

The rebase processes uses a shell script `/usr/libexec/ublue-rebase` that is called by a systemd unit called `ublue-rebase@.service`. This is a templated unit that can be parameterized by the image name to rebase to. For example, to rebase to the `aurora-dx:stable` image, the service launched would be `ublue-rebase@aurora\x2ddx:stable.service`.

The workflow works as follows:
- User selects a new image configuration (e.g., `aurora-dx:stable`) and clicks "Apply"
- The equivalent command is sent over dbus: `systemctl start ublue-rebase@aurora\x2ddx:stable.service`
- The `ublue-rebase@.service` executes the `ublue-rebase` script
- The `ublue-rebase` script creates a file at `/run/ublue-rebase` describing the in-process rebase and then calls either `rpm-ostree rebase` or `bootc switch`.
- Progress of the rebase is communicated back to the user by monitoring both the `/run/ublue-rebase` file and the status of the `ublue-rebase@.service` service using dbus.

## Qt Signals and Slots
![rebase-signal-slots](https://github.com/user-attachments/assets/4a0f1eda-06e7-4417-98a9-73eb78206f22)
