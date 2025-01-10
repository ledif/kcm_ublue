#pragma once

#include <KQuickConfigModule>

#include <vector>


class HWEFlagSet : public QObject
{
  Q_OBJECT
  Q_PROPERTY(bool nvidia MEMBER nvidia)
  Q_PROPERTY(bool nvidiaOpen MEMBER nvidiaOpen)
  Q_PROPERTY(bool asus MEMBER asus)
  Q_PROPERTY(bool surface MEMBER surface)

public:
  bool nvidia = false;
  bool nvidiaOpen = false;
  bool asus = false;
  bool surface = false;
};


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
    Q_PROPERTY(bool devExperience MEMBER devExperience)
    Q_PROPERTY(HWEFlagSet* hweFlags READ getHWEFlags WRITE setHWEFlags)
    Q_PROPERTY(UpdateStream updateStream MEMBER updateStream)

public:
    static ImageVariantInfo* loadFromDisk();

    HWEFlagSet* getHWEFlags();
    void setHWEFlags(HWEFlagSet*);

private:
    std::unique_ptr<HWEFlagSet> hweFlags;
    bool devExperience = false;
    UpdateStream updateStream = unknown;
};
