#include "rebasefilewatcher.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

QString kRunFilename = "/run/ublue-rebase"_L1;

RebaseFileWatcher::RebaseFileWatcher(QObject* parent)
  : QObject(parent)
{
  if (QFile::exists(kRunFilename))
  {
    this->fileWatcher.reset(new QFileSystemWatcher(this));
    fileWatcher->addPath(kRunFilename);
    connect(fileWatcher.get(), &QFileSystemWatcher::fileChanged, this, &RebaseFileWatcher::onRunFileCreatedOrChanged);
  }
  else
  {
    this->dirWatcher.reset(new QFileSystemWatcher(this));
    dirWatcher->addPath("/run"_L1);
    connect(dirWatcher.get(), &QFileSystemWatcher::directoryChanged, this, &RebaseFileWatcher::checkIfRunfileCreated);
  }
}

void RebaseFileWatcher::checkIfRunfileCreated()
{
  qDebug() << "/run dir changed";
  if (QFile::exists(kRunFilename))
  {
    qDebug() << "/run/ublue-rebase created";

    // We don't care about any more changes in /run now
    this->dirWatcher.reset();
    onRunFileCreatedOrChanged();

    this->fileWatcher.reset(new QFileSystemWatcher(this));
    fileWatcher->addPath(kRunFilename);
    connect(fileWatcher.get(), &QFileSystemWatcher::fileChanged, this, &RebaseFileWatcher::onRunFileCreatedOrChanged);
  }
}

std::optional<std::pair<QString, QString>> RebaseFileWatcher::tryParse()
{
  if (!QFile::exists(kRunFilename))
    return {};

  QFile file(kRunFilename);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "run file exists but is unreadable";
    return {};
  }

  QTextStream in(&file);
  QString line;
  in.readLineInto(&line);

  if (line.isEmpty())
  {
    qWarning() << "run file exists but is empty";
    return {};
  }

  QStringList parts = line.split(QChar::fromLatin1(' '), Qt::SkipEmptyParts);
  if (parts.size() < 2)
  {
    qWarning() << "run file does not contain two space-delimited fields";
    return {};
  }

  return std::make_pair(parts[0], parts[1]);
}

void RebaseFileWatcher::onRunFileCreatedOrChanged()
{
  if (auto parsed = tryParse(); parsed)
    Q_EMIT runFileChanged(parsed->first, parsed->second);
  else
    qWarning() << "ERROR: expected runfile to exist";
}
