#pragma once

#include <QObject>
#include <QString>


class SystemdUnitMonitor : public QObject 
{
  Q_OBJECT

public:
  SystemdUnitMonitor(QString unitName);

Q_SIGNALS:
  void unitStateChanged(QString newState);

private Q_SLOTS:
  void onPropertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList&);

};

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
  RebaseService(QString, QString);

  QString prettyName;
  QString unitName;
  ServiceStatus status;

Q_SIGNALS:
  void systemdUnitStateChange(ServiceStatus);

private:
  std::unique_ptr<SystemdUnitMonitor> systemdUnitMonitor;
};
