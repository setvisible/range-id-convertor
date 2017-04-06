/* - Range ID Convertor - Copyright (C) 2017 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "verticaltoolbar.h"
#include "ui_verticaltoolbar.h"

#include <Globals>

#include <QtWidgets/QAction>

VerticalToolBar::VerticalToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerticalToolBar)
{
    ui->setupUi(this);
    this->setWindowTitle(STR_APPLICATION_NAME);

    auto pbs = allPushButtons();
    foreach(auto *pb, pbs) {
        pb->setMinimumHeight(40);
    }

}

VerticalToolBar::~VerticalToolBar()
{
    delete ui;
}

void VerticalToolBar::setAddButton(const QAction *action)
{
    connectAction(ui->pushButton_add, action);
}

void VerticalToolBar::setRemoveButton(const QAction *action)
{
    connectAction(ui->pushButton_remove, action);
}

void VerticalToolBar::setBooleanButton(const QAction *action)
{
    connectAction(ui->pushButton_boolean, action);
}

void VerticalToolBar::setCopyButton(const QAction *action)
{
    connectAction(ui->pushButton_copy, action);
}

void VerticalToolBar::setCopyAllButton(const QAction *action)
{
    connectAction(ui->pushButton_copyAll, action);
}

void VerticalToolBar::setPackButton(const QAction *action)
{
    connectAction(ui->pushButton_pack, action);
}

void VerticalToolBar::setUnpackButton(const QAction *action)
{
    connectAction(ui->pushButton_unpack, action);
}
void VerticalToolBar::setClearButton(const QAction *action)
{
    connectAction(ui->pushButton_clear, action);
}

void VerticalToolBar::connectAction(QPushButton *button, const QAction *action)
{
    button->setWhatsThis(action->whatsThis());
    button->setStatusTip(action->statusTip());
    this->connect(button, SIGNAL(released()), action, SLOT(trigger()));
}

QList<QPushButton *> VerticalToolBar::allPushButtons()
{
    return this->findChildren<QPushButton *>(QString(), Qt::FindDirectChildrenOnly);
}

