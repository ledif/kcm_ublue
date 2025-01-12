#include "imagevariant.h"
#include "ubluesettings.h"

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

ImageVariantInfo::ImageVariantInfo(QObject* parent, HWEFlagSet* hweFlags, bool devExperience, UpdateStream updateStream)
  : QObject(parent)
  , hweFlags(hweFlags)
  , devExperience(devExperience)
  , updateStream(updateStream)
{
}

ImageVariantInfo* ImageVariantInfo::clone()
{
  return new ImageVariantInfo(nullptr, this->hweFlags->clone(), this->devExperience, this->updateStream);
}

bool ImageVariantInfo::operator==(const ImageVariantInfo& other) const
{
  return *hweFlags == *other.hweFlags && devExperience == other.devExperience && updateStream == other.updateStream;
}

ImageVariantInfo* ImageVariantInfo::loadFromDisk(QObject* parent)
{
  auto [imageName, imageStream] = getImageNameStream();
  std::cout << "Image name: " << imageName.toStdString() << std::endl;
  std::cout << "Image stream: " << imageStream.toStdString() << std::endl;

  // Update stream
  UpdateStream updateStream;
  if (imageStream == "stable"_L1)
    updateStream = stableWeekly;
  else if (imageStream == "stable-daily"_L1)
    updateStream = stableDaily;
  else if (imageStream == "latest"_L1)
    updateStream = latest;

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

  // TODO: display a deprecation warning if user is on asus or surface
  /*if (imageName.contains("asus"_L1))

  if (imageName.contains("surface"_L1))*/

  // Dev experience
  bool devExperience = imageName.contains("-dx"_L1);

  return new ImageVariantInfo{parent, hweFlags, devExperience, updateStream};
 }

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

HWEFlagSet* HWEFlagSet::clone()
{
  return new HWEFlagSet(this->hwe, this->nvidia, this->nvidiaOpen);
}

bool HWEFlagSet::operator==(const HWEFlagSet& other) const
{
  return hwe == other.hwe && nvidia == other.nvidia && nvidiaOpen == other.nvidiaOpen;
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
