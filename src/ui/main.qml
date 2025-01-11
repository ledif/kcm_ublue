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
        id: rootLayout

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
            onToggled: () => kcm.updatesEnabled = checked
        }

        signal updateStreamChanged(bool latest)
        onUpdateStreamChanged: (latest) => {
          console.log("onUpdateStreamChanged", latest)
          stableFrequency.enabled = !latest
          if (latest) {
            kcm.imageVariant.updateStream = 3

          } else {
            kcm.imageVariant.updateStream = 0
            if (stableFrequency.currentValue == "Weekly") {
              kcm.imageVariant.updateStream = 1
            }
            else if (stableFrequency.currentValue == "Daily") {
              kcm.imageVariant.updateStream = 2
            }
          }
          console.log("new updateStream", kcm.imageVariant.updateStream)
        }

        RowLayout {
            spacing: Kirigami.Units.mediumSpacing

            Controls.RadioButton {
                id: updateStreamBtn
                Kirigami.FormData.label: i18n("Update stream:")
                text: i18n("Stable")

                Controls.ButtonGroup.group: updateStreamGroup
                checked: kcm.imageVariant.updateStream == 1 || kcm.imageVariant.updateStream == 2
                onToggled: rootLayout.updateStreamChanged(false)
            }

            Controls.ComboBox {
                id: stableFrequency
                enabled: kcm.imageVariant.updateStream == 1 || kcm.imageVariant.updateStream == 2

                model: [ "Weekly", "Daily" ]
                onActivated: rootLayout.updateStreamChanged(false)
            }
        }

        RowLayout {
            spacing: Kirigami.Units.smallSpacing

            Controls.RadioButton {
                id: automaticallyRadio
                text: i18n("Latest")

                Controls.ButtonGroup.group: updateStreamGroup
                checked: kcm.imageVariant.updateStream == 3
                onToggled: rootLayout.updateStreamChanged(true)
            }

            Kirigami.ContextualHelpButton {
                toolTipText: xi18nc("@info", "For users who want the very latest Fedora has to offer. Daily updates with a bleeding-edge kernel.")
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
            text: i18nc("@option:check", "Enabled")
            checked: kcm.imageVariant.hweFlags.hwe
            onToggled: kcm.imageVariant.hweFlags.hwe = !checked
        }

        RowLayout {
            spacing: Kirigami.Units.mediumSpacing

            Controls.CheckBox {
                text: i18nc("@option:check", "NVIDIA")
                checked: kcm.imageVariant.hweFlags.nvidia || kcm.imageVariant.hweFlags.nvidiaOpen
                onToggled: kcm.imageVariant.hweFlags.nvidia = !checked
            }

            Controls.ComboBox {
                model: ["Open", "Closed"]
            }

            Kirigami.ContextualHelpButton {
                toolTipText: xi18nc("@info", "Open drivers are for modern NVIDIA GPUs (RTX series and GTX 16xx series+). Closed drivers are for legacy NVIDIA GPUs.")
            }
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


