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

#include "homepage.h"

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QDebug>

#include <DGuiApplicationHelper>
#include <DPlatformTheme>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QStackedLayout)
    , m_dailyPage(new DailyPage)
    , m_dictPage(new DictPage)
    , m_loadPage(new LoadPage)
    , m_queryEdit(new QueryEdit)
    , m_queryBtn(new QPushButton("查询"))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *queryLayout = new QHBoxLayout;
    queryLayout->setSpacing(0);
    queryLayout->setMargin(0);
    queryLayout->addWidget(m_queryEdit);
    queryLayout->addWidget(m_queryBtn);

    m_queryBtn->setFocusPolicy(Qt::NoFocus);
    m_queryBtn->setObjectName("QueryBtn");
    m_queryBtn->setFixedSize(90, 35);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addSpacing(1);
    mainLayout->addLayout(queryLayout);
    mainLayout->addLayout(m_layout);

    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_dailyPage);
    m_layout->addWidget(m_dictPage);
    m_layout->addWidget(m_loadPage);

    m_layout->setCurrentIndex(2);
    m_loadPage->start();

    connect(m_queryEdit, &QLineEdit::returnPressed, this, &HomePage::queryWord);
    connect(m_queryEdit, &QLineEdit::textChanged, this, &HomePage::queryWord);
    connect(m_queryBtn, &QPushButton::clicked, this, [=]() {
        queryWord();
        m_queryEdit->pressEnter();
    });

    connect(m_dailyPage, &DailyPage::loadFinished, this, [=] {
        m_layout->setCurrentIndex(0);
        m_queryEdit->setFocus();
        m_loadPage->stop();
    });

    connect(DGuiApplicationHelper::instance()->systemTheme(), &DPlatformTheme::activeColorChanged, this, [=](QColor activeColor) {
        QColor hover = DGuiApplicationHelper::adjustColor(activeColor, 0, 0, +10, 0, 0, 0, 0);
        QColor press = DGuiApplicationHelper::adjustColor(activeColor, 0, 0, -10, 0, 0, 0, 0);
        m_queryBtn->setStyleSheet("#QueryBtn {"
                                  "background-color: "
                                  + QString::number(activeColor.rgb(), 16).replace(0, 2, '#') + ";"
                                                                                                "border: none;"
                                                                                                "border-radius: none;"
                                                                                                "font-size: 15px;"
                                                                                                "color: white;"
                                                                                                "}"
                                                                                                "#QueryBtn:hover {"
                                                                                                "background-color: "
                                  + QString::number(hover.rgb(), 16).replace(0, 2, '#') + ";"
                                                                                          "}"
                                                                                          "#QueryBtn:pressed {"
                                                                                          "background-color: "
                                  + QString::number(press.rgb(), 16).replace(0, 2, '#') + ";"
                                                                                          "}");
    });

    emit DGuiApplicationHelper::instance()->systemTheme()->activeColorChanged(DGuiApplicationHelper::instance()->applicationPalette().highlight().color());
}

HomePage::~HomePage()
{
}

void HomePage::updatePos()
{
    m_queryEdit->updatePos();
}

void HomePage::detailQueried(QString queriedWord)
{
    if (!queriedWord.isEmpty()) {
        m_queryEdit->setText(queriedWord);
        QTimer::singleShot(100, this, [=]() {
            m_queryEdit->pressEnter();
        });
    }
}

void HomePage::queryWord()
{
    QString text = m_queryEdit->text();
    text = text.trimmed();

    if (text.isEmpty()) {
        m_layout->setCurrentIndex(0);
    } else {
        m_layout->setCurrentIndex(1);
        m_dictPage->queryWord(text);
    }
}
