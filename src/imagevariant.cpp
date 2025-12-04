#include "imagevariant.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProcess>
#include <QString>

using namespace Qt::Literals::StringLiterals;

QString getBootedContainerImageReference() {
    QProcess process;
    process.start("rpm-ostree"_L1, QStringList() << "status"_L1 << "--json"_L1);
    process.waitForFinished();

    QByteArray output = process.readAllStandardOutput();
    if (output.isEmpty()) {
        qWarning() << "Failed to retrieve rpm-ostree status JSON.";
        return QString();
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(output);
    if (!jsonDoc.isObject()) {
        qWarning() << "Invalid JSON format.";
        return QString();
    }

    QJsonObject rootObj = jsonDoc.object();
    if (!rootObj.contains("deployments"_L1) || !rootObj["deployments"_L1].isArray()) {
        qWarning() << "Missing 'deployments' array in JSON.";
        return QString();
    }

    QJsonArray deployments = rootObj["deployments"_L1].toArray();
    for (const QJsonValue &deploymentValue : deployments) {
        if (!deploymentValue.isObject()) continue;

        QJsonObject deployment = deploymentValue.toObject();
        if (deployment.contains("booted"_L1) && deployment["booted"_L1].toBool()) {
            if (deployment.contains("container-image-reference"_L1)) {
                return deployment["container-image-reference"_L1].toString();
            } else {
                qWarning() << "Booted deployment does not have 'container-image-reference'.";
                return QString();
            }
        }
    }

    qWarning() << "No booted deployment found.";
    return QString();
}


ImageVariantInfo::ImageVariantInfo(QObject* parent, bool nvidiaOpen, bool devExperience, UpdateStream updateStream, bool isDeprecatedStream)
  : QObject(parent)
  , nvidiaOpen(nvidiaOpen)
  , devExperience(devExperience)
  , updateStream(updateStream)
  , isDeprecatedStream(isDeprecatedStream)
{
}

ImageVariantInfo* ImageVariantInfo::clone()
{
  return new ImageVariantInfo(nullptr, this->nvidiaOpen, this->devExperience, this->updateStream, this->isDeprecatedStream);
}

bool ImageVariantInfo::operator==(const ImageVariantInfo& other) const
{
  return nvidiaOpen == other.nvidiaOpen && devExperience == other.devExperience && updateStream == other.updateStream;
}

ImageVariantInfo* ImageVariantInfo::loadFromDisk(QObject* parent)
{
  auto imageReference = getBootedContainerImageReference();
  qDebug() << "Image reference: " << imageReference;
  return ImageVariantInfo::parseFromImageReference(parent, imageReference);
}

ImageVariantInfo* ImageVariantInfo::parseFromImageReference(QObject* parent, const QString& imageReference)
{
  // Example: ostree-image-signed:docker://ghcr.io/ublue-os/aurora-dx:latest -> aurora-dx:latest
  QString imageRef = imageReference.split("/"_L1).last();

  QStringList parts = imageRef.split(":"_L1);
  QString imageName = parts.first();
  QString imageStream = parts.last();

  // Update stream
  ImageVariantInfo::UpdateStream updateStream;
  if (imageStream == "stable"_L1)
    updateStream = ImageVariantInfo::stableWeekly;
  else if (imageStream == "stable-daily"_L1)
    updateStream = ImageVariantInfo::stableDaily;
  else if (imageStream == "latest"_L1)
    updateStream = ImageVariantInfo::latest;

  // NVIDIA Open flag
  bool nvidiaOpen = imageName.contains("nvidia-open"_L1);

  bool isDeprecatedStream = imageName.contains("asus"_L1) || imageName.contains("surface"_L1);

  // Dev experience
  bool devExperience = imageName.contains("-dx"_L1);

  return new ImageVariantInfo{parent, nvidiaOpen, devExperience, updateStream, isDeprecatedStream};
}


// Create the string representation of this image (e.g., aurora-dx-nvidia-open)
QString ImageVariantInfo::asImageNameAndTag() const
{
  QString image = "aurora"_L1;

  if (devExperience)
    image += "-dx"_L1;

  if (nvidiaOpen)
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

bool ImageVariantInfo::getNvidiaOpen() const
{
  return this->nvidiaOpen;
}

bool ImageVariantInfo::isDeprecated() const
{
  return this->isDeprecatedStream;
}

ImageVariantInfo::UpdateStream ImageVariantInfo::getUpdateStream()
{
  return updateStream;
}
