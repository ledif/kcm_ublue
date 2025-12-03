#include <gtest/gtest.h>
#include "imagevariant.h"

using namespace Qt::Literals::StringLiterals;

TEST(ImageVariantTest, ParseImageName_DevExp) {
    QString imageReference = "ostree-image-signed:docker://ghcr.io/ublue-os/aurora-dx:latest"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageReference(nullptr, imageReference)};

    EXPECT_TRUE(info->getDevExperience());
    EXPECT_FALSE(info->getNvidiaOpen());
    EXPECT_EQ(info->getUpdateStream(), ImageVariantInfo::UpdateStream::latest);
}

TEST(ImageVariantTest, ParseImageName_Standard) {
    QString imageReference = "ostree-image-signed:docker://ghcr.io/ublue-os/aurora:stable"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageReference(nullptr, imageReference)};

    EXPECT_FALSE(info->getDevExperience());
    EXPECT_FALSE(info->getNvidiaOpen());
    EXPECT_EQ(info->getUpdateStream(), ImageVariantInfo::UpdateStream::stableWeekly);
}

TEST(ImageVariantTest, ParseImageName_NvidiaOpen) {
    QString imageReference = "ostree-image-signed:docker://ghcr.io/ublue-os/aurora-dx-nvidia-open:stable-daily"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageReference(nullptr, imageReference)};

    EXPECT_TRUE(info->getDevExperience());
    EXPECT_TRUE(info->getNvidiaOpen());
    EXPECT_EQ(info->getUpdateStream(), ImageVariantInfo::UpdateStream::stableDaily);
}

TEST(ImageVariantTest, AsImageNameAndTag_Basic) {
    std::unique_ptr<ImageVariantInfo> info{new ImageVariantInfo(nullptr, false, false, ImageVariantInfo::UpdateStream::stableWeekly, false)};

    EXPECT_EQ(info->asImageNameAndTag(), "aurora:stable"_L1);
}

TEST(ImageVariantTest, AsImageNameAndTag_DevExp) {
    std::unique_ptr<ImageVariantInfo> info{new ImageVariantInfo(nullptr, false, true, ImageVariantInfo::UpdateStream::latest, false)};

    EXPECT_EQ(info->asImageNameAndTag(), "aurora-dx:latest"_L1);
}

TEST(ImageVariantTest, AsImageNameAndTag_NvidiaOpen) {
    std::unique_ptr<ImageVariantInfo> info{new ImageVariantInfo(nullptr, true, true, ImageVariantInfo::UpdateStream::stableDaily, false)};

    EXPECT_EQ(info->asImageNameAndTag(), "aurora-dx-nvidia-open:stable-daily"_L1);
}