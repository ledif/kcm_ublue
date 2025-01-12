#include "ubluesettings.h"
#include "updateservice.h"

#include <KPluginFactory>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusObjectPath>

#include <iostream>

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

using namespace Qt::Literals::StringLiterals;

UBlueSettings::UBlueSettings(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
{
  setButtons(Help | Apply);
  load();
}

void UBlueSettings::load()
{
  variantInfo.reset(ImageVariantInfo::loadFromDisk(this));
  updatesEnabled = UpdateService::isEnabled();
  currentUpdatesEnabled = updatesEnabled;
  currentVariantInfo.reset(variantInfo->clone());

  std::cout << "load called" << std::endl;

  connect(this, &UBlueSettings::infoChanged, this, &UBlueSettings::onInfoChanged);
  connect(variantInfo.get(), &ImageVariantInfo::infoChanged, this, &UBlueSettings::onInfoChanged);
  connect(variantInfo->getHWEFlags(), &HWEFlagSet::infoChanged, this, &UBlueSettings::onInfoChanged);

  Q_EMIT onResetPressed();
}

void UBlueSettings::save()
{
  std::cout << "rebase " << variantInfo->asImageNameAndTag().toStdString() << std::endl;

  if (currentUpdatesEnabled != updatesEnabled)
  {
    if (updatesEnabled)
      UpdateService::enable();
    else
      UpdateService::disable();
  }

}

void UBlueSettings::onInfoChanged()
{
  std::cout << "onInfoChanged " << variantInfo->asImageNameAndTag().toStdString() << std::endl;
  bool updatesEqual = currentUpdatesEnabled == updatesEnabled;
  bool imagesEqual = *currentVariantInfo == *variantInfo;

  this->setNeedsSave(!updatesEqual || !imagesEqual);
}

void UBlueSettings::onResetPressed()
{
  std::cout << "onResetPressed " << variantInfo->asImageNameAndTag().toStdString() << std::endl;
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

