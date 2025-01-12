#pragma once

// Representation of the systemd unit ublue-update.timer
class UpdateService
{
public:
  static bool isEnabled();
  static void enable();
  static void disable();
};