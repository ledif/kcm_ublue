import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Item {
    Layout.fillWidth: true
    anchors.fill: parent

    ListView {
        id: deploymentList
        interactive: true
        currentIndex: -1

        Layout.fillWidth: true
        height: 500  // Temporary debugging value

        model: kcm.deploymentModel

        delegate: Controls.ItemDelegate {
            id: listItem2

            Kirigami.Theme.useAlternateBackgroundColor: true

            contentItem: RowLayout {
                spacing: Kirigami.Units.smallSpacing
                Layout.fillWidth: true // Ensure RowLayout takes full width

                Kirigami.Icon {
                    source: model.isDeployed ? "draw-circle" : ""
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                }

                Controls.Label {
                    text: model.imageName
                    textFormat: Text.PlainText
                }

                Controls.Label {
                    text: model.imageTag
                    textFormat: Text.PlainText
                    Layout.fillWidth: true  // Make this label fill the available width
                }

                Controls.Label {
                    text: model.version
                    textFormat: Text.PlainText
                    Layout.fillWidth: true  // Make this label fill the available width
                }

                Controls.ToolButton {
                    enabled: model.isPinnable
                    icon.name: model.isPinned ? "window-unpin" : "window-pin"
                    onClicked: kcm.deploymentModel.pinOrUnpinDeployment(index)

                    Controls.ToolTip {
                        text: i18nc("Pin deployment in rpm-ostree", (model.isPinned ? "Unpin" : "Pin") + " deployment")
                    }
                }

                Controls.ToolButton {
                    enabled: model.isRollbackTarget
                    icon.name: "view-refresh"
                    onClicked: kcm.deploymentModel.rollbackToDeployment(index)

                    Controls.ToolTip {
                        text: i18nc("Remove the list item for this filesystem path", "Rollback to deployment")
                    }
                }
            }
        }

        header: Kirigami.InlineViewHeader {
            width: deploymentList.width
            text: i18nc("@title:table", "Deployments")
            actions: [
                Kirigami.Action {
                    text: i18nc("@action:button", "Unpin all")
                    icon.name: "list-remove-symbolic"
                }
            ]
        }
    }
}