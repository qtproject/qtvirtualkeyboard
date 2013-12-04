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

#include "xcbinputpanel.h"
#include "inputview.h"
#include "platforminputcontext.h"
#include "declarativeinputcontext.h"
#include <QGuiApplication>
#include <QQmlEngine>
#include <QScreen>
#include "vkbdebug.h"
#include <xcb/xcb.h>
#include <xcb/xfixes.h>
#include <qpa/qplatformnativeinterface.h>

class XcbInputPanelPrivate
{
public:
    QScopedPointer<InputView> view;
};

XcbInputPanel::XcbInputPanel(QObject *parent) :
    AbstractInputPanel(parent),
    d_ptr(new XcbInputPanelPrivate())
{
    /*  Activate the alpha buffer for this application.
    */
    QQuickWindow::setDefaultAlphaBuffer(true);
    QScreen* screen = QGuiApplication::primaryScreen();
    connect(screen, SIGNAL(virtualGeometryChanged(QRect)), SLOT(repositionView(QRect)));
}

XcbInputPanel::~XcbInputPanel()
{
}

void XcbInputPanel::show()
{
    Q_D(XcbInputPanel);
    repositionView(QGuiApplication::primaryScreen()->availableGeometry());
    d->view->show();
}

void XcbInputPanel::hide()
{
    Q_D(XcbInputPanel);
    if (d->view) {
        d->view->hide();
    }
}

bool XcbInputPanel::isVisible() const
{
    Q_D(const XcbInputPanel);
    return d->view && d->view->isVisible();
}

void XcbInputPanel::setInputRect(const QRect& inputRect)
{
    VKB_DEBUG() << "XcbInputPanel::setInputRect():" << inputRect;
    /*  Set input region using platform specific code.
        In optimal case there should be generic way to
        do this, but currenly none is available.
    */
    Q_D(XcbInputPanel);
    QWindow* window = d->view.data();
    if (!window) {
        return;
    }
    QPlatformNativeInterface* platformNativeInterface = QGuiApplication::platformNativeInterface();
    xcb_connection_t* xbcConnection = static_cast<xcb_connection_t*>(platformNativeInterface->nativeResourceForWindow("connection", window));
    xcb_xfixes_region_t xbcRegion = xcb_generate_id(xbcConnection);
    const xcb_rectangle_t xbcInputRect = {
        static_cast<int16_t>(inputRect.x()),
        static_cast<int16_t>(inputRect.y()),
        static_cast<uint16_t>(inputRect.width()),
        static_cast<uint16_t>(inputRect.height())
    };
    xcb_xfixes_create_region(xbcConnection, xbcRegion, 1, &xbcInputRect);
    xcb_xfixes_set_window_shape_region(xbcConnection, window->winId(), XCB_SHAPE_SK_INPUT, 0, 0, xbcRegion);
    xcb_xfixes_destroy_region(xbcConnection, xbcRegion);
}

void XcbInputPanel::createView()
{
    Q_D(XcbInputPanel);
    if (!d->view) {
        d->view.reset(new InputView());
        d->view->setFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus | Qt::BypassWindowManagerHint);
        d->view->setColor(QColor(Qt::transparent));
        const QString vkbImportPath(QT_VKB_IMPORT_PATH);
        d->view->engine()->addImportPath(vkbImportPath);
        d->view->setSource(QUrl::fromLocalFile(vkbImportPath+"/InputPanel.qml"));
        /*  Destroy the view along with the last window in application. */
        connect(qGuiApp, SIGNAL(lastWindowClosed()), SLOT(destroyView()));
    }
}

void XcbInputPanel::destroyView()
{
    Q_D(XcbInputPanel);
    d->view.reset();
}

void XcbInputPanel::repositionView(const QRect& rect)
{
    Q_D(XcbInputPanel);
    VKB_DEBUG() << "XcbInputPanel::repositionView():" << rect;
    if (d->view && d->view->geometry() != rect) {
        DeclarativeInputContext* inputContext = qobject_cast<PlatformInputContext*>(parent())->declarativeInputContext();
        if (inputContext)
            inputContext->setAnimating(true);
        d->view->setResizeMode(QQuickView::SizeViewToRootObject);
        setInputRect(QRect());
        d->view->setGeometry(rect);
        d->view->setResizeMode(QQuickView::SizeRootObjectToView);
        if (inputContext)
            inputContext->setAnimating(false);
    }
}