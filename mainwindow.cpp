/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QKeyEvent>

#include <DTitlebar>

#include "youdaoapi.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
    , m_mainLayout(new QStackedLayout)
    , m_toolBar(new ToolBar)
    , m_eventMonitor(new EventMonitor)
    , m_popupWindow(new PopupWindow)
    , m_homePage(new HomePage)
    , m_transPage(new TransPage)
    , m_trayIcon(new TrayIcon(this))
    , m_settings(new QSettings("deepin", "redict"))
    , m_menu(new QMenu)
    , m_wordingAction(new QAction("划词翻译"))
    , m_trayIconAction(new QAction("托盘显示"))
{
    m_eventMonitor->start();

    titlebar()->setIcon(QIcon(":/images/redict.svg"));
    titlebar()->addWidget(m_toolBar, Qt::AlignCenter);
    titlebar()->setSeparatorVisible(true);
    //    titlebar()->setBackgroundTransparent(true);
    titlebar()->setMenu(m_menu);

    installEventFilter(this);

    // init central widget.
    QWidget *centralWidget = new QWidget;
    m_mainLayout->addWidget(m_homePage);
    m_mainLayout->addWidget(m_transPage);

    centralWidget->setLayout(m_mainLayout);
    setWindowIcon(QIcon(":/images/redict.svg"));
    setCentralWidget(centralWidget);
    //    setShadowOffset(QPoint(0, 0));
    setFixedSize(600, 450);
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    m_wordingAction->setCheckable(true);
    m_trayIconAction->setCheckable(true);

    initWordingAction();
    initTrayIconAction();

    m_menu->addAction(m_wordingAction);
    m_menu->addAction(m_trayIconAction);

    connect(m_trayIcon, &TrayIcon::openActionTriggered, this, &MainWindow::activeWindow);
    connect(m_trayIcon, &TrayIcon::exitActionTriggered, qApp, &QApplication::quit);
    connect(m_wordingAction, &QAction::triggered, this, &MainWindow::handleWordingTriggered);
    connect(m_trayIconAction, &QAction::triggered, this, &MainWindow::handleTrayIconTriggered);
    connect(m_toolBar, &ToolBar::currentChanged, m_mainLayout, &QStackedLayout::setCurrentIndex);
    connect(this, &MainWindow::requestKeyPressEvent, this, &MainWindow::keyPressEvent);

    connect(m_popupWindow, &PopupWindow::detailQueried, this, [=](QString queryWord) {
        activeWindow();
        m_toolBar->tabbar->setCurrentIndex(0);
        m_homePage->detailQueried(queryWord);
    });
}

MainWindow::~MainWindow()
{
    delete m_popupWindow;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (m_enableTrayIcon) {
        setVisible(false);
        e->ignore();
    } else {
        e->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Tab) {
        const int tabCount = m_toolBar->tabbar->count();
        int tabIndex = m_toolBar->tabbar->currentIndex() + 1;

        if (tabIndex >= tabCount) {
            tabIndex = 0;
        }

        m_toolBar->tabbar->setCurrentIndex(tabIndex);
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Move) {
        //        QMoveEvent *moveEvent = static_cast<QMoveEvent *>(event);
        if (m_toolBar->tabbar->currentIndex() == 0) {
            m_homePage->updatePos();
        }
    }

    return DMainWindow::eventFilter(object, event);
}

void MainWindow::activeWindow()
{
    setVisible(true);
    activateWindow();
    setFocus();
}

void MainWindow::initWordingAction()
{
    m_enableWording = m_settings->value("wording").toBool();

    if (m_enableWording) {
        m_wordingAction->setChecked(true);
        enableWording();
    } else {
        m_wordingAction->setChecked(false);
    }
}

void MainWindow::initTrayIconAction()
{
    m_enableTrayIcon = m_settings->value("tray").toBool();

    if (m_enableTrayIcon) {
        m_trayIconAction->setChecked(true);
        m_trayIcon->show();
    } else {
        m_trayIconAction->setChecked(false);
        m_trayIcon->hide();
    }
}

void MainWindow::enableWording()
{
    // Windows and MacOS not support.
    connect(qApp->clipboard(), &QClipboard::selectionChanged, [=] {
        if (m_popupWindow->content()->isVisible()) {
            return;
        }
        m_popupWindow->popup(QCursor::pos());
        m_popupWindow->query(qApp->clipboard()->text(QClipboard::Selection));
    });

    m_settings->setValue("wording", true);
    m_enableWording = true;
}

void MainWindow::disableWording()
{
    qApp->clipboard()->disconnect();

    m_settings->setValue("wording", false);
    m_enableWording = false;
}

void MainWindow::handleWordingTriggered()
{
    if (m_enableWording) {
        disableWording();
    } else {
        enableWording();
    }

    initWordingAction();
}

void MainWindow::handleTrayIconTriggered()
{
    if (m_enableTrayIcon) {
        m_enableTrayIcon = false;
        m_settings->setValue("tray", false);
        m_trayIcon->hide();
    } else {
        m_enableTrayIcon = true;
        m_settings->setValue("tray", true);
        m_trayIcon->show();
    }

    initTrayIconAction();
}
