#include "ubluesettings.h"
#include "updateservice.h"
#include "rebasemanager.h"

#include <KPluginFactory>
#include <KMessageDialog>
#include <QProcessEnvironment>

using namespace Qt::Literals::StringLiterals;

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

QString kTerminalApp = "/usr/bin/kde-ptyxis"_L1;

bool validateConfiguration(ImageVariantInfo* variantInfo)
{
  // HWE variants are purposefully only on latest
  // https://github.com/ublue-os/aurora/issues/114
  if (variantInfo->getHWEFlags()->hwe && variantInfo->getUpdateStream() != ImageVariantInfo::latest)
  {
    QString errorMessage = "Hardware enablement images can only be on the latest stream, not stable."_L1;
    auto *dlg = new KMessageDialog(KMessageDialog::Information, errorMessage);
    dlg->setCaption(QStringLiteral("Warning"));
    dlg->setDetails(QStringLiteral("See https://github.com/ublue-os/aurora/issues/114 for more details."));

    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowModality(Qt::WindowModal);
    dlg->show();

    return false;
  }

  return true;
}

UBlueSettings::UBlueSettings(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
    , rebaseManager(new RebaseManager(this))
{
  setButtons(Help | Apply);
  load();

  connect(this->getRebaseService(), &RebaseService::stateChanged, this, &UBlueSettings::onRebaseServiceChanged);
  connect(this->rebaseManager.get(), &RebaseManager::rebaseStarted, this, &UBlueSettings::onRebaseServiceChanged);
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

// Called when user clicks 'Apply'
void UBlueSettings::save()
{
  if (!validateConfiguration(variantInfo.get()))
    return;

  // Enable/disable updates if needed
  if (currentUpdatesEnabled != updatesEnabled)
  {
    if (updatesEnabled)
      UpdateService::enable();
    else
      UpdateService::disable();
  }

  // Rebase to new image if needed
  if (*currentVariantInfo != *variantInfo)
  {
    rebaseManager->startRebase(variantInfo->asImageNameAndTag());
  }
}

bool UBlueSettings::isRebaseInProgress()
{
  return !rebaseManager->getCurrentService()->unitName.isEmpty();
}

// Called when user clicks any UI element (checkbox, radio, etc)
void UBlueSettings::onInfoChanged()
{
  qDebug() << "onInfoChanged " << variantInfo->asImageNameAndTag();
  bool updatesEqual = currentUpdatesEnabled == updatesEnabled;
  bool imagesEqual = *currentVariantInfo == *variantInfo;

  this->setNeedsSave(!updatesEqual || !imagesEqual);
}

// Called when user clicks 'Reset'
void UBlueSettings::onResetPressed()
{
  qDebug() << "onResetPressed " << variantInfo->asImageNameAndTag();
}

void UBlueSettings::onRebaseServiceChanged()
{
  Q_EMIT rebaseServiceChanged();
  qDebug() << "onRebaseServiceChanged " << getRebaseService()->prettyName << getRebaseService()->status;
}

RebaseService* UBlueSettings::getRebaseService()
{
  return rebaseManager->getCurrentService();
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