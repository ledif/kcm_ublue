import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils

KCMUtils.SimpleKCM {
    Layout.fillWidth: true

    ColumnLayout {
        anchors.fill: parent
        Layout.fillWidth: true

        RowLayout {
            Layout.fillWidth: true

            Controls.TabBar {
                id: tabBar
                Layout.fillWidth: true

                Controls.TabButton {
                    text: i18nc("@title:tab", "Updates and Rebase")
                    Layout.fillWidth: true
                }
                Controls.TabButton {
                    text: i18nc("@title:tab", "Deployments")
                    Layout.fillWidth: true
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            currentIndex: tabBar.currentIndex
            //anchors.fill: parent

            Item {
                //anchors.fill: parent
                Layout.fillWidth: true

                Loader {
                    id: rebaseTab
                    anchors.fill: parent
                    Layout.fillWidth: true
                    source: "rebase.qml"
                }


            }

            Item {
                Layout.fillWidth: true
                Loader {
                    id: deploymentsTab
                    anchors.fill: parent
                    Layout.fillWidth: true
                    source: "deployments.qml"
                }
            }
        }
    }
}
