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
  QDBusMessage message = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "GetUnit"_L1
  );

  message.setArguments({kUpdateServiceName});
  QDBusMessage reply = QDBusConnection::systemBus().call(message);

  qDebug() << message;

  qDebug() << reply;

  QDBusObjectPath objectPath = reply.arguments().at(0).value<QDBusObjectPath>();

  QDBusInterface iface( "org.freedesktop.systemd1"_L1,
                        objectPath.path(),
                        "org.freedesktop.systemd1.Unit"_L1,
                        QDBusConnection::systemBus());
  QDBusMessage activeStateReply = iface.call("Get"_L1);

  /*QDBusMessage activeStateMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    objectPath.path(),
    "org.freedesktop.systemd1.Unit"_L1,
    "ActiveState"_L1
  );*/
  //qDebug() << activeStateMessage;

  //QDBusMessage activeStateReply = QDBusConnection::systemBus().call(activeStateMessage);

  qDebug() << activeStateReply;

  //std::cout << "systemd canconvert3 " << reply.arguments().at(0).canConvert<QDBusVariant>() << std::endl;


  return true;
}
