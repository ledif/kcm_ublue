import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.kirigami as Kirigami

Item {
    Layout.fillWidth: true
    anchors.horizontalCenter: parent.horizontalCenter
    ColumnLayout {
        Layout.fillWidth: true
        anchors.fill: parent

        Kirigami.InlineMessage {
            Layout.fillWidth: true
            text: "The ASUS and Surface images are deprecated and their functionality has been in the har. Please migrate to the hardware enablement stream."
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
                        kcm.onRebaseDetailsButtonPressed();
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
                function onRebaseServiceChanged() {
                    console.log("onRebaseServiceChanged", kcm.rebase.status);

                    if (kcm.rebase.status == 0 || kcm.rebase.status == 1) {
                        rebaseInProgressMessage.text = "Rebase to " + kcm.rebase.prettyName + " in progress";
                        rebaseInProgressMessage.type = Kirigami.MessageType.Information;
                    } else if (kcm.rebase.status == 2) {
                        rebaseInProgressMessage.text = "Rebase to " + kcm.rebase.prettyName + " failed";
                        rebaseInProgressMessage.type = Kirigami.MessageType.Error;
                    } else if (kcm.rebase.status == 3) {
                        rebaseInProgressMessage.text = "Rebase successful. New changes available on reboot.";
                        rebaseInProgressMessage.type = Kirigami.MessageType.Positive;
                    }
                    rebaseCancelButton.visible = kcm.rebase.status == 0 || kcm.rebase.status == 1;
                    rebaseDetailsButton.visible = true;
                    rebaseInProgressMessage.visible = true;
                }
            }
        }

        Kirigami.FormLayout {
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillHeight: true
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
            onUpdateStreamChanged: latest => {
                console.log("onUpdateStreamChanged", latest);
                stableFrequency.enabled = !latest;
                if (latest) {
                    kcm.imageVariant.updateStream = 3;
                } else {
                    kcm.imageVariant.updateStream = 0;
                    if (stableFrequency.currentValue == "Weekly") {
                        kcm.imageVariant.updateStream = 1;
                    } else if (stableFrequency.currentValue == "Daily") {
                        kcm.imageVariant.updateStream = 2;
                    }
                }
                console.log("new updateStream", kcm.imageVariant.updateStream);
            }

            Item {
                Kirigami.FormData.label: "Rebase"
                Kirigami.FormData.isSection: true
            }

            RowLayout {
                spacing: Kirigami.Units.smallSpacing
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

                    model: ["Weekly", "Daily"]
                    onActivated: rootLayout.updateStreamChanged(false)
                    Component.onCompleted: {
                        if (kcm.imageVariant.updateStream == 1)
                            currentIndex = 0;
                        else if (kcm.imageVariant.updateStream == 2)
                            currentIndex = 1;
                    }
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
                console.log("onHWEChanged");
                kcm.imageVariant.hweFlags.hwe = hweCheckbox.checked;
                kcm.imageVariant.hweFlags.nvidiaOpen = false;
                kcm.imageVariant.hweFlags.nvidia = false;

                if (nvidiaCombobox.currentIndex == 0)
                    kcm.imageVariant.hweFlags.nvidiaOpen = true;
                else if (nvidiaCombobox.currentIndex == 1)
                    kcm.imageVariant.hweFlags.nvidia = true;

                console.log("new HWEFlags", kcm.imageVariant.hweFlags.hwe, kcm.imageVariant.hweFlags.nvidia, kcm.imageVariant.hweFlags.nvidiaOpen);
            }
            RowLayout {
                spacing: Kirigami.Units.mediumSpacing
                Kirigami.FormData.label: i18n("GPU driver:")

                Controls.ComboBox {
                    id: nvidiaCombobox
                    model: ["NVIDIA", "NVIDIA (Legacy)", "Intel / AMD"]
                    Component.onCompleted: {
                        if (kcm.imageVariant.hweFlags.nvidiaOpen)
                            currentIndex = 0;
                        else if (kcm.imageVariant.hweFlags.nvidia)
                            currentIndex = 1;
                        else
                            currentIndex = 2;
                    }
                    onActivated: rootLayout.hweChanged()
                }

                Kirigami.ContextualHelpButton {
                    toolTipText: xi18nc("@info", "NVIDIA drivers are for modern NVIDIA GPUs including RTX series and GTX 16xx series+. NVIDIA (Legacy) is for older GPUs.")
                }
            }

            RowLayout {
                spacing: Kirigami.Units.mediumSpacing
                Kirigami.FormData.label: i18n("Hardware enablement:")

                Controls.CheckBox {
                    id: hweCheckbox
                    Kirigami.FormData.label: i18nc("@option:check", "Additional hardware support:")
                    text: i18nc("@option:check", "Handheld compatibility")
                    checked: kcm.imageVariant.hweFlags.hwe
                    onToggled: {
                        rootLayout.hweChanged();
                    }
                }
                Kirigami.ContextualHelpButton {
                    toolTipText: xi18nc("@info", "Use Bazzite's kernel which includes additional support for hardware from various vendors, including ASUS, Microsoft Surface, Steam Deck and more.")
                }
            }

            Kirigami.Separator {
                Kirigami.FormData.isSection: true
            }

            Item {
                height: Kirigami.Units.smallSpacing
            }
            RowLayout {
                Kirigami.FormData.label: i18n("Developer experience:")

                Controls.CheckBox {
                    id: dxCheckbox
                    Kirigami.FormData.label: i18nc("@option:check", "Developer experience:")
                    text: i18n("Enabled")
                    checked: kcm.imageVariant.devExperience
                    onToggled: kcm.imageVariant.devExperience = dxCheckbox.checked
                }

                Kirigami.ContextualHelpButton {
                    toolTipText: xi18nc("@info", "Provides popular developer tooling including Virtual Machine Manager, Docker and VS Code.")
                }
            }
        }
    }
}

