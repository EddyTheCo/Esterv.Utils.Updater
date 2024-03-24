import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esterv.Utils.Updater



Item
{
    id:control
    visible: Updater.state&&Updater.hasUpdate

    RowLayout
    {
        anchors.fill: parent
        Label
        {
            id:info
            text:(Updater.state===1)?qsTr("There are updates"):((Updater.state===3)?qsTr("Installing updates"):qsTr("Updates are installed"))
        }

        ColumnLayout
        {
            Layout.alignment: Qt.AlignVCenter|Qt.AlignHCenter
            Button
            {
                id:action
                text:(Updater.state===4)?qsTr("Restart"):qsTr("Update")
                enabled: (Updater.state===4)||(Updater.state===1)
                onClicked:
                {
                    if(Updater.state===1)
                    {
                        Updater.update();
                    }
                    if(Updater.state===4)
                    {
                        Updater.restart();
                    }
                }
            }
            Button
            {
                id:close
                text:qsTr("Close")
                onClicked: control.visible=false;
            }
        }
    }

}


