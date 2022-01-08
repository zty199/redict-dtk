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

#include "dictpage.h"
#include "widgets/scrollarea.h"

#include <QLayout>
#include <QScrollBar>

#include <DPaletteHelper>
#include <DPlatformTheme>

DictPage::DictPage(QWidget *parent)
    : QWidget(parent)
    , m_api(YoudaoAPI::instance())
    , m_wordLabel(new DLabel)
    , m_infoLabel(new DLabel)
    , m_ukBtn(new DIconButton(DStyle::SP_MediaVolumeHighElement))
    , m_usBtn(new DIconButton(DStyle::SP_MediaVolumeHighElement))
    , m_ukLabel(new DLabel)
    , m_usLabel(new DLabel)
    , m_audio(new QMediaPlayer)
    , m_scrollArea(new ScrollArea)
    , m_text("")
{
    initUI();
    initConnections();
}

DictPage::~DictPage()
{
    delete m_audio;
}

void DictPage::queryWord(const QString &text)
{
    if (text == m_text) {
        return;
    }
    m_text = text;

    // init status.
    m_wordLabel->setVisible(false);
    m_infoLabel->setVisible(false);
    m_usBtn->setVisible(false);
    m_ukBtn->setVisible(false);
    m_usLabel->setVisible(false);
    m_ukLabel->setVisible(false);

    m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->minimum());
    m_api->queryWord(text);
}

void DictPage::initUI()
{
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setWidgetResizable(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_scrollArea);

    QWidget *contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    QHBoxLayout *phoneticLayout = new QHBoxLayout;
    phoneticLayout->setMargin(0);
    phoneticLayout->setSpacing(0);
    phoneticLayout->addWidget(m_ukLabel);
    phoneticLayout->addSpacing(5);
    phoneticLayout->addWidget(m_ukBtn);
    phoneticLayout->addSpacing(70);
    phoneticLayout->addWidget(m_usLabel);
    phoneticLayout->addSpacing(5);
    phoneticLayout->addWidget(m_usBtn);
    phoneticLayout->addStretch();

    contentLayout->setContentsMargins(20, 0, 35, 0);
    contentLayout->setSpacing(0);
    contentLayout->setAlignment(Qt::AlignTop);
    contentLayout->addSpacing(5);
    contentLayout->addWidget(m_wordLabel);
    contentLayout->addSpacing(4);
    contentLayout->addLayout(phoneticLayout);
    contentLayout->addSpacing(6);
    contentLayout->addWidget(m_infoLabel);
    contentLayout->addSpacing(10);

    m_scrollArea->setWidget(contentWidget);

    QFont font = m_wordLabel->font();
    font.setPixelSize(32);
    font.setBold(true);
    m_wordLabel->setFont(font);
    font = m_ukBtn->font();
    font.setPixelSize(14);
    m_ukLabel->setFont(font);
    m_usLabel->setFont(font);
    font = m_infoLabel->font();
    font.setPixelSize(16);
    m_infoLabel->setFont(font);

    m_wordLabel->setWordWrap(true);
    m_infoLabel->setWordWrap(true);

    m_ukLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_usLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_infoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    m_ukBtn->setIconSize(QSize(20, 20));
    m_usBtn->setIconSize(QSize(20, 20));
    m_ukBtn->setFlat(true);
    m_usBtn->setFlat(true);

    initTheme(DGuiApplicationHelper::instance()->themeType());
}

void DictPage::initConnections()
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &DictPage::initTheme);

    connect(m_api, &YoudaoAPI::searchFinished, this, &DictPage::handleQueryFinished);

    connect(m_ukBtn, &DIconButton::clicked, this, [=] {
        m_audio->setMedia(QUrl("http://dict.youdao.com/dictvoice?type=1&audio=" + m_wordLabel->text()));
        m_audio->play();
    });

    connect(m_usBtn, &DIconButton::clicked, this, [=] {
        m_audio->setMedia(QUrl("http://dict.youdao.com/dictvoice?type=2&audio=" + m_wordLabel->text()));
        m_audio->play();
    });

    connect(DGuiApplicationHelper::instance()->systemTheme(), &DPlatformTheme::activeColorChanged, this, [=](QColor activeColor) {
        DPalette palette = DPaletteHelper::instance()->palette(m_wordLabel);
        palette.setColor(DPalette::WindowText, activeColor);
        DPaletteHelper::instance()->setPalette(m_wordLabel, palette);
    });
}

void DictPage::initTheme(DGuiApplicationHelper::ColorType themeType)
{
    Q_UNUSED(themeType)
}

void DictPage::handleQueryFinished(std::tuple<QString, QString, QString, QString, QString> data)
{
    const QString &queryWord = std::get<0>(data);
    const QString &ukPhonetic = std::get<1>(data);
    const QString &usPhonetic = std::get<2>(data);
    const QString &basicExplains = std::get<3>(data);
    const QString &webReferences = std::get<4>(data);

    if (ukPhonetic.isEmpty()) {
        m_ukLabel->setVisible(false);
        m_ukBtn->setVisible(false);
    } else {
        m_ukLabel->setVisible(true);
        m_ukBtn->setVisible(true);
        m_ukLabel->setText(QString("英 [%1]").arg(ukPhonetic));
    }

    if (usPhonetic.isEmpty()) {
        m_usLabel->setVisible(false);
        m_usBtn->setVisible(false);
    } else {
        m_usLabel->setVisible(true);
        m_usBtn->setVisible(true);
        m_usLabel->setText(QString("美 [%1]").arg(usPhonetic));
    }

    m_wordLabel->setVisible(true);
    m_wordLabel->setText(queryWord);

    QString text = basicExplains;

    if (!webReferences.isEmpty()) {
        text += "<br>网络释义</br>";
        text += webReferences;
    }
    text.replace("<br>", "\n");
    text.remove("</br>");

    m_infoLabel->setVisible(true);
    m_infoLabel->setText(text);
}
