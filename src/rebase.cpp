#include "rebase.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusVariant>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QProcess>
#include <QFile>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

QString kRebaseManagerRootName = "ublue-rebase@"_L1;
QString kRebaseManagerName =  kRebaseManagerRootName + ".service"_L1;

QString kRunFilename = "/run/ublue-rebase"_L1;

// Example:
//   > systemd-escape --template ublue-rebase@.service aurora-dx-nvidia-open:stable-daily
//   ublue-rebase@aurora\x2ddx\x2dnvidia\x2dopen:stable\x2ddaily.service
QString getFullServiceName(const QString& rebaseTarget)
{
  QStringList arguments;
  arguments << "--template"_L1 << kRebaseManagerName << rebaseTarget;

  QProcess proc = QProcess(nullptr);
  proc.start("systemd-escape"_L1, arguments);
  proc.waitForFinished();

  if (proc.exitStatus() != QProcess::NormalExit)
  {
    qWarning() << "systemd-escape crashed";
    return {};
  }

  return QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
}


RebaseService::RebaseService()
{
  if (QFile::exists(kRunFilename))
  {
    QFile file(kRunFilename);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qWarning() << "Failed to open file:" << file.errorString();
    }

    QTextStream in(&file);

    QString line = in.readLine();
    if (!line.isNull()) {


      QStringList parts = line.split(QChar::fromLatin1(' '), Qt::SkipEmptyParts);
      if (parts.size() < 2) {
        qWarning() << "Line does not contain two space-delimited strings.";
      }

      // Assign values to foo and bar
      prettyName = parts[0];
      unitName = parts[1];
    } else {
      qWarning() << "run file is empty or unreadable.";

    }
  }
}



RebaseManager::RebaseManager(QObject* parent)
  : QObject(parent)
  , currentService(new RebaseService{})
{
  connect(this, &RebaseManager::runFileChanged, this, &RebaseManager::onRunFileChanged);

  if (QFile::exists(kRunFilename)) {
    this->fileWatcher.reset(new QFileSystemWatcher(this));
    fileWatcher->addPath(kRunFilename);
    connect(fileWatcher.get(), &QFileSystemWatcher::fileChanged, this, &RebaseManager::runFileChanged);

    Q_EMIT runFileChanged();
  } else {
    this->dirWatcher.reset(new QFileSystemWatcher(this));
    dirWatcher->addPath("/run"_L1);
    connect(dirWatcher.get(), &QFileSystemWatcher::directoryChanged, this, &RebaseManager::checkIfRunfileCreated);
  }
}

void RebaseManager::checkIfRunfileCreated()
{
  qDebug() << "/run dir changed";
  if (QFile::exists(kRunFilename)) {
    qDebug() << "/run/ublue-rebase created";
    this->fileWatcher.reset(new QFileSystemWatcher(this));
    fileWatcher->addPath(kRunFilename);
    connect(fileWatcher.get(), SIGNAL(&QFileSystemWatcher::fileChanged), this, SIGNAL(&RebaseManager::runFileChanged));
    Q_EMIT runFileChanged();

    this->dirWatcher.reset();
  }
}

void RebaseManager::onRunFileChanged()
{
  qDebug() << "onRunFileChanged";
  if (QFile::exists(kRunFilename)) {
    currentService.reset(new RebaseService());
    Q_EMIT serviceChanged(currentService.get());
  }
}


RebaseService* RebaseManager::getCurrentService()
{
  return currentService.get();
}

bool RebaseManager::startRebase(const QString& rebaseTarget)
{
  QString fullUnitName = getFullServiceName(rebaseTarget);

  // systemctl start
  QDBusMessage startUnitMessage = QDBusMessage::createMethodCall(
    "org.freedesktop.systemd1"_L1,
    "/org/freedesktop/systemd1"_L1,
    "org.freedesktop.systemd1.Manager"_L1,
    "StartUnit"_L1
  );

  startUnitMessage.setInteractiveAuthorizationAllowed(true);

  startUnitMessage.setArguments({fullUnitName, "replace"_L1});
  QDBusReply<QDBusObjectPath> startUnitReply = QDBusConnection::systemBus().call(startUnitMessage);

  qDebug() << startUnitMessage;
  qDebug() << startUnitReply;

  return startUnitReply.isValid();
}
