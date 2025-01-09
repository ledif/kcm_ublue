#pragma once

#include <KQuickConfigModule>
#include "imagevariant.h"

class UBlueSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(bool updatesEnabled READ getUpdatesEnabled WRITE setUpdatesEnabled)

public:
    UBlueSettings(QObject *parent, const KPluginMetaData &data);

    bool getUpdatesEnabled();
    void setUpdatesEnabled(bool);

private:
    BootcImageVariantInfo variantInfo;
    bool updatesEnabled;
};

