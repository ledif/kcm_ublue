#pragma once

#include <KQuickConfigModule>

class UBlueSettings : public KQuickConfigModule
{
    Q_OBJECT
    public:
        UBlueSettings(QObject *parent, const KPluginMetaData &data);
};

