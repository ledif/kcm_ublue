#include "updateservice.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusObjectPath>
#include <QDBusInterface>

#include <iostream>

using namespace Qt::Literals::StringLiterals;

QString kUpdateServiceName = "ublue-update.timer"_L1;

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

  QString activeState = activeStateReply.arguments().at(0).value<QDBusVariant>().variant().value<QString>();

  qDebug() << activeState;

  return activeState == "active"_L1;
}
