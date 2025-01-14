#pragma once

#include "rebasefilewatcher.h"
#include "rebaseservice.h"

#include <QObject>
#include <QString>

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
