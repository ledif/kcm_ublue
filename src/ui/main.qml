import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils

KCMUtils.SimpleKCM {
    ColumnLayout {
        anchors.fill: parent

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
            anchors.horizontalCenter: parent.horizontalCenter

            Item {
                anchors.centerIn: parent
                Loader {
                    
                    anchors.fill: parent
                    id: rebaseTab
                    source: "rebase.qml"
                }
            }

            Item {
                Layout.fillWidth: true
                Loader {
                    Layout.fillWidth: true
                    id: deploymentsTab
                    source: "deployments.qml"
                }
            }
        }
    }
}
