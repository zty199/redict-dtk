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

#include "popupcontent.h"
#include "widgets/scrollarea.h"

#include <QLayout>
#include <QMouseEvent>

#include <DBlurEffectWidget>
#include <DGuiApplicationHelper>
#include <DPaletteHelper>
#include <DPlatformTheme>

PopupContent::PopupContent(QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_queryLabel(new DLabel)
    , m_transLabel(new DLabel)
    , m_detailBtn(new DCommandLinkButton(tr("详情")))
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setFixedSize(300, 225);

    //    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    //    setMaskColor(DBlurEffectWidget::LightColor);

    ScrollArea *contentFrame = new ScrollArea;
    contentFrame->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(contentFrame);

    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    contentFrame->setWidget(mainWidget);

    QWidget *queryWidget = new QWidget;
    QHBoxLayout *queryLayout = new QHBoxLayout(queryWidget);
    queryLayout->setMargin(0);
    queryLayout->setSpacing(0);
    queryLayout->addWidget(m_queryLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    queryLayout->addWidget(m_detailBtn, 0, Qt::AlignRight | Qt::AlignVCenter);

    mainLayout->setContentsMargins(10, 10, 25, 10);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(queryWidget);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_transLabel);
    mainLayout->addStretch();

    QFont font = m_queryLabel->font();
    font.setPixelSize(24);
    m_queryLabel->setFont(font);
    font.setPixelSize(14);
    m_detailBtn->setFont(font);
    m_transLabel->setFont(font);

    m_queryLabel->setWordWrap(true);
    m_transLabel->setWordWrap(true);

    m_transLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    m_queryLabel->setMaximumWidth(230);
    m_queryLabel->setElideMode(Qt::ElideRight);

    connect(m_detailBtn, &DCommandLinkButton::clicked, this, [=]() {
        emit detailQueried(m_queryLabel->text());
        hide();
    });

    connect(DGuiApplicationHelper::instance()->systemTheme(), &DPlatformTheme::activeColorChanged, this, [=](QColor activeColor) {
        DPalette palette = DPaletteHelper::instance()->palette(m_queryLabel);
        palette.setColor(DPalette::WindowText, activeColor);
        DPaletteHelper::instance()->setPalette(m_queryLabel, palette);
    });
}

PopupContent::~PopupContent()
{
}

void PopupContent::mouseMoveEvent(QMouseEvent *e)
{
    // disable move window.
    e->accept();
}

void PopupContent::updateContent(std::tuple<QString, QString, QString, QString, QString> data)
{
    m_queryLabel->setText(std::get<0>(data));
    m_queryLabel->setToolTip(std::get<0>(data));

    QString trans = std::get<3>(data);
    trans.replace("<br>", "\n");
    trans.remove("</br>");
    m_transLabel->setText(trans);
}
