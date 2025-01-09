#include "imagevariant.h"

#include <QJsonObject>
#include <QFile>

std::string getImageName()
{
    QFile jsonFile(QString::fromLatin1("/usr/share/ublue-os/image-info.json"));

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open image-info.json file.");
    }

    return "foo";
}

 BootcImageVariantInfo BootcImageVariantInfo::loadFromDisk()
 {
    BootcImageVariantInfo info;
    info.updateStream = UpdateStream::stableWeekly;
    return info;
 }