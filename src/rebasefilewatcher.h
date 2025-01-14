#pragma once

#include <QFileSystemWatcher>
#include <QObject>
#include <QString>

#include <optional>

// Class that emits a signal on these conditions:
//   - /run/ublue-rebase is created
//   - /run/ublue-rebase is modified
class RebaseFileWatcher : public QObject
{
  Q_OBJECT
public:
  RebaseFileWatcher(QObject*);
  std::optional<std::pair<QString, QString>> tryParse();

Q_SIGNALS:
  // The structure of the /run/ublue-rebase file contains two fields:
  //  - name of image (e.g., aurora-dx:latest)
  //  - name of systemd unit (e.g, ublue-rebase@aurora\x2ddx:latest.service)
  void runFileChanged(QString, QString);

private Q_SLOTS:
  void onRunFileCreatedOrChanged();

private:
  void checkIfRunfileCreated();

  std::unique_ptr<QFileSystemWatcher> fileWatcher;
  std::unique_ptr<QFileSystemWatcher> dirWatcher;
};
