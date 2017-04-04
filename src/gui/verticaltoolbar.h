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


#ifndef VERTICALTOOLBAR_H
#define VERTICALTOOLBAR_H

#include <QtWidgets/QWidget>

class QAction;
class QPushButton;

namespace Ui {
class VerticalToolBar;
}

class VerticalToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit VerticalToolBar(QWidget *parent = 0);
    ~VerticalToolBar();

    void setAddButton(const QAction *action);
    void setRemoveButton(const QAction *action);
    void setBooleanButton(const QAction *action);
    void setCopyButton(const QAction *action);
    void setCopyAllButton(const QAction *action);
    void setPackButton(const QAction *action);
    void setUnpackButton(const QAction *action);
    void setClearButton(const QAction *action);

private:
    Ui::VerticalToolBar *ui;
    void connectAction(QPushButton *button, const QAction *action);
    QList<QPushButton *> allPushButtons();

};

#endif // VERTICALTOOLBAR_H
