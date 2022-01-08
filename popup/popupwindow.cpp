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

#include "popupwindow.h"
#include "utils.h"
#include "eventmonitor.h"

#include <QMouseEvent>
#include <QCloseEvent>
#include <QPainter>
#include <QDebug>

PopupWindow::PopupWindow(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QStackedLayout(this))
    , m_content(new PopupContent)
    , m_api(new YoudaoAPI)
    , m_eventMonitor(new EventMonitor)
{
    connect(m_eventMonitor, &EventMonitor::buttonPress, this, &PopupWindow::onGlobMousePress, Qt::QueuedConnection);

    m_iconPixmap = Utils::renderSVG(":/images/redict.svg", QSize(32, 32));

    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::PointingHandCursor);
    setFixedSize(32, 32);

    QWidget::hide();

    connect(m_api, &YoudaoAPI::searchFinished, m_content, &PopupContent::updateContent);
    connect(m_content, &PopupContent::detailQueried, this, &PopupWindow::detailQueried);
}

PopupWindow::~PopupWindow()
{
    delete m_content;
    delete m_api;

    if (m_eventMonitor) {
        m_eventMonitor->quit();
        m_eventMonitor->deleteLater();
    }
}

PopupContent *PopupWindow::content()
{
    return m_content;
}

void PopupWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.save();

    painter.drawPixmap(rect(), m_iconPixmap);

    painter.restore();

    QWidget::paintEvent(e);
}

void PopupWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        QWidget::hide();

        m_content->move(geometry().bottomRight());
        m_content->show();
    }

    QWidget::mouseReleaseEvent(e);
}

void PopupWindow::onGlobMousePress(const int &x, const int &y)
{
    QPoint mousePos(x, y);

    if (m_content->isVisible()) {
        const QRect rect = QRect(m_content->pos(), m_content->size());
        if (rect.contains(mousePos)) {
            return;
        }
    }

    const QRect rect = QRect(pos(), size());
    if (rect.contains(mousePos) && !m_content->isVisible()) {
        return;
    }

    if (m_eventMonitor->isRunning()) {
        m_eventMonitor->quit();
    }

    m_content->hide();
    QWidget::hide();
}

void PopupWindow::popup(const QPoint &pos)
{
    QWidget::move(QPoint(pos.x(), pos.y() - 40));
    QWidget::show();

    if (!m_eventMonitor->isRunning()) {
        m_eventMonitor->start();
    }

    m_content->hide();
}

void PopupWindow::query(const QString &text)
{
    m_api->queryWord(text);
}
