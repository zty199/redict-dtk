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

#include <QLayout>
#include <QMouseEvent>

#include <DPlatformTheme>

#include "utils.h"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , m_activeColor("")
{
    tabbar = new QTabBar;
    tabbar->addTab("词典");
    tabbar->addTab("翻译");
    tabbar->setFocusPolicy(Qt::NoFocus);
    tabbar->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(tabbar);
    layout->addStretch();
    layout->setMargin(0);

    connect(tabbar, &QTabBar::currentChanged, this, [=](int current) {
        emit currentChanged(current);
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &ToolBar::initTheme);

    connect(DGuiApplicationHelper::instance()->systemTheme(), &DPlatformTheme::activeColorChanged, this, [=](QColor activeColor) {
        m_activeColor = QString::number(activeColor.rgb(), 16).replace(0, 2, '#');
        initTheme(DGuiApplicationHelper::instance()->themeType());
    });

    emit DGuiApplicationHelper::instance()->systemTheme()->activeColorChanged(DGuiApplicationHelper::instance()->applicationPalette().highlight().color());
}

ToolBar::~ToolBar()
{
}

void ToolBar::initTheme(DGuiApplicationHelper::ColorType themeType)
{
    if (themeType == DGuiApplicationHelper::DarkType) {
        tabbar->setStyleSheet(styleSheet() + "QTabBar::tab {"
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
                                             "color: "
                              + m_activeColor + ";"
                                                "border: 0px;"
                                                "border-bottom: 3px solid "
                              + m_activeColor + ";"
                                                "}");
    } else {
        tabbar->setStyleSheet(styleSheet() + "QTabBar::tab {"
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
                                             "color: "
                              + m_activeColor + ";"
                                                "border: 0px;"
                                                "border-bottom: 3px solid "
                              + m_activeColor + ";"
                                                "}");
    }
}
