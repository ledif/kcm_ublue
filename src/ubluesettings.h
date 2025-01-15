#pragma once

#include "imagevariant.h"
#include "rebasemanager.h"

#include <KQuickConfigModule>
#include <QProcess>

class UBlueSettings : public KQuickConfigModule
{
  Q_OBJECT
  Q_PROPERTY(bool updatesEnabled MEMBER updatesEnabled NOTIFY infoChanged)
  Q_PROPERTY(ImageVariantInfo* imageVariant READ getImageVariant WRITE setImageVariant NOTIFY infoChanged)
  Q_PROPERTY(RebaseService* rebase READ getRebaseService CONSTANT)
  Q_PROPERTY(bool rebaseInProgress READ isRebaseInProgress CONSTANT)

public:
  UBlueSettings(QObject *parent, const KPluginMetaData &data);

  ImageVariantInfo* getImageVariant();
  void setImageVariant(ImageVariantInfo*);

  RebaseService* getRebaseService();
  bool isRebaseInProgress();

Q_SIGNALS:
    void infoChanged();
    void resetPressed();
    void rebaseServiceChanged();

public Q_SLOTS:
    void onInfoChanged();
    void onResetPressed();
    void load() override;
    void save() override;

private Q_SLOTS:
    void onRebaseServiceChanged();

private:
  std::unique_ptr<RebaseManager> rebaseManager;
  // Values of current settings before any changes
  bool currentUpdatesEnabled;
  std::unique_ptr<ImageVariantInfo> currentVariantInfo;

  // Values of (potentially new) settings
  std::unique_ptr<ImageVariantInfo> variantInfo;
  bool updatesEnabled;
};

