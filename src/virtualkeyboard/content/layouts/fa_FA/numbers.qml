/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
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

    KeyboardRow {
        Layout.fillWidth: false
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignHCenter
        KeyboardColumn {
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: parent.height / 4 * 3
            KeyboardRow {
                Key {
                    key: Qt.Key_ParenLeft
                    text: "("
                }
                Key {
                    key: Qt.Key_ParenRight
                    text: ")"
                }
                Key {
                    key: Qt.Key_Period
                    text: "."
                }
            }
            KeyboardRow {
                Key {
                    key: Qt.Key_division
                    text: "\u00F7"
                }
                Key {
                    key: Qt.Key_multiply
                    text: "\u00D7"
                }
                Key {
                    key: Qt.Key_Plus
                    text: "+"
                }
            }
            KeyboardRow {
                Key {
                    key: Qt.Key_AsciiCircum
                    text: "^"
                }
                Key {
                    key: Qt.Key_Slash
                    text: "/"
                }
                Key {
                    key: Qt.Key_Minus
                    text: "-"
                }
            }
            KeyboardRow {
                Key {
                    key: 0x221A
                    text: "√"
                }
                Key {
                    key: Qt.Key_Percent
                    text: "%"
                }
                Key {
                    key: Qt.Key_Asterisk
                    text: "*"
                }
            }
        }
        KeyboardColumn {
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: parent.height / 8
            KeyboardRow {
                FillerKey {}
            }
        }
        KeyboardColumn {
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: parent.height
            KeyboardRow {
                Key {
                    text: "\u06F7"
                    alternativeKeys: "\u06F77"
                }
                Key {
                    text: "\u06F8"
                    alternativeKeys: "\u06F88"
                }
                Key {
                    text: "\u06F9"
                    alternativeKeys: "\u06F99"
                }
                BackspaceKey {}
            }
            KeyboardRow {
                Key {
                    text: "\u06F4"
                    alternativeKeys: "\u06F44"
                }
                Key {
                    text: "\u06F5"
                    alternativeKeys: "\u06F55"
                }
                Key {
                    text: "\u06F6"
                    alternativeKeys: "\u06F66"
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
                    text: "\u06F1"
                    alternativeKeys: "\u06F11"
                }
                Key {
                    text: "\u06F2"
                    alternativeKeys: "\u06F22"
                }
                Key {
                    text: "\u06F3"
                    alternativeKeys: "\u06F33"
                }
                HideKeyboardKey {}
            }
            KeyboardRow {
                ChangeLanguageKey {
                    customLayoutsOnly: true
                }
                Key {
                    text: "\u06F0"
                    alternativeKeys: "\u06F00"
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
}