#pragma once

#include <KQuickConfigModule>

#include <vector>

enum class HWEFlag
{
  nvidia, nvidiaOpen, asus, surface
};

enum class UpdateStream
{
  latest, stableDaily, stableWeekly, unknown
};

class ImageVariantInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool devExperience MEMBER devExperience)
    //Q_PROPERTY(ImageVariantInfo imageVariant READ getImageVariant WRITE setImageVariant)

public:
    static ImageVariantInfo* loadFromDisk();

private:
    std::vector<HWEFlag> hweFlags;
    bool devExperience = false;
    UpdateStream updateStream;
};
