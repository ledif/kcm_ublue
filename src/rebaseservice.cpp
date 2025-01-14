#include "rebaseservice.h"


#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QVariantMap>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

SystemdUnitMonitor::SystemdUnitMonitor(QString unitName)
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

  bool connected = QDBusConnection::systemBus().connect(
    "org.freedesktop.systemd1"_L1,
    objectPath.path(),
    "org.freedesktop.DBus.Properties"_L1,
    "PropertiesChanged"_L1,
    this,
    SLOT(onPropertiesChanged(QString, QVariantMap, QStringList))
  );

  if (!connected)
    qWarning() << "Failed to connect to systemd unit's PropertiesChanged signal for " << objectPath.path();
  else
    qDebug() << "Connected to systemd unit's PropertiesChanged signal.";
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

RebaseService::RebaseService(QString prettyName, QString unitName)
  : prettyName(prettyName), unitName(unitName)
{
  if (!unitName.isEmpty())
  {
    systemdUnitMonitor.reset(new SystemdUnitMonitor{unitName});
  }
}
