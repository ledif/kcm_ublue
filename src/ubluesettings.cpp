#include "ubluesettings.h"
#include "updateservice.h"

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
    startRebase();
  }
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

// Launch a terminal and call rpm-ostree rebase / bootc switch
void UBlueSettings::startRebase()
{
  QString rebaseTarget = variantInfo->asImageNameAndTag();
  qDebug() << "Rebasing to " << rebaseTarget;

  // https://wiki.archlinux.org/title/Running_GUI_applications_as_root
  // TODO: there's gotta be a better way to do this
  auto runtimeDir = QProcessEnvironment::systemEnvironment().value("XDG_RUNTIME_DIR"_L1);
  auto waylandDisplay = QProcessEnvironment::systemEnvironment().value("WAYLAND_DISPLAY"_L1);
  auto waylandDisplayPath = runtimeDir + "/"_L1 + waylandDisplay;

  QStringList arguments;

  QString command = "echo Rebasing to "_L1 + rebaseTarget;
  command += "; rpm-ostree rebase ostree-image-signed:docker://ghcr.io/ublue-os/"_L1 + rebaseTarget;
  command += "; read -p 'Press enter to continue'"_L1;

  arguments << "env"_L1 << ("WAYLAND_DISPLAY="_L1 + waylandDisplayPath) << "XDG_RUNTIME_DIR=/run/user/0"_L1;
  arguments << kTerminalApp << "--"_L1 << "bash"_L1 << "-c"_L1 << command;

  rebaseProcess.reset(new QProcess(this));
  rebaseProcess->start("pkexec"_L1, arguments);
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

