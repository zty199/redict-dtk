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

#include "toolbar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

#include "utils.h"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    QPixmap iconPixmap = Utils::renderSVG(":/images/redict.svg", QSize(22, 22));
    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(iconPixmap);

    tabbar = new QTabBar;
    tabbar->addTab("词典");
    tabbar->addTab("翻译");
    tabbar->setFocusPolicy(Qt::NoFocus);
    tabbar->installEventFilter(this);

    initTheme(DGuiApplicationHelper::instance()->themeType());

    QHBoxLayout *layout = new QHBoxLayout(this);
    // layout->addSpacing(5);
    // layout->addWidget(iconLabel);
    // layout->addSpacing(5);
    layout->addWidget(tabbar);
    layout->addStretch();
    layout->setMargin(0);

    connect(tabbar, &QTabBar::currentChanged, this, [=] (int current) { emit currentChanged(current); });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &ToolBar::initTheme);
}

ToolBar::~ToolBar()
{

}

void ToolBar::initTheme(DGuiApplicationHelper::ColorType themeType)
{
    if (themeType == DGuiApplicationHelper::DarkType) {
        tabbar->setStyleSheet(styleSheet() +
                              "QTabBar::tab {"
                              "font-size: 18px;"
                              "height: 30px;"
                              "padding: 9px;"
                              "border: 0px;"
                              "background-color: transparent;"
                              "border-bottom: 3px solid transparent;"
                              "width: 55px;"
                              "color: palette(text);"
                              "}"
                              "QTabBar::tab:hover {"
                              "background-color: #2F2F2F;"
                              "}"
                              "QTabBar::tab:selected {"
                              "color: #2CA7F8;"
                              "border: 0px;"
                              "border-bottom: 3px solid #2CA7F8;"
                              "}");
    } else {
        tabbar->setStyleSheet(styleSheet() +
                              "QTabBar::tab {"
                              "font-size: 18px;"
                              "height: 30px;"
                              "padding: 9px;"
                              "border: 0px;"
                              "background-color: transparent;"
                              "border-bottom: 3px solid transparent;"
                              "width: 55px;"
                              "color: palette(text);"
                              "}"
                              "QTabBar::tab:hover {"
                              "background-color: #EDEDED;"
                              "}"
                              "QTabBar::tab:selected {"
                              "color: #2CA7F8;"
                              "border: 0px;"
                              "border-bottom: 3px solid #2CA7F8;"
                              "}");
    }
}
