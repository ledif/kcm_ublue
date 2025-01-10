/**
 * SPDX-FileCopyrightText: Year Author <author@domain.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */


import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.iconthemes as KIconThemes

import org.kde.kquickcontrols as KQuickControls
import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils


KCMUtils.SimpleKCM {

    Kirigami.FormLayout {
        anchors.fill: parent

        Item {
            height: Kirigami.Units.smallSpacing
        }

        Controls.ButtonGroup {
          id: updateStreamGroup
        }

        Controls.CheckBox {
            Kirigami.FormData.label: i18nc("@option:check", "System updates:")
            text: i18n("Enabled")
            checked: kcm.updatesEnabled
            onToggled: kcm.updatesEnabled = !checked
        }


        Controls.RadioButton {
            Kirigami.FormData.label: i18n("Update stream:")
            text: i18n("Stable")

            Controls.ButtonGroup.group: updateStreamGroup
            checked: kcm.imageVariant.updateStream == 1 || kcm.imageVariant.updateStream == 2
        }

        Controls.ComboBox {
            model: ["Weekly", "Daily"]
        }

        RowLayout {
            spacing: Kirigami.Units.smallSpacing

            Controls.RadioButton {
                id: automaticallyRadio
                text: i18n("Latest")

                Controls.ButtonGroup.group: updateStreamGroup
                checked: kcm.imageVariant.updateStream == 3
            }

            Kirigami.ContextualHelpButton {
                toolTipText: xi18nc("@info", "Software updates will be downloaded automatically when they become available. Updates for applications will be installed immediately, while system updates will be installed the next time the computer is restarted.")
            }
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Item {
            height: Kirigami.Units.smallSpacing
        }

        Controls.CheckBox {
            Kirigami.FormData.label: i18nc("@option:check", "Hardware enablement:")
            text: i18nc("@option:check", "NVIDIA")
            checked: kcm.imageVariant.hweFlags.nvidia || kcm.imageVariant.hweFlags.nvidiaOpen
            onToggled: settings.inhibitScreen = !checked
        }

        Controls.ComboBox {
            model: ["Open", "Closed"]
        }

        Controls.CheckBox {
            text: i18nc("@option:check", "ASUS")
            checked: kcm.imageVariant.hweFlags.asus
            onToggled: settings.inhibitScreen = !checked
        }

        Controls.CheckBox {
            text: i18nc("@option:check", "Surface")
            checked: kcm.imageVariant.hweFlags.surface
            onToggled: settings.inhibitScreen = !checked
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Item {
            height: Kirigami.Units.smallSpacing
        }

        Controls.CheckBox {
            Kirigami.FormData.label: i18nc("@option:check", "Developer experience:")
            text: i18n("Enabled")
            checked: kcm.imageVariant.devExperience
            onToggled: kcm.imageVariant.devExperience = !checked
        }


    }
}


