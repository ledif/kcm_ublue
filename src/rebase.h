#pragma once

#include <QFileSystemWatcher>
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
  RebaseService();

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
  const RebaseService* getCurrentService();

Q_SIGNALS:
  void serviceChanged();
  void runFileChanged();

public Q_SLOTS:
  void onRunFileChanged();

private:
  void checkIfRunfileCreated();

  std::unique_ptr<RebaseService> currentService;
  std::unique_ptr<QFileSystemWatcher> fileWatcher;
  std::unique_ptr<QFileSystemWatcher> dirWatcher;
};
