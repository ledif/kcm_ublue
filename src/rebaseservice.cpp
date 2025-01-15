#include "rebaseservice.h"


#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QVariantMap>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

void RebaseService::reload(QString pname, QString uname)
{
  prettyName = pname;
  unitName = uname;

  if (!unitName.isEmpty())
  {
    systemdUnitMonitor.reset(new SystemdUnitMonitor{unitName});
    connect(systemdUnitMonitor.get(), &SystemdUnitMonitor::unitStateChanged, this, &RebaseService::onSystemdStateChanged);
    Q_EMIT systemdUnitMonitor->unitStateChanged(systemdUnitMonitor->getCurrentActiveState(unitName));
  }
}

bool RebaseService::cancel()
{
  // systemctl stop
  QDBusMessage stopUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "StopUnit"_L1
  );

  stopUnitMessage.setInteractiveAuthorizationAllowed(true);

  stopUnitMessage.setArguments({unitName, "replace"_L1});
  QDBusMessage stopUnitReply = QDBusConnection::systemBus().call(stopUnitMessage);

  qDebug() << stopUnitMessage;
  qDebug() << stopUnitReply;

  return stopUnitReply.type() != QDBusMessage::ErrorMessage;
}


void RebaseService::onSystemdStateChanged(QString activeState)
{
  qDebug() << "onSystemdStateChanged" << activeState;
  if (activeState == "active"_L1 || activeState == "activating"_L1 || activeState == "deactivating"_L1 || activeState == "reloading"_L1)
    status = ServiceStatus::started;
  else if (activeState == "inactive"_L1)
    status = ServiceStatus::success;
  else if (activeState == "failed"_L1)
    status = ServiceStatus::failed;

  Q_EMIT stateChanged();
}
