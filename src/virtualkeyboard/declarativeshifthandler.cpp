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

#include "declarativeshifthandler.h"
#include "declarativeinputcontext.h"
#include <QtCore/private/qobject_p.h>
#include <QSet>

class DeclarativeShiftHandlerPrivate : public QObjectPrivate
{
public:
    DeclarativeShiftHandlerPrivate() :
        QObjectPrivate(),
        inputContext(0),
        sentenceEndingCharacters(QString(".!?") + QChar(Qt::Key_exclamdown) + QChar(Qt::Key_questiondown)),
        autoCapitalizationEnabled(false),
        toggleShiftEnabled(false),
        shiftChanged(false),
        languageFilter(QSet<QLocale::Language>() << QLocale::Arabic << QLocale::Persian << QLocale::Hindi)
    {
    }

    DeclarativeInputContext *inputContext;
    QString sentenceEndingCharacters;
    bool autoCapitalizationEnabled;
    bool toggleShiftEnabled;
    bool shiftChanged;
    QLocale locale;
    const QSet<QLocale::Language> languageFilter;
};

/*!
    \qmltype ShiftHandler
    \inqmlmodule QtQuick.Enterprise.VirtualKeyboard
    \ingroup qtvirtualkeyboard-qml
    \instantiates DeclarativeShiftHandler
    \brief Manages the shift state.
*/

/*!
    \class DeclarativeShiftHandler
    \inmodule InputFramework
    \brief Manages the shift state.
*/

DeclarativeShiftHandler::DeclarativeShiftHandler(DeclarativeInputContext *parent) :
    QObject(*new DeclarativeShiftHandlerPrivate(), parent)
{
    Q_D(DeclarativeShiftHandler);
    d->inputContext = parent;
    if (d->inputContext) {
        connect(d->inputContext, SIGNAL(inputMethodHintsChanged()), SLOT(restart()));
        connect(d->inputContext, SIGNAL(focusEditorChanged()), SLOT(restart()));
        connect(d->inputContext, SIGNAL(preeditTextChanged()), SLOT(autoCapitalize()));
        connect(d->inputContext, SIGNAL(cursorPositionChanged()), SLOT(autoCapitalize()));
        connect(d->inputContext, SIGNAL(shiftChanged()), SLOT(shiftChanged()));
        connect(d->inputContext, SIGNAL(capsLockChanged()), SLOT(shiftChanged()));
        connect(d->inputContext, SIGNAL(localeChanged()), SLOT(localeChanged()));
        d->locale = QLocale(d->inputContext->locale());
    }
}

/*!
    \internal
*/
DeclarativeShiftHandler::~DeclarativeShiftHandler()
{

}

QString DeclarativeShiftHandler::sentenceEndingCharacters() const
{
    Q_D(const DeclarativeShiftHandler);
    return d->sentenceEndingCharacters;
}

void DeclarativeShiftHandler::setSentenceEndingCharacters(const QString &value)
{
    Q_D(DeclarativeShiftHandler);
    if (d->sentenceEndingCharacters != value) {
        d->sentenceEndingCharacters = value;
        autoCapitalize();
        emit sentenceEndingCharactersChanged();
    }
}

bool DeclarativeShiftHandler::autoCapitalizationEnabled() const
{
    Q_D(const DeclarativeShiftHandler);
    return d->autoCapitalizationEnabled;
}

bool DeclarativeShiftHandler::toggleShiftEnabled() const
{
    Q_D(const DeclarativeShiftHandler);
    return d->toggleShiftEnabled;
}

/*!
    \since 1.2

    \qmlmethod void ShiftHandler::toggleShift()

    Toggles the current shift state.

    This method provides the functionality of the shift key.

    \sa toggleShiftEnabled
*/
/*!
    \since 1.2

    Toggles the current shift state.

    This method provides the functionality of the shift key.

    \sa toggleShiftEnabled
*/
void DeclarativeShiftHandler::toggleShift()
{
    Q_D(DeclarativeShiftHandler);
    if (!d->toggleShiftEnabled)
        return;
    if (d->languageFilter.contains(d->locale.language())) {
        d->inputContext->setCapsLock(false);
        d->inputContext->setShift(!d->inputContext->shift());
    } else if (d->inputContext->inputMethodHints() & Qt::ImhNoAutoUppercase) {
        bool capsLock = d->inputContext->capsLock();
        d->inputContext->setCapsLock(!capsLock);
        d->inputContext->setShift(!capsLock);
    } else {
        d->inputContext->setCapsLock(!d->inputContext->capsLock() && d->inputContext->shift() && !d->shiftChanged);
        d->inputContext->setShift(d->inputContext->capsLock() || !d->inputContext->shift());
        d->shiftChanged = false;
    }
}

