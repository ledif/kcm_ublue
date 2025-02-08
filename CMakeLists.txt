# SPDX-FileCopyrightText: Year Author <email@company.com>
#
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16)

set(QT_MIN_VERSION "6.8.0")
set(KF6_MIN_VERSION "6.0.0")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(kcm_ublue)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED Core Quick Gui Qml)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    Config
    CoreAddons
    KCMUtils
    WidgetsAddons
    I18n
)

add_subdirectory(src)

enable_testing()

find_package(GTest REQUIRED)
add_executable(test_imagevariant tests/test_imagevariant.cpp src/imagevariant.cpp)
target_include_directories(test_imagevariant PRIVATE src)

target_link_libraries(test_imagevariant
    PRIVATE
    Qt::Core
    GTest::GTest
    GTest::Main
)

add_test(NAME ImageVariantTest COMMAND test_imagevariant)
