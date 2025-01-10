#pragma once

#include <KQuickConfigModule>
#include "imagevariant.h"

class UBlueSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(bool updatesEnabled MEMBER updatesEnabled NOTIFY updatesEnabledChanged)
    Q_PROPERTY(ImageVariantInfo* imageVariant READ getImageVariant WRITE setImageVariant)

public:
    UBlueSettings(QObject *parent, const KPluginMetaData &data);

    ImageVariantInfo* getImageVariant();
    void setImageVariant(ImageVariantInfo*);

    void updatesEnabledChanged();

private:
    std::unique_ptr<ImageVariantInfo> variantInfo;
    bool updatesEnabled;
};

