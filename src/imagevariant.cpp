#include "imagevariant.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>

#include <iostream>

using namespace Qt::Literals::StringLiterals;

std::pair<QString, QString> getImageNameStream()
{

    QFile jsonFile("/usr/share/ublue-os/image-info.json"_L1);

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open image-info.json file.");
    }

    QByteArray ba = jsonFile.readAll();

    QJsonParseError parseError;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at" << parseError.offset << ":" << parseError.errorString();
    }

    QJsonObject jsonObj;
    jsonObj = jsonDoc.object();

    QString imageName = jsonObj.value("image-name"_L1).toString();

    return {
        imageName,
        // TODO: these fields are different in Aurora vs Bazzite
        imageName.startsWith("aurora"_L1) ?
            jsonObj.value("image-tag"_L1).toString() :
            jsonObj.value("image-branch"_L1).toString()
    };
}

ImageVariantInfo* ImageVariantInfo::loadFromDisk()
 {
    auto [imageName, imageStream] = getImageNameStream();
    std::cout << "Image name: " << imageName.toStdString() << std::endl;
    std::cout << "Image stream: " << imageStream.toStdString() << std::endl;
    ImageVariantInfo* info = new ImageVariantInfo;

    // Update stream
    if (imageStream == "stable"_L1)
        info->updateStream = stableWeekly;
    else if (imageStream == "stable-daily"_L1)
        info->updateStream = stableDaily;
    else if (imageStream == "latest"_L1)
        info->updateStream = latest;

    info->hweFlags.reset(new HWEFlagSet);

    // HWE flags
    if (imageName.contains("nvidia"_L1))
    {
        if (imageName.contains("nvidia-open"_L1))
          info->hweFlags->nvidiaOpen = true;
        else
          info->hweFlags->nvidia = true;
    }

    if (imageName.contains("asus"_L1))
      info->hweFlags->asus = true;

    if (imageName.contains("surface"_L1))
      info->hweFlags->surface = true;

    // Dev experience
    if (imageName.endsWith("-dx"_L1))
      info->devExperience = true;

    return info;
 }

HWEFlagSet* ImageVariantInfo::getHWEFlags()
{
  return hweFlags.get();
}

void ImageVariantInfo::setHWEFlags(HWEFlagSet* x)
{
  return hweFlags.reset(x);
}

//#include "imagevariant.moc"
