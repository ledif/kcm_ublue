#include "ubluesettings.h"
#include "updateservice.h"

#include <KPluginFactory>

#include <iostream>

using namespace Qt::Literals::StringLiterals;

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

QString kTerminalApp = "/usr/bin/kde-ptyxis"_L1;

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

  connect(this, &UBlueSettings::infoChanged, this, &UBlueSettings::onInfoChanged);
  connect(variantInfo.get(), &ImageVariantInfo::infoChanged, this, &UBlueSettings::onInfoChanged);
  connect(variantInfo->getHWEFlags(), &HWEFlagSet::infoChanged, this, &UBlueSettings::onInfoChanged);

  Q_EMIT onResetPressed();
}

void UBlueSettings::save()
{
  // Enable/disable updates if needed
  if (currentUpdatesEnabled != updatesEnabled)
  {
    if (updatesEnabled)
      UpdateService::enable();
    else
      UpdateService::disable();
  }

  if (*currentVariantInfo != *variantInfo)
  {
    startRebase();
  }
}

void UBlueSettings::startRebase()
{
  QString rebaseTarget = variantInfo->asImageNameAndTag();
  qDebug() << "Rebasing to " << rebaseTarget;

  QStringList arguments;

  QString command = "echo Rebasing to "_L1 + rebaseTarget;
  command += "; sudo rpm-ostree rebase ostree-image-signed:docker://ghcr.io/ublue-os/"_L1 + rebaseTarget;
  command += "; read -p 'Press enter to continue'"_L1;

  arguments << "--"_L1 << "bash"_L1 << "-c"_L1 << command;

  rebaseProcess.reset(new QProcess(this));
  rebaseProcess->start(kTerminalApp, arguments);
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

