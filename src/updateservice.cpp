#include "updateservice.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusObjectPath>
#include <QDBusInterface>

using namespace Qt::Literals::StringLiterals;

QString kUpdateServiceName = "rpm-ostreed-automatic.timer"_L1;

// Use D-Bus to basically call systemctl status rpm-ostreed-automatic.timer
// and check that it is enabled
bool UpdateService::isEnabled()
{
  QDBusMessage getUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "GetUnit"_L1
  );

  getUnitMessage.setArguments({kUpdateServiceName});
  QDBusMessage getUnitReply = QDBusConnection::systemBus().call(getUnitMessage);

  qDebug() << getUnitMessage;
  qDebug() << getUnitReply;

  // If it's disabled, sometimes systemd will report that the unit doesn't exist
  if (getUnitReply.type() == QDBusMessage::ErrorMessage)
    return false;

  // This should be something like "/org/freedesktop/systemd1/unit/ublue_2dupdate_2etimer"
  QDBusObjectPath objectPath = getUnitReply.arguments().at(0).value<QDBusObjectPath>();

  QDBusMessage activeStateMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    objectPath.path(),
    "org.freedesktop.DBus.Properties"_L1,
    "Get"_L1
  );

  activeStateMessage.setArguments({"org.freedesktop.systemd1.Unit"_L1, "ActiveState"_L1});
  QDBusMessage activeStateReply = QDBusConnection::systemBus().call(activeStateMessage);

  qDebug() << activeStateMessage;
  qDebug() << activeStateReply;

  // This should be either "active" or "inactive"
  QString activeState = activeStateReply.arguments().at(0).value<QDBusVariant>().variant().value<QString>();

  qDebug() << activeState;

  return activeState == "active"_L1;
}

void disableOrEnable(QString startOrStopUnit, QString disableorEnableUnitFiles, bool enable)
{
  // systemctl start/stop
  QDBusMessage stopUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    startOrStopUnit
  );

  stopUnitMessage.setInteractiveAuthorizationAllowed(true);

  stopUnitMessage.setArguments({kUpdateServiceName, "replace"_L1});
  QDBusMessage stopUnitReply = QDBusConnection::systemBus().call(stopUnitMessage);

  qDebug() << stopUnitMessage;
  qDebug() << stopUnitReply;

  // systemctl enable/disable
  QDBusMessage disableUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    disableorEnableUnitFiles
  );

  disableUnitMessage.setInteractiveAuthorizationAllowed(true);

  QList<QVariant> arguments;
  QStringList unitFiles;
  unitFiles << kUpdateServiceName;
  arguments << QVariant{unitFiles} << QVariant{false};

  if (enable)
    arguments << QVariant{true};

  disableUnitMessage.setArguments(arguments);
  QDBusMessage disableUnitReply = QDBusConnection::systemBus().call(disableUnitMessage);

  qDebug() << disableUnitMessage;
  qDebug() << disableUnitReply;

  // systemctl daemon-reload
  QDBusMessage daemonReloadMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "Reload"_L1
  );

  QDBusMessage daemonReloadReply = QDBusConnection::systemBus().call(daemonReloadMessage);

  qDebug() << daemonReloadMessage;
  qDebug() << daemonReloadReply;
}

void UpdateService::disable()
{
  disableOrEnable("StopUnit"_L1, "DisableUnitFiles"_L1, false);
}

void UpdateService::enable()
{
  disableOrEnable("StartUnit"_L1, "EnableUnitFiles"_L1, true);
}