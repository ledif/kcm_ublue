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

        Controls.RadioButton {
            Kirigami.FormData.label: i18n("Update stream:")
            text: i18n("Stable")

            Controls.ButtonGroup.group: updateStreamGroup
            checked: !kcm.updatesSettings.useUnattendedUpdates
        }

        RowLayout {
            spacing: Kirigami.Units.smallSpacing

            Controls.RadioButton {
                id: automaticallyRadio
                text: i18n("Latest")

                Controls.ButtonGroup.group: updateStreamGroup
                checked: kcm.updatesSettings.useUnattendedUpdates
            }

            Kirigami.ContextualHelpButton {
                toolTipText: xi18nc("@info", "Software updates will be downloaded automatically when they become available. Updates for applications will be installed immediately, while system updates will be installed the next time the computer is restarted.")
            }
        }


        Controls.CheckBox {
            Kirigami.FormData.label: i18nc("@option:check", "System updates:")
            text: i18n("Enabled")
            checked: !settings.isPrivate
            onToggled: settings.isPrivate = !checked
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }


        Controls.CheckBox {
            Kirigami.FormData.label: i18nc("@option:check", "Hardware enablement:")
            text: i18nc("@option:check", "NVIDIA")
            checked: !settings.inhibitScreen
            onToggled: settings.inhibitScreen = !checked
        }

        Controls.CheckBox {
            text: i18nc("@option:check", "NVIDIA (open)")
            checked: !settings.inhibitScreen
            onToggled: settings.inhibitScreen = !checked
        }

        Controls.CheckBox {
            text: i18nc("@option:check", "ASUS")
            checked: !settings.inhibitScreen
            onToggled: settings.inhibitScreen = !checked
        }

        Controls.CheckBox {
            text: i18nc("@option:check", "Surface")
            checked: !settings.inhibitScreen
            onToggled: settings.inhibitScreen = !checked
        }

    }
}


