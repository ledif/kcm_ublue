# SPDX-License-Identifier: BSD-2-Clause

kcmutils_add_qml_kcm(kcm_ublue)
target_sources(kcm_ublue PRIVATE
   ubluesettings.cpp
   ubluesettings.h
   imagevariant.h
   imagevariant.cpp
)

target_link_libraries(kcm_ublue PRIVATE
    Qt::Quick
    KF6::CoreAddons
    KF6::KCMUtils
    KF6::I18n
)

