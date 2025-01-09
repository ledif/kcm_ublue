#include "ubluesettings.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

BootcImageVariantInfo getVariantInfo()
{
  // TODO
  return BootcImageVariantInfo{};
}

UpdateStream getUpdateStream()
{
  // TODO
  return UpdateStream::stableWeekly;
}

bool isUpdateServiceEnabled()
{
  // TODO
  return true;
}

UBlueSettings::UBlueSettings(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
    , variantInfo(getVariantInfo())
    , updateStream(getUpdateStream())
    , updatesEnabled(isUpdateServiceEnabled())
{
    setButtons(Help | Apply | Default);
}

bool UBlueSettings::getUpdatesEnabled()
{
  return updatesEnabled;
}

void UBlueSettings::setUpdatesEnabled(bool b)
{
  updatesEnabled = b;
}

#include "ubluesettings.moc"

