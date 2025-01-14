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
    connect(systemdUnitMonitor.get(), &SystemdUnitMonitor::unitStateChanged, this, &RebaseService::onSystemdStateChange);
  }
}

void RebaseService::onSystemdStateChange(QString activeState)
{
  if (activeState == "active"_L1)
    status = ServiceStatus::started;
  else if (activeState == "inactive"_L1)
    status = ServiceStatus::success;
  else if (activeState == "failed"_L1)
    status = ServiceStatus::failed;

  Q_EMIT stateChange(status);
}
