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

#include "transpage.h"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QKeyEvent>

TransPage::TransPage(QWidget *parent)
    : QWidget(parent)
    , m_orginEdit(new TextEdit)
    , m_transEdit(new TextEdit)
    , m_typeBox(new QComboBox)
    , m_transBtn(new QPushButton("翻译"))
    , m_api(YoudaoAPI::instance())
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *transLayout = new QHBoxLayout;

    m_typeBox->addItem("自动检测语言");
    m_typeBox->addItem("中文 → 英语");
    m_typeBox->addItem("中文 → 日语");
    m_typeBox->addItem("中文 → 韩语");
    m_typeBox->addItem("中文 → 法语");
    m_typeBox->addItem("中文 → 俄语");
    m_typeBox->addItem("中文 → 西班牙语");
    m_typeBox->addItem("英语 → 中文");

    transLayout->addWidget(m_typeBox);
    transLayout->addWidget(m_transBtn);

    layout->setContentsMargins(15, 10, 15, 15);
    layout->addWidget(m_orginEdit);
    layout->addSpacing(5);
    layout->addLayout(transLayout);
    layout->addSpacing(5);
    layout->addWidget(m_transEdit);

    m_transBtn->setObjectName("QueryBtn");
    m_transBtn->setStyleSheet("#QueryBtn {"
                              "background-color: #2CA7F8;"
                              "border: none;"
                              "border-radius: 8px;"
                              "font-size: 15px;"
                              "color: white;"
                              "}"
                              "#QueryBtn:hover {"
                              "background-color: #43B4FF;"
                              "}"
                              "#QueryBtn:pressed {"
                              "background-color: #099DFF;"
                              "}");
    m_transBtn->setFixedSize(130, 36);

    m_orginEdit->setPlaceholderText("请输入您要翻译的文字");
    m_transEdit->setReadOnly(true);

    initTheme(DGuiApplicationHelper::instance()->themeType());

    connect(m_transBtn, &QPushButton::clicked, this, &TransPage::translate);
    connect(m_api, &YoudaoAPI::translateFinished, this, &TransPage::handleTranslateFinished);
    connect(m_typeBox, &QComboBox::currentTextChanged, [=] { translate(); });

    connect(m_orginEdit, &TextEdit::textChanged, [=] { if (m_orginEdit->toPlainText().isEmpty()) { m_transEdit->clear(); } });

    connect(m_orginEdit, &TextEdit::focusIn, [=] { m_transEdit->clearSelection(); });
    connect(m_orginEdit, &TextEdit::focusOut, [=] { m_orginEdit->clearSelection(); });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &TransPage::initTheme);
}

TransPage::~TransPage()
{

}

void TransPage::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return && (e->modifiers() & Qt::ControlModifier)) {
        translate();
    }

    // request window keypress.
    (qobject_cast<MainWindow *>(this->window()))->requestKeyPressEvent(e);
}

void TransPage::translate()
{
    QString text = m_orginEdit->toPlainText();

    if (text.isEmpty())
        return;

    int currentType = m_typeBox->currentIndex();
    QString type;

    switch (currentType) {
    case 0:
        type = "AUTO";
        break;
    case 1:
        type = "ZH_CN2EN";
        break;
    case 2:
        type = "ZH_CN2JA";
        break;
    case 3:
        type = "ZH_CN2KR";
        break;
    case 4:
        type = "ZH_CN2FR";
        break;
    case 5:
        type = "ZH_CN2RU";
        break;
    case 6:
        type = "ZH_CN2SP";
        break;
    case 7:
        type = "EN2ZH_CN";
        break;
    default:
        break;
    }

    m_api->translate(text, type);
}

void TransPage::handleTranslateFinished(const QString &result)
{
    m_transEdit->setPlainText(result);
}

void TransPage::initTheme(DGuiApplicationHelper::ColorType themeType)
{
    if (themeType == DGuiApplicationHelper::DarkType)
    {
        m_typeBox->setStyleSheet(styleSheet() + "font-size: 15px;");
        m_orginEdit->setStyleSheet(styleSheet() +
                                   "QPlainTextEdit {"
                                   "background-color: #2D2D2D;"
                                   "border: 1px solid #151515;"
                                   "border-radius: 8px;"
                                   "padding: 5px;"
                                   "font-size: 15px;"
                                   "}");
        m_transEdit->setStyleSheet(styleSheet() +
                                   "QPlainTextEdit {"
                                   "background-color: #2D2D2D;"
                                   "border: 1px solid #151515;"
                                   "border-radius: 8px;"
                                   "padding: 5px;"
                                   "font-size: 15px;"
                                   "}");
    } else {
        m_typeBox->setStyleSheet(styleSheet() + "font-size: 15px;");
        m_orginEdit->setStyleSheet(styleSheet() +
                                   "QPlainTextEdit {"
                                   "background-color: #F2F2F2;"
                                   "border: 1px solid #E8E8E8;"
                                   "border-radius: 8px;"
                                   "padding: 5px;"
                                   "font-size: 15px;"
                                   "}");
        m_transEdit->setStyleSheet(styleSheet() +
                                   "QPlainTextEdit {"
                                   "background-color: #F2F2F2;"
                                   "border: 1px solid #E8E8E8;"
                                   "border-radius: 8px;"
                                   "padding: 5px;"
                                   "font-size: 15px;"
                                   "}");
    }
}
