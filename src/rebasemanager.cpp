#include "rebasemanager.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusVariant>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QProcess>
#include <QFile>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

QString kRebaseManagerRootName = "ublue-rebase@"_L1;
QString kRebaseManagerName =  kRebaseManagerRootName + ".service"_L1;

// Example:
//   > systemd-escape --template ublue-rebase@.service aurora-dx-nvidia-open:stable-daily
//   ublue-rebase@aurora\x2ddx\x2dnvidia\x2dopen:stable\x2ddaily.service
QString getFullServiceName(const QString& rebaseTarget)
{
  QStringList arguments;
  arguments << "--template"_L1 << kRebaseManagerName << rebaseTarget;

  QProcess proc = QProcess(nullptr);
  proc.start("systemd-escape"_L1, arguments);
  proc.waitForFinished();

  if (proc.exitStatus() != QProcess::NormalExit)
  {
    qWarning() << "systemd-escape crashed";
    return {};
  }

  return QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
}

RebaseService::RebaseService(QString prettyName, QString unitName)
  : prettyName(prettyName), unitName(unitName)
{ }

RebaseManager::RebaseManager(QObject* parent)
  : QObject(parent)
  , currentService(nullptr)
  , fileWatcher(new RebaseFileWatcher(this))
{
  if (auto parsedRunFile = fileWatcher->tryParse(); parsedRunFile)
    currentService.reset(new RebaseService(parsedRunFile->first, parsedRunFile->second));

  connect(fileWatcher.get(), &RebaseFileWatcher::runFileChanged, this, &RebaseManager::onRunFileChanged);
}

void RebaseManager::onRunFileChanged(QString prettyName, QString unitName)
{
  currentService.reset(new RebaseService(prettyName, unitName));
  Q_EMIT serviceChanged(currentService.get());
}

RebaseService* RebaseManager::getCurrentService()
{
  return currentService.get();
}

bool RebaseManager::startRebase(const QString& rebaseTarget)
{
  QString fullUnitName = getFullServiceName(rebaseTarget);

  // systemctl start ublue-rebase@aurora:stable.service
  QDBusMessage startUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "StartUnit"_L1
  );

  startUnitMessage.setInteractiveAuthorizationAllowed(true);

  startUnitMessage.setArguments({fullUnitName, "replace"_L1});
  QDBusReply<QDBusObjectPath> startUnitReply = QDBusConnection::systemBus().call(startUnitMessage);

  qDebug() << startUnitMessage;
  qDebug() << startUnitReply;

  return startUnitReply.isValid();
}
