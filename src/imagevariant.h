#pragma once

#include <KQuickConfigModule>

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

  HWEFlagSet(bool hwe, bool nvidia, bool nvidiaOpen)
    : hwe(hwe), nvidia(nvidia), nvidiaOpen(nvidiaOpen)
  { }

  HWEFlagSet* clone();

  bool operator==(const HWEFlagSet&) const;

Q_SIGNALS:
  void infoChanged();
};

class UBlueSettings;

class ImageVariantInfo : public QObject
{
  enum UpdateStream
  {
    unknown = 0,
    stableWeekly = 1,
    stableDaily = 2,
    latest = 3,
  };

  Q_OBJECT
  Q_PROPERTY(bool devExperience MEMBER devExperience NOTIFY infoChanged)
  Q_PROPERTY(HWEFlagSet* hweFlags READ getHWEFlags WRITE setHWEFlags NOTIFY infoChanged)
  Q_PROPERTY(UpdateStream updateStream MEMBER updateStream NOTIFY infoChanged)

public:
  ImageVariantInfo(QObject*, HWEFlagSet*, bool, UpdateStream);
  ImageVariantInfo* clone();

  bool operator==(const ImageVariantInfo&) const;

  static ImageVariantInfo* loadFromDisk(QObject*);

  HWEFlagSet* getHWEFlags();
  void setHWEFlags(HWEFlagSet*);

  QString asImageNameAndTag() const;

Q_SIGNALS:
  void infoChanged();

private:
  std::unique_ptr<HWEFlagSet> hweFlags;
  bool devExperience = false;
  UpdateStream updateStream = unknown;
};
