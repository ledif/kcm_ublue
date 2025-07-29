import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami

Item {
    id: deploymentViewItem
    Layout.fillWidth: true
    anchors.fill: parent

    Component.onCompleted: {
        console.log("Deployments view loaded, model count:", kcm.deploymentModel.rowCount())
        // Use a timer to ensure the view is fully set up before updating
        updateTimer.start()
    }

    Timer {
        id: updateTimer
        interval: 100
        repeat: false
        onTriggered: {
            console.log("Timer triggered, updating deployment list")
            kcm.deploymentModel.updateDeploymentList()
            console.log("After timer update, model count:", kcm.deploymentModel.rowCount())
            
            // Try a more aggressive refresh
            Qt.callLater(function() {
                console.log("CallLater: forcing model refresh")
                deploymentList.model = null
                deploymentList.model = kcm.deploymentModel
                console.log("CallLater: model reassigned, count should be:", kcm.deploymentModel.rowCount())
            })
        }
    }

    Connections {
        target: kcm.deploymentModel
        function onModelReset() {
            console.log("Model reset signal received, count:", kcm.deploymentModel.rowCount())
        }
        function onDeploymentListUpdated() {
            console.log("Deployment list updated signal received, count:", kcm.deploymentModel.rowCount())
            // Force ListView to refresh by reassigning the model
            deploymentList.model = null
            deploymentList.model = kcm.deploymentModel
        }
    }

    ListView {
        id: deploymentList
        anchors.fill: parent
        currentIndex: -1

        model: kcm.deploymentModel

        Component.onCompleted: {
            console.log("ListView created, model count:", count)
        }

        onCountChanged: {
            console.log("ListView count changed to:", count)
            console.log("Model row count:", kcm.deploymentModel.rowCount())
        }

        onModelChanged: {
            console.log("ListView model changed, new count:", count)
        }

        delegate: Controls.ItemDelegate {
            id: listItem2
            
            width: deploymentList.width

            Component.onCompleted: {
                console.log("Delegate created for index:", index, "imageName:", model.imageName)
            }

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
                    onClicked: {
                        kcm.deploymentModel.pinOrUnpinDeployment(index);
                    }

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

    // Show a message when there are no deployments
    Controls.Label {
        anchors.centerIn: parent
        text: "No deployments found"
        visible: deploymentList.count === 0
        color: Kirigami.Theme.disabledTextColor
    }
}
