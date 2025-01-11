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
{
  setButtons(Help | Apply);
  load();
}

void UBlueSettings::load()
{
  variantInfo.reset(ImageVariantInfo::loadFromDisk());
  updatesEnabled = isUpdateServiceEnabled();
  currentUpdatesEnabled = updatesEnabled;
  currentVariantInfo.reset(variantInfo->clone());

  connect(this, &UBlueSettings::infoChanged, this, &UBlueSettings::onInfoChanged);
}

void UBlueSettings::onInfoChanged()
{
  std::cout << "updatesEnabledChanged " << this->updatesEnabled << std::endl;

  this->setNeedsSave(currentUpdatesEnabled != updatesEnabled);
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

