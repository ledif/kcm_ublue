#pragma once

#include "systemdunitmonitor.h"

#include <QObject>
#include <QString>

// Representation of a systemd unit ublue-rebase@.service
struct RebaseService : public QObject
{
  enum ServiceStatus
  {
    noService = 0,
    started = 1,
    failed = 2,
    success = 3
  };
  
  Q_OBJECT
  Q_PROPERTY(QString prettyName MEMBER prettyName CONSTANT)
  Q_PROPERTY(ServiceStatus status MEMBER status CONSTANT)

public:
  RebaseService() = default;
  void reload(QString, QString);

  QString prettyName;
  QString unitName;
  ServiceStatus status = noService;

Q_SIGNALS:
  void stateChanged();

private Q_SLOTS:
  void onSystemdStateChanged(QString);

private:
  std::unique_ptr<SystemdUnitMonitor> systemdUnitMonitor;
};
