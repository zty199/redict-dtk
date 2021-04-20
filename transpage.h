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

#ifndef TRANSPAGE_H
#define TRANSPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

#include <DApplication>
#include <DGuiApplicationHelper>

#include "widgets/textedit.h"
#include "youdaoapi.h"

class TransPage : public QWidget
{
    Q_OBJECT

public:
    TransPage(QWidget *parent = nullptr);
    ~TransPage();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    void translate();
    void handleTranslateFinished(const QString &result);
    void initTheme(DGuiApplicationHelper::ColorType);

private:
    TextEdit *m_orginEdit;
    TextEdit *m_transEdit;
    QComboBox *m_typeBox;
    QPushButton *m_transBtn;
    YoudaoAPI *m_api;

    QMap<QString, QString> m_types;
};

#endif
