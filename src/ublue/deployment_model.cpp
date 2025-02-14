#include "ublue/deployment_model.h"

using namespace Qt::Literals::StringLiterals;

namespace UBlue {

DeploymentModel::DeploymentModel(QObject*)
{
    roles[imageName] = "imageName";
    roles[imageTag] = "imageTag";
    roles[version] = "version";
    roles[isPinned] = "isPinned";
    roles[isDeployed] = "isDeployed";
}

void DeploymentModel::updateDeploymentList()
{
    beginResetModel();
    deployments.append(DeploymentInfo{"aurora-dx"_L1, "stable"_L1, "41.20250214.1"_L1, false, true});
    deployments.append(DeploymentInfo{"aurora-dx"_L1, "stable-daily"_L1, "41.20250213.1"_L1, false, true});
    endResetModel();
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
            return deployment.isPinned;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> DeploymentModel::roleNames() const
{
    return roles;
}

} // namespace UBlue