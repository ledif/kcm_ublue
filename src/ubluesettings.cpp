#include "ubluesettings.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

bool isUpdateServiceEnabled()
{
  // TODO
  return true;
}

UBlueSettings::UBlueSettings(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
    , variantInfo(BootcImageVariantInfo::loadFromDisk())
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

