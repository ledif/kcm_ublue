#include <gtest/gtest.h>
#include "imagevariant.h"

using namespace Qt::Literals::StringLiterals;

TEST(ImageVariantTest, ParseImageName_DevExp) {
    QString imageName = "aurora-dx"_L1;
    QString imageStream = "latest"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageNameAndTag(nullptr, imageName, imageStream)};

    EXPECT_TRUE(info->getDevExperience());
}
TEST(ImageVariantTest, ParseImageName_Standard) {
    QString imageName = "aurora"_L1;
    QString imageStream = "stable"_L1;

    std::unique_ptr<ImageVariantInfo> info{ImageVariantInfo::parseFromImageNameAndTag(nullptr, imageName, imageStream)};

    EXPECT_FALSE(info->getDevExperience());
}