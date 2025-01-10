#include "imagevariant.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <iostream>

std::pair<QString, QString> getImageNameStream()
{

    QFile jsonFile(QString::fromLatin1("/usr/share/ublue-os/image-info.json"));

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

    return {
        jsonObj.value(QString::fromLatin1("image-name")).toString(),
        jsonObj.value(QString::fromLatin1("image-branch")).toString()
    };
}

ImageVariantInfo* ImageVariantInfo::loadFromDisk()
 {
    auto [imageName, imageStream] = getImageNameStream();
    std::cout << "Image name: " << imageName.toStdString() << std::endl;
    std::cout << "Image stream: " << imageStream.toStdString() << std::endl;
    ImageVariantInfo* info = new ImageVariantInfo;

    info->updateStream = UpdateStream::unknown;
    if (imageName == QString::fromLatin1("stable"))
        info->updateStream = UpdateStream::stableWeekly;
    else if (imageName == QString::fromLatin1("stable-daily"))
        info->updateStream = UpdateStream::stableDaily;
    else if (imageName == QString::fromLatin1("latest"))
        info->updateStream = UpdateStream::latest;

    //std::cout <<  info.updateStream << std::endl;

    return info;
 }

#include "imagevariant.moc"
