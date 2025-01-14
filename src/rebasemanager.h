#pragma once

#include "rebasefilewatcher.h"

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
  RebaseService(QString, QString);

  QString prettyName;
  QString unitName;
  ServiceStatus status;
};

class RebaseManager : public QObject
{
  Q_OBJECT
public:
  RebaseManager(QObject*);

  bool startRebase(const QString&);
  RebaseService* getCurrentService();

Q_SIGNALS:
  void serviceChanged(RebaseService*);

private Q_SLOTS:
  void onRunFileChanged(QString, QString);

private:
  std::unique_ptr<RebaseService> currentService;
  std::unique_ptr<RebaseFileWatcher> fileWatcher;
};
