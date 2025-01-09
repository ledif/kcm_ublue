#pragma once

#include <vector>

enum class HWEFlag
{
  nvidia, nvidiaOpen, asus, surface
};

enum class UpdateStream
{
  latest, stableDaily, stableWeekly
};

class BootcImageVariantInfo
{
public:
  static BootcImageVariantInfo loadFromDisk();

private:
  std::vector<HWEFlag> hweFlags;
  bool devExperience = false;
  UpdateStream updateStream;
};