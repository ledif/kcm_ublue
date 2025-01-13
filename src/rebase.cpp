#include "rebase.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QProcess>

using namespace Qt::Literals::StringLiterals;

QString kRebaseServiceName = "ublue-rebase@.service"_L1;

// systemd-escape --template ublue-rebase@.service aurora-dx-nvidia-open:stable-daily
QString getFullServiceName(const QString& rebaseTarget)
{
  QStringList arguments;
  arguments << "--template"_L1 << kRebaseServiceName << rebaseTarget;

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

void RebaseService::startRebase(const QString& rebaseTarget)
{
  QString fullUnitName = getFullServiceName(rebaseTarget);

  // systemctl start
  QDBusMessage startUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "StartUnit"_L1
  );

  startUnitMessage.setInteractiveAuthorizationAllowed(true);

  startUnitMessage.setArguments({fullUnitName, "replace"_L1});
  QDBusMessage startUnitReply = QDBusConnection::systemBus().call(startUnitMessage);

  qDebug() << startUnitMessage;
  qDebug() << startUnitReply;
}