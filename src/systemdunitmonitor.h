#pragma once

#include <QObject>
#include <QString>

class SystemdUnitMonitor : public QObject 
{
  Q_OBJECT

public:
  SystemdUnitMonitor(QString unitName);
  QString getCurrentActiveState(QString unitName);

  static QString getSystemdUnitObjectPath(const QString& unitName);


Q_SIGNALS:
  void unitStateChanged(QString newState);

private Q_SLOTS:
  void onPropertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList&);
};
