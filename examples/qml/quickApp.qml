import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Esterv.Utils.Updater

ApplicationWindow {
    id: window
    visible: true

    UpdateBox {}
    Label {
        anchors.centerIn: parent
        text: Updater.state
    }
}