void DeclarativeShiftHandler::reset()
{
    Q_D(DeclarativeShiftHandler);
    if (d->inputContext->inputItem()) {
        Qt::InputMethodHints inputMethodHints = d->inputContext->inputMethodHints();
        bool preferUpperCase = (inputMethodHints & (Qt::ImhPreferUppercase | Qt::ImhUppercaseOnly));
        bool autoCapitalizationEnabled = !(d->inputContext->inputMethodHints() & (Qt::ImhNoAutoUppercase |
              Qt::ImhUppercaseOnly | Qt::ImhLowercaseOnly | Qt::ImhEmailCharactersOnly |
              Qt::ImhUrlCharactersOnly | Qt::ImhDialableCharactersOnly | Qt::ImhFormattedNumbersOnly |
              Qt::ImhDigitsOnly));
        bool toggleShiftEnabled = !(inputMethodHints & (Qt::ImhUppercaseOnly | Qt::ImhLowercaseOnly));
        // For filtered languages reset the initial shift status to lower case
        // and allow manual shift change
        if (d->languageFilter.contains(d->locale.language())) {
            preferUpperCase = false;
            autoCapitalizationEnabled = false;
            toggleShiftEnabled = true;
        }
        d->inputContext->setShift(preferUpperCase);
        d->inputContext->setCapsLock(preferUpperCase);
        setToggleShiftEnabled(toggleShiftEnabled);
        setAutoCapitalizationEnabled(autoCapitalizationEnabled);
    }
}

void DeclarativeShiftHandler::autoCapitalize()
{
    Q_D(DeclarativeShiftHandler);
    if (d->inputContext->capsLock())
        return;
    if (!d->autoCapitalizationEnabled || !d->inputContext->preeditText().isEmpty()) {
        d->inputContext->setShift(false);
    } else {
        int cursorPosition = d->inputContext->cursorPosition();
        bool preferLowerCase = d->inputContext->inputMethodHints() & Qt::ImhPreferLowercase;
        if (cursorPosition == 0) {
            d->inputContext->setShift(!preferLowerCase);
        } else {
            QString text = d->inputContext->surroundingText();
            text.truncate(cursorPosition);
            text = text.trimmed();
            if (text.length() == 0)
                d->inputContext->setShift(!preferLowerCase);
            else if (text.length() > 0 && d->sentenceEndingCharacters.indexOf(text[text.length() - 1]) >= 0)
                d->inputContext->setShift(!preferLowerCase);
            else
                d->inputContext->setShift(false);
        }
    }
}

void DeclarativeShiftHandler::restart()
{
    reset();
    autoCapitalize();
}

void DeclarativeShiftHandler::shiftChanged()
{
    Q_D(DeclarativeShiftHandler);
    d->shiftChanged = true;
}

void DeclarativeShiftHandler::localeChanged()
{
    Q_D(DeclarativeShiftHandler);
    d->locale = QLocale(d->inputContext->locale());
    restart();
}

void DeclarativeShiftHandler::setAutoCapitalizationEnabled(bool enabled)
{
    Q_D(DeclarativeShiftHandler);
    if (d->autoCapitalizationEnabled != enabled) {
        d->autoCapitalizationEnabled = enabled;
        emit autoCapitalizationEnabledChanged();
    }
}

void DeclarativeShiftHandler::setToggleShiftEnabled(bool enabled)
{
    Q_D(DeclarativeShiftHandler);
    if (d->toggleShiftEnabled != enabled) {
        d->toggleShiftEnabled = enabled;
        emit toggleShiftEnabledChanged();
    }
}

/*!
    \property DeclarativeShiftHandler::sentenceEndingCharacters

    This property specifies the sentence ending characters which
    will cause shift state change.

    By default, the property is initialized to sentence
    ending characters found in the ASCII range (i.e. ".!?").
*/

/*!
    \qmlproperty string ShiftHandler::sentenceEndingCharacters

    This property specifies the sentence ending characters which
    will cause shift state change.

    By default, the property is initialized to sentence
    ending characters found in the ASCII range (i.e. ".!?").
*/

/*!
    \since 1.2

    \property DeclarativeShiftHandler::autoCapitalizationEnabled

    This property provides the current state of the automatic
    capitalization feature.
*/

/*!
    \since 1.2

    \qmlproperty bool ShiftHandler::autoCapitalizationEnabled

    This property provides the current state of the automatic
    capitalization feature.
*/

/*!
    \since 1.2

    \property DeclarativeShiftHandler::toggleShiftEnabled

    This property provides the current state of the toggleShift()
    method. When true, the current shift state can be changed by
    calling the toggleShift() method.
*/

/*!
    \since 1.2

    \qmlproperty bool ShiftHandler::toggleShiftEnabled

    This property provides the current state of the toggleShift()
    method. When true, the current shift state can be changed by
    calling the toggleShift() method.
*/