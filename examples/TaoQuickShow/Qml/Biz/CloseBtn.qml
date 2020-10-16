import QtQuick 2.9

import TaoQuick 1.0

CusButton_Image {
    width: 20
    height: 20
    tipText: qsTr("close")  + trans.transString
    btnImgUrl: imgPath + (hovered || pressed ? "Window/close_white.png" : "Window/close_gray.png")
}