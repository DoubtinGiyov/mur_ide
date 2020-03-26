import QtQuick 2.9

Item {
    id: root;

    width: row.width + 24;
    height: label.height + 6;
    property alias label: label;
    property bool checked: false;
    property var from;
    property string bind;

    signal clicked;

    Row {
        id: row;
        anchors.centerIn: parent;
        spacing: 4;

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter;
            width: 16;
            height: 16;
            color: ma.pressed ? "#181A1F" : ma.containsMouse ? "#363C46" : "#333842"
            border.color: "#181A1F";
            radius: 2;

            Icon {
                anchors.centerIn: parent;

                color: label.color;
                visible: root.checked;
                icon: icons.fa_check;
            }
        }

        UiLabel {
            id: label;
            anchors.verticalCenter: parent.verticalCenter;
            font.weight: Font.Medium;
            color: enabled ? ma.containsMouse ? "#fff" : "#9DA5B4" : "#6E7582";
        }
    }

    MouseArea {
        id: ma;
        anchors.fill: parent;
        propagateComposedEvents: true;
        hoverEnabled: true;

        onClicked: {
            root.clicked();
            if (root.from) {
                root.from[root.bind] = !root.from[root.bind];
            }
        }
    }

    Component.onCompleted: {
        root.checked = Qt.binding(function() { return root.from[root.bind]; });
    }
}
