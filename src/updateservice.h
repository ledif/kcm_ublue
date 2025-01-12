#pragma once

class UpdateService
{
public:
  static bool isEnabled();
  static void enable();
  static void disable();
};