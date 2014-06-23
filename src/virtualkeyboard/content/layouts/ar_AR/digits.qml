/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Virtual Keyboard add-on for Qt Enterprise.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Enterprise.VirtualKeyboard 1.1

KeyboardLayout {
    inputMethod: PlainInputMethod {}
    inputMode: InputEngine.Numeric

    KeyboardColumn {
        Layout.fillWidth: false
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignHCenter
        Layout.preferredWidth: height
        KeyboardRow {
            Key {
                key: 0x0667
                text: "\u0667"
                alternativeKeys: "\u06677"
            }
            Key {
                key: 0x0668
                text: "\u0668"
                alternativeKeys: "\u06688"
            }
            Key {
                key: 0x0669
                text: "\u0669"
                alternativeKeys: "\u06699"
            }
            BackspaceKey {}
        }
        KeyboardRow {
            Key {
                key: 0x0664
                text: "\u0664"
                alternativeKeys: "\u06644"
            }
            Key {
                key: 0x0665
                text: "\u0665"
                alternativeKeys: "\u06655"
            }
            Key {
                key: 0x0666
                text: "\u0666"
                alternativeKeys: "\u06666"
            }
            Key {
                text: " "
                displayText: "_"
                repeat: true
                showPreview: false
                key: Qt.Key_Space
            }
        }
        KeyboardRow {
            Key {
                key: 0x0661
                text: "\u0661"
                alternativeKeys: "\u06611"
            }
            Key {
                key: 0x0662
                text: "\u0662"
                alternativeKeys: "\u06622"
            }
            Key {
                key: 0x0663
                text: "\u0663"
                alternativeKeys: "\u06633"
            }
            HideKeyboardKey {}
        }
        KeyboardRow {
            ChangeLanguageKey {
                customLayoutsOnly: true
            }
            Key {
                key: 0x0660
                text: "\u0660"
                alternativeKeys: "\u06600"
            }
            Key {
                key: Qt.Key_Comma
                text: "\u066B"
                alternativeKeys: "\u066B,."
            }
            EnterKey {}
        }
    }
}