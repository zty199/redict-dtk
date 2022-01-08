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

#include "queryedit.h"
#include "utils.h"
#include "listdelegate.h"

#include <QLayout>
#include <QKeyEvent>

QueryEdit::QueryEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_listView(new QListView(this))
    , m_listModel(new QStringListModel(this))
    , m_api(YoudaoAPI::instance())
    , m_closeBtn(new DIconButton(DStyle::SP_CloseButton))
    , m_iconLabel(new QLabel)
    , m_isEnter(false)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addSpacing(9);

    m_iconLabel->setFixedSize(18, 18);

    m_closeBtn->setFixedSize(18, 18);
    m_closeBtn->setIconSize(QSize(18, 18));
    m_closeBtn->setEnabledCircle(true);
    m_closeBtn->setFlat(true);
    m_closeBtn->hide();

    layout->addWidget(m_iconLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_closeBtn, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addSpacing(9);

    m_listView->setItemDelegate(new ListDelegate);
    m_listView->setWindowFlags(Qt::ToolTip);
    m_listView->setFixedHeight(145);

    setPlaceholderText("输入要查询的单词或词组");
    setFocusPolicy(Qt::StrongFocus);
    setTextMargins(30, 0, 32, 0);
    setObjectName("QueryEdit");
    setFixedHeight(35);

    initTheme(DGuiApplicationHelper::instance()->themeType());

    connect(this, &QLineEdit::textChanged, [=] {
        const QString text = this->text();

        if (text.isEmpty()) {
            m_closeBtn->hide();
            m_listView->hide();
        } else {
            m_closeBtn->show();
        }

        if (!m_isEnter) {
            m_api->suggest(text);
        }
    });

    connect(m_api, &YoudaoAPI::suggestFinished, this, &QueryEdit::handleSuggest);
    connect(m_closeBtn, &DIconButton::clicked, this, &QLineEdit::clear);

    connect(m_listView, &QListView::clicked, this, [=](const QModelIndex &index) {
        QStringList data = index.data().toString().split(" | ");
        m_isEnter = true;
        setText(data.first().trimmed());
        m_listView->hide();
        selectAll();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &QueryEdit::initTheme);
}

QueryEdit::~QueryEdit()
{
}

void QueryEdit::pressEnter()
{
    QModelIndex currentIndex = m_listView->currentIndex();

    m_isEnter = true;
    if (currentIndex.isValid()) {
        QStringList data = currentIndex.data().toString().split(" | ");
        setText(data.first().trimmed());
    }
    m_listView->hide();
    selectAll();
}

void QueryEdit::updatePos()
{
    if (m_listView->isVisible()) {
        QPoint p(0, height());
        int x = mapToGlobal(p).x() + 20;
        int y = mapToGlobal(p).y() + 1;
        m_listView->move(x, y);
        m_listView->show();
    }
}

void QueryEdit::keyPressEvent(QKeyEvent *e)
{
    if (m_listView->isHidden()) {
        m_isEnter = false;
        QLineEdit::keyPressEvent(e);
    } else {
        QModelIndex currentIndex = m_listView->currentIndex();
        int count = m_listView->model()->rowCount();

        if (e->key() == Qt::Key_Down) {
            int row = currentIndex.row() + 1;
            if (row >= count) {
                row = 0;
            }

            QModelIndex index = m_listView->model()->index(row, 0);
            m_listView->setCurrentIndex(index);

        } else if (e->key() == Qt::Key_Up) {
            int row = currentIndex.row() - 1;
            if (row >= count || row < 0) {
                row = 0;
            }

            QModelIndex index = m_listView->model()->index(row, 0);
            m_listView->setCurrentIndex(index);

        } else if (e->key() == Qt::Key_Escape) {
            m_listView->hide();
        } else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
            pressEnter();
        } else {
            m_isEnter = false;
            m_listView->hide();
            QLineEdit::keyPressEvent(e);
        }
    }
}

void QueryEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
}

void QueryEdit::focusOutEvent(QFocusEvent *e)
{
    m_listView->hide();

    QLineEdit::focusOutEvent(e);
}

void QueryEdit::initTheme(DGuiApplicationHelper::ColorType themeType)
{
    // const bool isDark = DThemeManager::instance()->theme() == "dark";

    QPixmap iconPixmap;

    if (themeType == DGuiApplicationHelper::DarkType) {
        iconPixmap = Utils::renderSVG(":/images/search_dark.svg", QSize(14, 14));
        setStyleSheet("#QueryEdit {"
                      "border: none;"
                      "border-radius: none;"
                      "background: #353535;"
                      "}"
                      "QListView {"
                      "border: 1px solid #505050;"
                      "background-color: #2D2D2D;"
                      "}");
    } else {
        iconPixmap = Utils::renderSVG(":/images/search_light.svg", QSize(14, 14));
        setStyleSheet("#QueryEdit {"
                      "border: none;"
                      "border-radius: none;"
                      "background: #EDEDED;"
                      "}"
                      "QListView {"
                      "border: 1px solid #2CA7F8;"
                      "background: white;"
                      "}");
    }

    m_iconLabel->setPixmap(iconPixmap);
}

void QueryEdit::handleSuggest(const QStringList &list)
{
    const int rowCount = list.count();

    m_listModel->setStringList(list);
    m_listView->setModel(m_listModel);

    if (rowCount == 0) {
        m_listView->hide();
        return;
    }

    // adjust the height of listview.
    switch (rowCount) {
    case 1:
        m_listView->setFixedHeight(35 + 5);
        break;

    case 2:
        m_listView->setFixedHeight(35 * 2 + 5);
        break;

    case 3:
        m_listView->setFixedHeight(35 * 3 + 5);
        break;

    case 4:
        m_listView->setFixedHeight(35 * 4 + 5);
        break;
    }

    m_listView->setFixedWidth(width() - 20);
    QPoint p(0, height());
    int x = mapToGlobal(p).x() + 20;
    int y = mapToGlobal(p).y() + 1;
    m_listView->move(x, y);
    m_listView->show();
}
