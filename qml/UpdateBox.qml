import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esterv.Utils.Updater


RowLayout
{
    id:control
    Label
    {
        id:info
        Layout.alignment: Qt.AlignLeft||Qt.AlignVCenter
        text:(Updater.state===1)?qsTr("There are updates"):((Updater.state===3)?qsTr("Installing updates"):qsTr("Updates are installed"))
    }

    Button
    {
        id:action
        text:(Updater.state===4)?qsTr("Restart"):qsTr("Update")
        enabled: (Updater.state===4)||(Updater.state===1)
        Layout.alignment: Qt.AlignRight||Qt.AlignVCenter
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


}



