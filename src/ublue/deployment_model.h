#pragma once

#include <QAbstractListModel>

namespace UBlue {

class DeploymentModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    explicit DeploymentModel(QObject *parent);

    enum Roles {
        imageName = Qt::UserRole + 1,
        imageTag,
        version,
        isPinned,
        isDeployed,
        isStaged,
        isRollbackTarget,
        isPinnable
    };

    Q_ENUM(Roles)

    QVariant data(const QModelIndex &idx, int role) const override;
    //bool setData(const QModelIndex &idx, const QVariant &value, int role) override;
    int rowCount(const QModelIndex &parent) const override;

    //Q_INVOKABLE void addFolder(const QString &folder, const bool included);
    Q_INVOKABLE void pinOrUnpinDeployment(int index);
    Q_INVOKABLE void rollbackToDeployment(int index);
    Q_INVOKABLE void updateDeploymentList();
    QHash<int, QByteArray> roleNames() const override;

Q_SIGNALS:
    void deploymentListUpdated();

private:
    struct DeploymentInfo {
        QString imageName;
        QString imageTag;
        QString version;
        QString commit;
        bool isPinned;
        bool isDeployed;
        bool isStaged;
    };

    QList<DeploymentInfo> deployments;
    QHash<int, QByteArray> roles;
    //QStringList m_deletedSettings; //< track deleted entries

    void syncDeploymentConfig(const DeploymentInfo &entry);
};

} // namespace UBlue