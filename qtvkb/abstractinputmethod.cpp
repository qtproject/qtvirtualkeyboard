/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Quick Enterprise Controls add-on.
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

#include "abstractinputmethod.h"

/*!
    \class AbstractInputMethod

    \inmodule InputFramework

    \brief A base class for an input method.

    Use this class if you want to implement custom input
    method using C/C++ language.
*/

/*!
    Constructs an input method with \a d_ptr as private data
    from derived class and \a parent.
*/
AbstractInputMethod::AbstractInputMethod(AbstractInputMethodPrivate* d_ptr, QObject* parent) :
    QObject(parent),
    d_ptr(d_ptr)
{
}

/*!
    Constructs an input method with \a parent.
*/
AbstractInputMethod::AbstractInputMethod(QObject* parent) :
    QObject(parent),
    d_ptr(new AbstractInputMethodPrivate())
{
}

/*!
    Destroys the input method and frees all allocated resources.
*/
AbstractInputMethod::~AbstractInputMethod()
{
}

/*!
    Returns meta class name of this instance.
*/
QString AbstractInputMethod::className() const
{
    return metaObject()->className();
}

/*!
    Returns the input context associated with the input method.
    This method returns NULL if the input method is not active.
*/
DeclarativeInputContext* AbstractInputMethod::inputContext() const
{
    Q_D(const AbstractInputMethod);
    return d->inputEngine ? d->inputEngine->inputContext() : 0;
}

/*!
    Returns the input engine associated with the input method.
    This method returns NULL if the input method is not active.
*/
DeclarativeInputEngine* AbstractInputMethod::inputEngine() const
{
    Q_D(const AbstractInputMethod);
    return d->inputEngine;
}

/*!
    This method is called by input engine when input method needs
    reset. The input method must reset internal state only. The main
    difference to update() method is that the reset() modifies only
    the input method state, i.e. it must not modify the input context.
*/
void AbstractInputMethod::reset()
{
}

/*!
    This method is called by input engine when input method needs
    update. The input method must close current pre-edit text and
    restore internal state to default.
*/
void AbstractInputMethod::update()
{
}

/*!
    \internal
    Called by input engine when the input method is activated and
    deactivated.
*/
void AbstractInputMethod::setInputEngine(DeclarativeInputEngine* inputEngine)
{
    Q_D(AbstractInputMethod);
    if (d->inputEngine) {
        disconnect(this, SLOT(reset()));
        disconnect(this, SLOT(update()));
    }
    d->inputEngine = inputEngine;
    if (d->inputEngine) {
        connect(d->inputEngine, SIGNAL(inputMethodReset()), SLOT(reset()));
        connect(d->inputEngine, SIGNAL(inputMethodUpdate()), SLOT(update()));
    }
}

QList<DeclarativeSelectionListModel::Type> AbstractInputMethod::selectionLists()
{
    return QList<DeclarativeSelectionListModel::Type>();
}

int AbstractInputMethod::selectionListItemCount(DeclarativeSelectionListModel::Type type)
{
    Q_UNUSED(type)
    return 0;
}

QVariant AbstractInputMethod::selectionListData(DeclarativeSelectionListModel::Type type, int index, int role)
{
    Q_UNUSED(type)
    Q_UNUSED(index)
    switch(role) {
    case DeclarativeSelectionListModel::DisplayRole:
        return QVariant("");
    case DeclarativeSelectionListModel::WordCompletionLengthRole:
        return QVariant(0);
    }
    return QVariant();
}

void AbstractInputMethod::selectionListItemSelected(DeclarativeSelectionListModel::Type type, int index)
{
    Q_UNUSED(type)
    Q_UNUSED(index)
}

/*!
    \property AbstractInputMethod::className
    \brief Class name of this input method.

    This property is used to distinct different input method classes.
    The class name is taken from Qt meta class object.
*/

/*!
    \fn QList<DeclarativeInputEngine::InputMode> AbstractInputMethod::inputModes(const QString& locale)

    Returns a list of input modes for \a locale.
*/

/*!
    \fn bool AbstractInputMethod::setInputMode(const QString& locale, DeclarativeInputEngine::InputMode inputMode)
    \ingroup testi

    Changes \a inputMode and \a locale for input method. The method returns true if
    successful.
*/

/*!
    \fn bool AbstractInputMethod::setTextCase(DeclarativeInputEngine::TextCase textCase)

    Updates \a textCase for input method. The method returns true if successful.
*/

/*!
    \fn bool AbstractInputMethod::keyEvent(Qt::Key key, const QString& text, Qt::KeyboardModifiers modifiers)

    Purpose of this method is to handle the key events generated by the the
    input engine.

    The \a key parameter specifies code of the key to handle. The key code
    does not distinguish between capital and non-capital letters. The \a
    text parameter contains the Unicode text for the key. The \a modifiers
    parameter contains the key modifiers that apply to key.

    This method returns true if the key event was successfully handled.
    If the return value is false, the key event is redirected for default
    input method for futher processing.
*/

/*!
    \fn QList<DeclarativeSelectionListModel::Type> AbstractInputMethod::selectionLists()

    Returns list of selection lists used for input method.

    This method is called by input engine when the input method is being
    activated. The input method can reserve the selection lists for its use
    by returning a list of selection list types required.
*/

/*!
    \fn int AbstractInputMethod::selectionListItemCount(DeclarativeSelectionListModel::Type type)

    Returns number of items in the selection list identified by \a type.
*/

/*!
    \fn QVariant AbstractInputMethod::selectionListData(DeclarativeSelectionListModel::Type type, int index, int role)

    Returns item data for a selection list identified by \a type. The \a role
    parameter specifies which data is requested. The \a index parameter is a
    zero based index to list.
*/

/*!
    \fn void AbstractInputMethod::selectionListItemSelected(DeclarativeSelectionListModel::Type type, int index)

    This method is called when an item at \a index has been selected by user.
    The selection list is identified by \a type parameter.
*/

/*!
    \fn void AbstractInputMethod::selectionListChanged(int type)

    The input method emits this signal when the contents of selection list
    is changed. The \a type parameter specifies which selection list has changed.
*/

/*!
    \fn void AbstractInputMethod::selectionListActiveItemChanged(int type, int index)

    The input method emits this signal when the current \a index has changed
    in the selection list identified by \a type.
*/