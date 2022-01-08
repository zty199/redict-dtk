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

#ifndef DAILYPAGE_H
#define DAILYPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <DApplication>
#include <DGuiApplicationHelper>
#include <DLabel>

#include "youdaoapi.h"

DWIDGET_USE_NAMESPACE

class DailyPage : public QWidget
{
    Q_OBJECT

public:
    DailyPage(QWidget *parent = nullptr);
    ~DailyPage();

signals:
    void loadFinished();

private:
    void checkDirectory();
    void clearImageCache();
    void handleQueryFinished(std::tuple<QString, QString, QString, QString, QString>);
    void loadImage(const QByteArray &data);
    void initTheme();

private:
    QNetworkAccessManager *m_networkManager;
    DLabel *m_imageLabel;
    DLabel *m_contentLabel;
    YoudaoAPI *m_api;
};

#endif
