#include "imagevariant.h"
//#include "ubluesettings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>

using namespace Qt::Literals::StringLiterals;

QString kImageInfoJson = "/usr/share/ublue-os/image-info.json"_L1;

std::pair<QString, QString> getImageNameStream()
{
  QFile jsonFile(kImageInfoJson);

  if (!jsonFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Couldn't open file " << kImageInfoJson;
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

ImageVariantInfo::ImageVariantInfo(QObject* parent, HWEFlagSet* hweFlags, bool devExperience, UpdateStream updateStream, bool isDeprecatedStream)
  : QObject(parent)
  , hweFlags(hweFlags)
  , devExperience(devExperience)
  , updateStream(updateStream)
  , isDeprecatedStream(isDeprecatedStream)
{
}

ImageVariantInfo* ImageVariantInfo::clone()
{
  return new ImageVariantInfo(nullptr, this->hweFlags->clone(), this->devExperience, this->updateStream, this->isDeprecatedStream);
}

bool ImageVariantInfo::operator==(const ImageVariantInfo& other) const
{
  return *hweFlags == *other.hweFlags && devExperience == other.devExperience && updateStream == other.updateStream;
}

ImageVariantInfo* ImageVariantInfo::loadFromDisk(QObject* parent)
{
  auto [imageName, imageStream] = getImageNameStream();
  qDebug() << "Image name: " << imageName;
  qDebug() << "Image stream: " << imageStream;
  return ImageVariantInfo::parseFromImageNameAndTag(parent, imageName, imageStream);
}

ImageVariantInfo* ImageVariantInfo::parseFromImageNameAndTag(QObject* parent, const QString& imageName, const QString& imageStream)
{
  // Update stream
  ImageVariantInfo::UpdateStream updateStream;
  if (imageStream == "stable"_L1)
    updateStream = ImageVariantInfo::stableWeekly;
  else if (imageStream == "stable-daily"_L1)
    updateStream = ImageVariantInfo::stableDaily;
  else if (imageStream == "latest"_L1)
    updateStream = ImageVariantInfo::latest;

  HWEFlagSet* hweFlags = new HWEFlagSet{false, false, false};

  // HWE flags
  if (imageName.contains("nvidia"_L1))
  {
    if (imageName.contains("nvidia-open"_L1))
      hweFlags->nvidiaOpen = true;
    else
      hweFlags->nvidia = true;
  }

  if (imageName.contains("hwe"_L1))
    hweFlags->hwe = true;

  bool isDeprecatedStream = imageName.contains("asus"_L1) || imageName.contains("surface"_L1);

  // Dev experience
  bool devExperience = imageName.contains("-dx"_L1);

  return new ImageVariantInfo{parent, hweFlags, devExperience, updateStream, isDeprecatedStream};
}


// Create the string representation of this image (e.g., aurora-dx-hwe)
QString ImageVariantInfo::asImageNameAndTag() const
{
  QString image = "aurora"_L1;

  if (devExperience)
    image += "-dx"_L1;

  if (hweFlags->hwe)
    image += "-hwe"_L1;
  if (hweFlags->nvidia)
    image += "-nvidia"_L1;
  if (hweFlags->nvidiaOpen)
    image += "-nvidia-open"_L1;


  image += ":"_L1;
  if (updateStream == UpdateStream::stableWeekly)
    image += "stable"_L1;
  else if (updateStream == UpdateStream::stableDaily)
    image += "stable-daily"_L1;
  else if (updateStream == UpdateStream::latest)
    image += "latest"_L1;

  return image;
}

bool ImageVariantInfo::getDevExperience() const
{
  return this->devExperience;
}

bool ImageVariantInfo::isDeprecated() const
{
  return this->isDeprecatedStream;
}

HWEFlagSet* ImageVariantInfo::getHWEFlags()
{
  return hweFlags.get();
}

void ImageVariantInfo::setHWEFlags(HWEFlagSet* x)
{
  return hweFlags.reset(x);
}

ImageVariantInfo::UpdateStream ImageVariantInfo::getUpdateStream()
{
  return updateStream;
}

HWEFlagSet::HWEFlagSet(bool hwe, bool nvidia, bool nvidiaOpen)
  : hwe(hwe), nvidia(nvidia), nvidiaOpen(nvidiaOpen)
{
}

HWEFlagSet* HWEFlagSet::clone()
{
  return new HWEFlagSet(this->hwe, this->nvidia, this->nvidiaOpen);
}

bool HWEFlagSet::operator==(const HWEFlagSet& other) const
{
  return hwe == other.hwe && nvidia == other.nvidia && nvidiaOpen == other.nvidiaOpen;
}
