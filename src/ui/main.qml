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
  ColumnLayout {
    Kirigami.InlineMessage {
        Layout.fillWidth: true
        text: "The ASUS and Surface streams have been discontinued. Please migrate to the hardware enablement stream."
        type: Kirigami.MessageType.Warning
        visible: kcm.imageVariant.isDeprecatedStream
    }

    Kirigami.InlineMessage {
        id: rebaseInProgressMessage
        Layout.fillWidth: true
        text: "placeholder"
        visible: kcm.rebaseInProgress


        actions: [
            Kirigami.Action {
                id: rebaseDetailsButton
                enabled: true
                text: qsTr("Details")
                icon.name: "view-list-details"
                onTriggered: {
                    kcm.onRebaseDetailsButtonPressed()
                }
            },
            Kirigami.Action {
                id: rebaseCancelButton
                enabled: true
                text: qsTr("Cancel")
                icon.name: "cancel"
                onTriggered: kcm.onRebaseCancelButtonPressed()
            }
        ]

        Connections {
            target: kcm
            onRebaseServiceChanged: {
              console.log("onRebaseServiceChanged", kcm.rebase.status)
              
              if (kcm.rebase.status == 0 || kcm.rebase.status == 1) {
                rebaseInProgressMessage.text = "Rebase to " + kcm.rebase.prettyName + " in progress"
                rebaseInProgressMessage.type = Kirigami.MessageType.Information
              } else if (kcm.rebase.status == 2) {
                rebaseInProgressMessage.text = "Rebase to " + kcm.rebase.prettyName + " failed"
                rebaseInProgressMessage.type = Kirigami.MessageType.Error
              } else if (kcm.rebase.status == 3) {
                rebaseInProgressMessage.text = "Rebase successful. New changes available on reboot."
                rebaseInProgressMessage.type = Kirigami.MessageType.Positive
              }
              rebaseCancelButton.visible = kcm.rebase.status == 0 || kcm.rebase.status == 1
              rebaseDetailsButton.visible = true
              rebaseInProgressMessage.visible = true
            }
        }
    }


    /*Kirigami.InlineMessage {
        Layout.fillWidth: true
        text: "The hardware enablement image can only be on the latest stream. See "
        type: Kirigami.MessageType.Warning
        visible: true
    }*/

    Kirigami.FormLayout {
        //anchors.fill: parent
        id: rootLayout

        Item {
            height: Kirigami.Units.smallSpacing
        }

        Controls.ButtonGroup {
          id: updateStreamGroup
        }

        Controls.CheckBox {
          id: updatesCheckBox
          Kirigami.FormData.label: i18nc("@option:check", "System updates:")
          text: i18n("Enabled")
          checked: kcm.updatesEnabled
          onToggled: kcm.updatesEnabled = updatesCheckBox.checked
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
            Kirigami.FormData.label: i18n("Update stream:")

            Controls.RadioButton {
                id: updateStreamBtn
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

        signal hweChanged
        onHweChanged: {
          console.log("onHWEChanged")
          nvidiaCombobox.enabled = nvidiaCheckbox.checked

          kcm.imageVariant.hweFlags.hwe = hweCheckbox.checked
          kcm.imageVariant.hweFlags.nvidiaOpen = false
          kcm.imageVariant.hweFlags.nvidia = false

          if (nvidiaCheckbox.checked) {
            if (nvidiaCombobox.currentValue == "Open")
              kcm.imageVariant.hweFlags.nvidiaOpen = true
            else if (nvidiaCombobox.currentValue == "Closed")
              kcm.imageVariant.hweFlags.nvidia = true
          }

          console.log("new HWEFlags", kcm.imageVariant.hweFlags.hwe, kcm.imageVariant.hweFlags.nvidia, kcm.imageVariant.hweFlags.nvidiaOpen)
        }

        Controls.CheckBox {
            id: hweCheckbox
            Kirigami.FormData.label: i18nc("@option:check", "Hardware enablement:")
            text: i18nc("@option:check", "Enabled")
            checked: kcm.imageVariant.hweFlags.hwe
            onToggled: {
              rootLayout.hweChanged()
            }
        }

        RowLayout {
            spacing: Kirigami.Units.mediumSpacing

            Controls.CheckBox {
              id: nvidiaCheckbox
              text: i18nc("@option:check", "NVIDIA")
              checked: kcm.imageVariant.hweFlags.nvidia || kcm.imageVariant.hweFlags.nvidiaOpen
              onToggled: {
                rootLayout.hweChanged()
              }
            }

            Controls.ComboBox {
              id: nvidiaCombobox
              enabled: nvidiaCheckbox.checked
              model: ["Open", "Closed"]
              onActivated: rootLayout.hweChanged()
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
            id: dxCheckbox
            Kirigami.FormData.label: i18nc("@option:check", "Developer experience:")
            text: i18n("Enabled")
            checked: kcm.imageVariant.devExperience
            onToggled: kcm.imageVariant.devExperience = dxCheckbox.checked
        }
    }
  }
}


