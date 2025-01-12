#pragma once

#include "imagevariant.h"

#include <KQuickConfigModule>
#include <QProcess>

class UBlueSettings : public KQuickConfigModule
{
  Q_OBJECT
  Q_PROPERTY(bool updatesEnabled MEMBER updatesEnabled NOTIFY infoChanged)
  Q_PROPERTY(ImageVariantInfo* imageVariant READ getImageVariant WRITE setImageVariant NOTIFY infoChanged)

public:
  UBlueSettings(QObject *parent, const KPluginMetaData &data);

  ImageVariantInfo* getImageVariant();
  void setImageVariant(ImageVariantInfo*);

Q_SIGNALS:
    void infoChanged();
    void resetPressed();

public Q_SLOTS:
    void onInfoChanged();
    void onResetPressed();
    void load() override;
    void save() override;

private:
  void startRebase();
  std::unique_ptr<QProcess> rebaseProcess;

  // Values of current settings before any changes
  bool currentUpdatesEnabled;
  std::unique_ptr<ImageVariantInfo> currentVariantInfo;

  // Values of (potentially new) settings
  std::unique_ptr<ImageVariantInfo> variantInfo;
  bool updatesEnabled;
};

