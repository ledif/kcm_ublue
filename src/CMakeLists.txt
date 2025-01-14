# SPDX-License-Identifier: BSD-2-Clause

kcmutils_add_qml_kcm(kcm_ublue)
target_sources(kcm_ublue PRIVATE
    imagevariant.h
    imagevariant.cpp
    rebasemanager.h
    rebasemanager.cpp
    rebasefilewatcher.h
    rebasefilewatcher.cpp
    rebaseservice.h
    rebaseservice.cpp
    ubluesettings.cpp
    ubluesettings.h
    updateservice.cpp
    updateservice.h
)

target_link_libraries(kcm_ublue PRIVATE
    Qt::Quick
    Qt::DBus
    Qt::Core
    KF6::CoreAddons
    KF6::KCMUtils
    KF6::I18n
    KF6::WidgetsAddons
)

