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

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QStackedLayout>
#include <QPushButton>

#include "dailypage.h"
#include "dictpage.h"
#include "loadpage.h"
#include "widgets/queryedit.h"

class HomePage : public QWidget
{
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

    void updatePos();
    void detailQueried(QString queriedWord);

private:
    void queryWord();

private:
    QStackedLayout *m_layout;
    DailyPage *m_dailyPage;
    DictPage *m_dictPage;
    LoadPage *m_loadPage;
    QueryEdit *m_queryEdit;
    QPushButton *m_queryBtn;

    int m_currentIndex = 0;
};

#endif
