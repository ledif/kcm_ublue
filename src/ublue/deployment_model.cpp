#include "ublue/deployment_model.h"


#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

using namespace Qt::Literals::StringLiterals;

namespace UBlue {

DeploymentModel::DeploymentModel(QObject*)
{
    roles[imageName] = "imageName";
    roles[imageTag] = "imageTag";
    roles[version] = "version";
    roles[isPinned] = "isPinned";
    roles[isDeployed] = "isDeployed";
    roles[isRollbackTarget] = "isRollbackTarget";
    roles[isPinnable] = "isPinnable";

}

void DeploymentModel::updateDeploymentList()
{
    beginResetModel();

    QProcess process;
    process.start("rpm-ostree"_L1, QStringList() << "status"_L1 << "--json"_L1);

    // Wait for the process to finish
    process.waitForFinished();

    // Get the output of the process
    QByteArray output = process.readAllStandardOutput();

    // Parse the JSON data
    QJsonDocument doc = QJsonDocument::fromJson(output);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Failed to parse JSON from rpm-ostree status --json.";
        return;
    }

    QJsonObject jsonObject = doc.object();

    if (jsonObject.contains("deployments"_L1) && jsonObject["deployments"_L1].isArray())
    {
        QJsonArray deploymentsArr = jsonObject["deployments"_L1].toArray();

        for (const QJsonValue &deploymentValue : deploymentsArr)
        {
            if (deploymentValue.isObject())
            {
                QJsonObject deploymentObject = deploymentValue.toObject();

                bool pinned = deploymentObject.contains("pinned"_L1) ? deploymentObject["pinned"_L1].toBool() : false;
                bool booted = deploymentObject.contains("booted"_L1) ? deploymentObject["booted"_L1].toBool() : false;
                QString imageVersion = deploymentObject["version"_L1].toString();

                QString fullImageRef = deploymentObject["container-image-reference"_L1].toString();
                QString imageRef = fullImageRef.split("/"_L1).last();

                QStringList parts = imageRef.split(":"_L1);
                QString imageName = parts.first();
                QString imageStream = parts.last();

                deployments.append(DeploymentInfo{imageName, imageStream, imageVersion, pinned, booted});

            }
        }
    } else {
        qWarning() << "'deployments' array not found in JSON output.";
    }

    endResetModel();
}


void DeploymentModel::pinOrUnpinDeployment(int index)
{
    qDebug() << "Pin or unpin " << index;

    QStringList arguments;
    arguments << "ostree"_L1 << "admin"_L1 << "pin"_L1; 
    if (deployments[index].isPinned)
        arguments << "-u"_L1;
    arguments<< QString::number(index);

    QProcess process;

    // Start the process with the appropriate command
    process.start("pkexec"_L1, arguments);

    // Wait for the process to finish
    if (!process.waitForFinished()) {
        qWarning() << "Process failed:" << process.errorString();
        return;
    }

    this->updateDeploymentList();
}

int DeploymentModel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
    Q_UNUSED(parent);
    qDebug() << "== DeploymentModel::rowCount " << deployments.size();
    return deployments.size();
}

QVariant DeploymentModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "getting data for row " << index.row();
    if (!index.isValid() || index.row() >= deployments.size())
        return QVariant();

    auto bootedIdx = std::distance(deployments.begin(), std::find_if(deployments.begin(), deployments.end(), [](const DeploymentInfo& info) {
        return info.isDeployed;
    }));

    auto deployment = deployments.at(index.row());
    switch (role) {
        case imageName:
            return deployment.imageName;
        case imageTag:
            return deployment.imageTag;
        case version:
            return deployment.version;
        case isPinned:
            return deployment.isPinned;
        case isDeployed:
            return deployment.isDeployed;
        case isRollbackTarget:
            // We can rollback to any deployments after the booted deployment
            return index.row() > bootedIdx;
        case isPinnable:
            // Can pin anything if nothing is staged, otherwise pin booted or older
            return index.row() >= bootedIdx;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> DeploymentModel::roleNames() const
{
    return roles;
}

} // namespace UBlue