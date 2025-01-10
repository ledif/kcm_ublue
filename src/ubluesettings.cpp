#include "ubluesettings.h"

#include <KPluginFactory>

#include <iostream>

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

bool isUpdateServiceEnabled()
{
  // TODO
  return true;
}

UBlueSettings::UBlueSettings(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
    , variantInfo(ImageVariantInfo::loadFromDisk())
    , updatesEnabled(isUpdateServiceEnabled())
{
    setButtons(Help | Apply | Default);
}

void UBlueSettings::updatesEnabledChanged()
{
  std::cout << "updatesEnabledChanged " << this->updatesEnabled << std::endl;
}

ImageVariantInfo* UBlueSettings::getImageVariant()
{
  return variantInfo.get();
}

void UBlueSettings::setImageVariant(ImageVariantInfo* x)
{
  variantInfo.reset(x);
}

#include "ubluesettings.moc"

