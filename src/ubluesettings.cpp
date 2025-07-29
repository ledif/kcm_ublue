#include "ubluesettings.h"
#include "updateservice.h"
#include "rebasemanager.h"

#include <KPluginFactory>
#include <KPopupFrame>
#include <KMessageDialog>
#include <QProcessEnvironment>
#include <QPlainTextEdit>

#include <iostream>

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

  rebaseManager->tryReload();
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

  rebaseManager->tryReload();

  deploymentModel.reset(new UBlue::DeploymentModel(this));
  // Don't update here - let QML trigger the update after it's ready

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

  // TODO: this could be improved by checking if the rebase is successful but not super important
  if (this->isRebaseInProgress()) {
    this->setNeedsSave(true);
    return;
  }

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

UBlue::DeploymentModel* UBlueSettings::getDeploymentModel()
{
  return this->deploymentModel.get();
}


ImageVariantInfo* UBlueSettings::getImageVariant()
{
  return variantInfo.get();
}

void UBlueSettings::setImageVariant(ImageVariantInfo* x)
{
  variantInfo.reset(x);
}

void UBlueSettings::onRebaseCancelButtonPressed()
{
  qDebug() << "onRebaseCancelButtonPressed";
  if (!rebaseManager->getCurrentService()->cancel())
  {
    QString errorMessage = "Could not cancel rebase using systemd. Try to manually cancel the activated systemd units with: systemctl --legend=false list-units 'ublue-rebase@*' | awk '{print $2}' | xargs systemctl stop"_L1;
    auto *dlg = new KMessageDialog(KMessageDialog::Error, errorMessage);
    dlg->setCaption(QStringLiteral("Error"));

    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowModality(Qt::WindowModal);
    dlg->show();
  }
}

void UBlueSettings::onRebaseDetailsButtonPressed()
{
  qDebug() << "onRebaseDetailsButtonPressed" << rebaseManager->getCurrentService()->unitName;

    QStringList arguments;

    QString unitName = rebaseManager->getCurrentService()->unitName;

    // It was a nightmare figuring out how to pass unit names with backslashes
    unitName.replace("\\"_L1, "\\\\"_L1);

    QString command = "set -x; journalctl -n 50 -fu "_L1 + unitName;
    arguments <<  "--"_L1 << "bash"_L1 << "-c"_L1 << command;
    qDebug() << rebaseManager->getCurrentService()->unitName.toStdString();

    rebaseDetailsProcess.reset(new QProcess(this));
    rebaseDetailsProcess->start(kTerminalApp, arguments);
}

#include "ubluesettings.moc"