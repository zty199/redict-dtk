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

#ifndef POPUPCONTENT_H
#define POPUPCONTENT_H

#include <DBlurEffectWidget>
#include <DLabel>
#include <DCommandLinkButton>

DWIDGET_USE_NAMESPACE

class PopupContent : public DBlurEffectWidget
{
    Q_OBJECT

public:
    PopupContent(QWidget *parent = nullptr);
    ~PopupContent();

protected:
    void mouseMoveEvent(QMouseEvent *);

signals:
    void detailQueried(QString);

public slots:
    void updateContent(std::tuple<QString, QString, QString, QString, QString>);

private:
    DLabel *m_queryLabel;
    DLabel *m_transLabel;
    DCommandLinkButton *m_detailBtn;
};

#endif
