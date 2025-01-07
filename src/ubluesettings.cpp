#include "ubluesettings.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(UBlueSettings, "kcm_ublue.json")

UBlueSettings::UBlueSettings(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
{
    setButtons(Help | Apply | Default);
}

#include "ubluesettings.moc"

