# SPDX-License-Identifier: BSD-2-Clause

kcmutils_add_qml_kcm(kcm_ublue)
target_sources(kcm_ublue PRIVATE
   imagevariant.h
   imagevariant.cpp
   updateservice.cpp
   updateservice.h
   ubluesettings.cpp
   ubluesettings.h
)

target_link_libraries(kcm_ublue PRIVATE
    Qt::Quick
    Qt::DBus
    KF6::CoreAddons
    KF6::KCMUtils
    KF6::I18n
)

