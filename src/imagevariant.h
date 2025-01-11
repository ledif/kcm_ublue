#pragma once

#include <KQuickConfigModule>

#include <vector>


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
    static ImageVariantInfo* loadFromDisk();

    HWEFlagSet* getHWEFlags();
    void setHWEFlags(HWEFlagSet*);

Q_SIGNALS:
    void infoChanged();

private:
    std::unique_ptr<HWEFlagSet> hweFlags;
    bool devExperience = false;
    UpdateStream updateStream = unknown;
};
