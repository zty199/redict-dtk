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

#ifndef DICTPAGE_H
#define DICTPAGE_H

#include <QWidget>
#include <QMediaPlayer>

#include <DGuiApplicationHelper>
#include <DLabel>
#include <DIconButton>

#include "youdaoapi.h"

DWIDGET_USE_NAMESPACE

class ScrollArea;
class DictPage : public QWidget
{
    Q_OBJECT

public:
    DictPage(QWidget *parent = nullptr);
    ~DictPage();

    void queryWord(const QString &text);

private:
    void initUI();
    void initConnections();
    void initTheme(DGuiApplicationHelper::ColorType);
    void handleQueryFinished(std::tuple<QString, QString, QString, QString, QString>);

private:
    YoudaoAPI *m_api;
    DLabel *m_wordLabel;
    DLabel *m_infoLabel;
    DIconButton *m_ukBtn;
    DIconButton *m_usBtn;
    DLabel *m_ukLabel;
    DLabel *m_usLabel;
    QMediaPlayer *m_audio;
    ScrollArea *m_scrollArea;

    QString m_text;
};

#endif
