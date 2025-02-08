#include <gtest/gtest.h>
#include "imagevariant.h"

using namespace Qt::Literals::StringLiterals;

TEST(ImageVariantTest, ParseImageName_DevExp) {
    QString imageReference = "ostree-image-signed:docker://ghcr.io/ublue-os/aurora-dx:latest"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageReference(nullptr, imageReference)};

    EXPECT_TRUE(info->getDevExperience());
    EXPECT_FALSE(info->getHWEFlags()->hwe);
    EXPECT_FALSE(info->getHWEFlags()->nvidiaOpen);
    EXPECT_FALSE(info->getHWEFlags()->nvidia);
    EXPECT_EQ(info->getUpdateStream(), ImageVariantInfo::UpdateStream::latest);}

TEST(ImageVariantTest, ParseImageName_Standard) {
    QString imageReference = "ostree-image-signed:docker://ghcr.io/ublue-os/aurora:stable"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageReference(nullptr, imageReference)};

    EXPECT_FALSE(info->getDevExperience());
    EXPECT_FALSE(info->getHWEFlags()->hwe);
    EXPECT_FALSE(info->getHWEFlags()->nvidiaOpen);
    EXPECT_FALSE(info->getHWEFlags()->nvidia);
    EXPECT_EQ(info->getUpdateStream(), ImageVariantInfo::UpdateStream::stableWeekly);
}

TEST(ImageVariantTest, ParseImageName_Everything) {
    QString imageReference = "ostree-image-signed:docker://ghcr.io/ublue-os/aurora-dx-hwe-nvidia-open:stable-daily"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageReference(nullptr, imageReference)};

    EXPECT_TRUE(info->getDevExperience());
    EXPECT_TRUE(info->getHWEFlags()->hwe);
    EXPECT_TRUE(info->getHWEFlags()->nvidiaOpen);
    EXPECT_FALSE(info->getHWEFlags()->nvidia);
    EXPECT_EQ(info->getUpdateStream(), ImageVariantInfo::UpdateStream::stableDaily);
}