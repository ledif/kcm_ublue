#pragma once

#include <KQuickConfigModule>
#include "imagevariant.h"

class UBlueSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(bool updatesEnabled READ getUpdatesEnabled WRITE setUpdatesEnabled)
    Q_PROPERTY(ImageVariantInfo* imageVariant READ getImageVariant WRITE setImageVariant)

public:
    UBlueSettings(QObject *parent, const KPluginMetaData &data);

    bool getUpdatesEnabled();
    void setUpdatesEnabled(bool);

    ImageVariantInfo* getImageVariant();
    void setImageVariant(ImageVariantInfo*);

private:
    std::unique_ptr<ImageVariantInfo> variantInfo;
    bool updatesEnabled;
};

