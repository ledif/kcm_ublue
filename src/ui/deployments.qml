import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Item {
    Layout.fillWidth: true
    ListView {
        id: deploymentList
        interactive: true
        currentIndex: -1

        Layout.fillWidth: true
        height: 500  // Temporary debugging value

        model: kcm.deploymentModel


        delegate: Controls.ItemDelegate {
            id: listItem2

            //width: deploymentList.width

            // There's no need for a list item to ever be selected
            //down: false
            //highlighted: false
            //hoverEnabled: false
            // ... and because of that, use alternating backgrounds to visually
            // connect list items' left and right side content elements
            Kirigami.Theme.useAlternateBackgroundColor: true

            contentItem: RowLayout {
                spacing: Kirigami.Units.smallSpacing

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

                    Layout.fillWidth: true
                }

                Controls.Label {
                    text: model.version
                    textFormat: Text.PlainText

                    Layout.fillWidth: true
                }

                Controls.ToolButton {
                    icon.name: model.isPinned ? "window-unpin" : "window-pin"

                    onClicked: kcm.filteredModel.pinOrUnpinDeployment(index)

                    Controls.ToolTip {
                        text: i18nc("Pin deployment in rpm-ostree", (model.isPinned ? "Unpin" : "Pin") + " deployment")
                    }
                }

                Controls.ToolButton {
                    enabled: !model.isDeployed

                    icon.name: "view-refresh"

                    onClicked: kcm.filteredModel.rollbackToDeployment(index)

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