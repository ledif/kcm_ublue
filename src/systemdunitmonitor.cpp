#include "systemdunitmonitor.h"

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QVariantMap>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

QString SystemdUnitMonitor::getSystemdUnitObjectPath(const QString& unitName)
{
  QDBusMessage getUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "GetUnit"_L1
  );

  getUnitMessage.setArguments({unitName});
  QDBusMessage getUnitReply = QDBusConnection::systemBus().call(getUnitMessage);

  qDebug() << getUnitMessage;
  qDebug() << getUnitReply;

  QDBusObjectPath objectPath = getUnitReply.arguments().at(0).value<QDBusObjectPath>();
  return objectPath.path();
}

SystemdUnitMonitor::SystemdUnitMonitor(QString unitName)
{
  QString objectPath = SystemdUnitMonitor::getSystemdUnitObjectPath(unitName);

  bool connected = QDBusConnection::systemBus().connect(
    "org.freedesktop.systemd1"_L1,
    objectPath,
    "org.freedesktop.DBus.Properties"_L1,
    "PropertiesChanged"_L1,
    this,
    SLOT(onPropertiesChanged(QString, QVariantMap, QStringList))
  );

  if (!connected)
    qWarning() << "Failed to connect to systemd unit's PropertiesChanged signal for " << objectPath;
  else
    qDebug() << "Connected to systemd unit's PropertiesChanged signal.";
}

QString SystemdUnitMonitor::getCurrentActiveState(QString unitName)
{
  QString objectPath = SystemdUnitMonitor::getSystemdUnitObjectPath(unitName);

  QDBusMessage activeStateMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    objectPath,
    "org.freedesktop.DBus.Properties"_L1,
    "Get"_L1
  );

  activeStateMessage.setArguments({"org.freedesktop.systemd1.Unit"_L1, "ActiveState"_L1});
  QDBusMessage activeStateReply = QDBusConnection::systemBus().call(activeStateMessage);

  qDebug() << activeStateMessage;
  qDebug() << activeStateReply;

  QString activeState = activeStateReply.arguments().at(0).value<QDBusVariant>().variant().value<QString>();

  qDebug() << activeState;

  return activeState;
}

void SystemdUnitMonitor::onPropertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList&)
{
  if (interface == "org.freedesktop.systemd1.Unit"_L1 && changedProperties.contains("ActiveState"_L1))
  {
    QString newState = changedProperties.value("ActiveState"_L1).toString();
    qDebug() << "onPropertiesChanged " << newState;
    Q_EMIT unitStateChanged(newState);
  }
}
