#pragma once

#include <KQuickConfigModule>
#include "imagevariant.h"

class UBlueSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(bool updatesEnabled MEMBER updatesEnabled NOTIFY infoChanged)
    Q_PROPERTY(ImageVariantInfo* imageVariant READ getImageVariant WRITE setImageVariant NOTIFY infoChanged)

public:
    UBlueSettings(QObject *parent, const KPluginMetaData &data);
    void load() override;

    ImageVariantInfo* getImageVariant();
    void setImageVariant(ImageVariantInfo*);

Q_SIGNALS:
    void infoChanged();

public Q_SLOTS:
    void onInfoChanged();

private:
    std::unique_ptr<ImageVariantInfo> variantInfo;
    bool updatesEnabled;

    bool currentUpdatesEnabled;
    std::unique_ptr<ImageVariantInfo> currentVariantInfo;
};

