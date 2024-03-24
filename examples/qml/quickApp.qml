
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esterv.Utils.Updater

ApplicationWindow {
    visible: true
    id:window

    UpdateBox
    {
    }
    Label
    {
        anchors.centerIn: parent
        text:Updater.state
    }

}
