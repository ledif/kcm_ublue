#pragma once

#include <KQuickConfigModule>

enum class HWEFlag
{
  nvidia, nvidiaOpen, asus, surface
};

enum class UpdateStream
{
  latest, stableDaily, stableWeekly
};

struct BootcImageVariantInfo
{
  std::vector<HWEFlag> hweFlags;
  bool devExperience = false;
};

class UBlueSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(bool updatesEnabled READ getUpdatesEnabled WRITE setUpdatesEnabled)

public:
    UBlueSettings(QObject *parent, const KPluginMetaData &data);

    bool getUpdatesEnabled();
    void setUpdatesEnabled(bool);

private:
    BootcImageVariantInfo variantInfo;
    UpdateStream updateStream;
    bool updatesEnabled;
};

