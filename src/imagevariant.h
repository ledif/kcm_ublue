#pragma once

#include <QObject>
#include <QString>

#include <memory>

class HWEFlagSet : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool hwe MEMBER hwe NOTIFY infoChanged)
  Q_PROPERTY(bool nvidia MEMBER nvidia NOTIFY infoChanged)
  Q_PROPERTY(bool nvidiaOpen MEMBER nvidiaOpen NOTIFY infoChanged)

public:
  bool hwe = false;
  bool nvidia = false;
  bool nvidiaOpen = false;

  HWEFlagSet(bool hwe, bool nvidia, bool nvidiaOpen);
  HWEFlagSet* clone();
  bool operator==(const HWEFlagSet&) const;

Q_SIGNALS:
  void infoChanged();
};

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
  Q_PROPERTY(HWEFlagSet* hweFlags READ getHWEFlags WRITE setHWEFlags NOTIFY infoChanged)
  Q_PROPERTY(UpdateStream updateStream MEMBER updateStream NOTIFY infoChanged)
  Q_PROPERTY(bool isDeprecatedStream MEMBER isDeprecatedStream CONSTANT)

public:
  ImageVariantInfo(QObject*, HWEFlagSet*, bool, UpdateStream, bool);
  ImageVariantInfo* clone();
  bool operator==(const ImageVariantInfo&) const;

  static ImageVariantInfo* loadFromDisk(QObject*);
  static ImageVariantInfo* parseFromImageNameAndTag(QObject*, const QString&, const QString&);

  HWEFlagSet* getHWEFlags();
  void setHWEFlags(HWEFlagSet*);
  UpdateStream getUpdateStream();
  bool getDevExperience() const;
  bool isDeprecated() const;

  QString asImageNameAndTag() const;

Q_SIGNALS:
  void infoChanged();

private:
  std::unique_ptr<HWEFlagSet> hweFlags;
  bool devExperience = false;
  UpdateStream updateStream = unknown;
  bool isDeprecatedStream;
};
