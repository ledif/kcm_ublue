#pragma once

// Representation of the systemd unit rpm-ostreed-automatic.timer
class UpdateService
{
public:
  static bool isEnabled();
  static void enable();
  static void disable();
};