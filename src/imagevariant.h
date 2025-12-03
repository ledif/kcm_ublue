#pragma once

#include <QObject>
#include <QString>

#include <memory>

class ImageVariantInfo : public QObject
{
public:
  enum UpdateStream
  {
    unknown = 0,
    stableWeekly = 1,
    stableDaily = 2,
    latest = 3,
  };

private:
  Q_OBJECT
  Q_PROPERTY(bool devExperience MEMBER devExperience NOTIFY infoChanged)
  Q_PROPERTY(bool nvidiaOpen MEMBER nvidiaOpen NOTIFY infoChanged)
  Q_PROPERTY(UpdateStream updateStream MEMBER updateStream NOTIFY infoChanged)
  Q_PROPERTY(bool isDeprecatedStream MEMBER isDeprecatedStream CONSTANT)

public:
  ImageVariantInfo(QObject*, bool nvidiaOpen, bool devExperience, UpdateStream, bool isDeprecatedStream);
  ImageVariantInfo* clone();
  bool operator==(const ImageVariantInfo&) const;

  static ImageVariantInfo* loadFromDisk(QObject*);
  static ImageVariantInfo* parseFromImageReference(QObject*, const QString&);

  UpdateStream getUpdateStream();
  bool getDevExperience() const;
  bool getNvidiaOpen() const;
  bool isDeprecated() const;

  QString asImageNameAndTag() const;

Q_SIGNALS:
  void infoChanged();

private:
  bool nvidiaOpen = false;
  bool devExperience = false;
  UpdateStream updateStream = unknown;
  bool isDeprecatedStream;
};
