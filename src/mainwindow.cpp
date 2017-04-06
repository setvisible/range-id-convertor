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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "globals.h"

#include <Core/RangeListModel>
#include <GUI/BooleanDialog>
#include <GUI/VerticalToolBar>

#include <QtCore/QDebug>
#include <QtGui/QCloseEvent>
#include <QtGui/QClipboard>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_rangeListModel(new RangeListModel(this))
{
    ui->setupUi(this);
    this->setWindowTitle(STR_APPLICATION_NAME);
    this->setWindowIcon(QIcon(":/icons/logo/logo_24x24.png"));

    createActions();
    createMenus();
    createContextMenu();

    ui->listView->setModel(m_rangeListModel);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(m_rangeListModel, SIGNAL(countChanged(int)), this, SLOT(updateCounterText(int)));


    ui->comboBoxOutput->setCurrentIndex(2);
    ui->comboBoxOutput->setEnabled(false);

#ifdef QT_DEBUG
    this->setWindowTitle(QString("%0 - **debug mode**").arg(STR_APPLICATION_NAME));
    m_rangeListModel->add("1001");
    m_rangeListModel->add("1002");
    m_rangeListModel->add("1009");
    m_rangeListModel->add("2001:2202:15");
    m_rangeListModel->add("999999");
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
    /// \todo store settings
    event->accept();
}


/***********************************************************************************
 ***********************************************************************************/
void MainWindow::add()
{
    Q_ASSERT(m_rangeListModel);
    QString text = QInputDialog::getMultiLineText(this, STR_APPLICATION_NAME, "IDs:");
    m_rangeListModel->add(text);
}

void MainWindow::remove()
{
    Q_ASSERT(m_rangeListModel);
    QString text = QInputDialog::getMultiLineText(this, STR_APPLICATION_NAME, "IDs:");
    m_rangeListModel->remove(text);
}

void MainWindow::removeSelected()
{
    Q_ASSERT(m_rangeListModel);
    QItemSelectionModel *selectionModel = ui->listView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QString text;
    foreach(index, indexes) {
        text += index.data().toString();
        text += ' ';
    }
    m_rangeListModel->remove(text);

}

void MainWindow::boolean()
{    
    QMessageBox msgBox(QMessageBox::NoIcon, QLatin1String("Info"),
                       QLatin1String("Not implemented yet"));
    msgBox.exec();
}


/***********************************************************************************
 ***********************************************************************************/
void MainWindow::selectAll()
{
    ui->listView->selectAll();
}

void MainWindow::copy()
{
    QString text;
    QItemSelectionModel *selectionModel = ui->listView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    foreach(index, indexes) {
        text += index.data().toString();
        text += '\n';
    }
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

void MainWindow::copyAll()
{
    QItemSelectionModel *selectionModel = ui->listView->selectionModel();
    QItemSelection selection = selectionModel->selection();
    this->selectAll();
    this->copy();
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    ui->listView->setSelectionModel(selectionModel);
}


/***********************************************************************************
 ***********************************************************************************/
void MainWindow::clear()
{
    this->selectAll();
    this->removeSelected();
}


/***********************************************************************************
 ***********************************************************************************/
void MainWindow::pack()
{
    Q_ASSERT(m_rangeListModel);
    m_rangeListModel->setPacked(true);
}

void MainWindow::unpack()
{
    Q_ASSERT(m_rangeListModel);
    m_rangeListModel->setPacked(false);
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::about()
{
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About %0").arg(STR_APPLICATION_NAME),
                       aboutHtml());
    msgBox.exec();
}


/***********************************************************************************
 ***********************************************************************************/
void MainWindow::createActions()
{
    ui->action_Exit->setShortcuts(QKeySequence::Quit);
    ui->action_Exit->setStatusTip(tr("Quit %0").arg(STR_APPLICATION_NAME));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));

    ui->action_Add->setShortcuts(QKeySequence::Paste);
    ui->action_Add->setStatusTip(tr("Add IDs..."));
    connect(ui->action_Add, SIGNAL(triggered()), this, SLOT(add()));

    ui->action_Remove->setStatusTip(tr("Remove IDs..."));
    connect(ui->action_Remove, SIGNAL(triggered()), this, SLOT(remove()));

    ui->action_RemoveSelected->setShortcuts(QKeySequence::Delete);
    ui->action_RemoveSelected->setStatusTip(tr("Remove Selected IDs"));
    connect(ui->action_RemoveSelected, SIGNAL(triggered()), this, SLOT(removeSelected()));

    ui->action_Boolean->setStatusTip(tr("Boolean Operation..."));
    connect(ui->action_Boolean, SIGNAL(triggered()), this, SLOT(boolean()));

    ui->action_SelectAll->setShortcuts(QKeySequence::SelectAll);
    ui->action_SelectAll->setStatusTip(tr("Select All"));
    connect(ui->action_SelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));

    ui->action_CopySelected->setShortcuts(QKeySequence::Copy);
    ui->action_CopySelected->setStatusTip(tr("Copy Selected IDs"));
    connect(ui->action_CopySelected, SIGNAL(triggered()), this, SLOT(copy()));

    ui->action_CopyAll->setStatusTip(tr("Copy All"));
    connect(ui->action_CopyAll, SIGNAL(triggered()), this, SLOT(copyAll()));

    ui->action_Clear->setShortcuts(QKeySequence::New);
    ui->action_Clear->setStatusTip(tr("Clear All"));
    connect(ui->action_Clear, SIGNAL(triggered()), this, SLOT(clear()));

    ui->action_Pack->setStatusTip(tr("Pack IDs"));
    connect(ui->action_Pack, SIGNAL(triggered()), this, SLOT(pack()));

    ui->action_Unpack->setStatusTip(tr("Unpack IDs"));
    connect(ui->action_Unpack, SIGNAL(triggered()), this, SLOT(unpack()));

    ui->action_About->setShortcuts(QKeySequence::HelpContents);
    ui->action_About->setStatusTip(tr("About %0").arg(STR_APPLICATION_NAME));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));

    ui->action_AboutQt->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F1));
    ui->action_AboutQt->setStatusTip(tr("About Qt"));
    connect(ui->action_AboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void MainWindow::createMenus()
{
    ui->verticalToolBar->setAddButton(ui->action_Add);
    ui->verticalToolBar->setRemoveButton(ui->action_Remove);
    ui->verticalToolBar->setBooleanButton(ui->action_Boolean);
    ui->verticalToolBar->setCopyButton(ui->action_CopySelected);
    ui->verticalToolBar->setCopyAllButton(ui->action_CopyAll);
    ui->verticalToolBar->setPackButton(ui->action_Pack);
    ui->verticalToolBar->setUnpackButton(ui->action_Unpack);
    ui->verticalToolBar->setClearButton(ui->action_Clear);

}

void MainWindow::createContextMenu()
{
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::updateCounterText(int count)
{
    ui->countLabel->setText(QString("Total %0 identifiers.").arg(count));
}

/***********************************************************************************
 ***********************************************************************************/
void MainWindow::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu;
    QAction *addButton          = contextMenu.addAction(tr("Add IDs..."));
    QAction *removeButton       = contextMenu.addAction(tr("Remove IDs..."));
    contextMenu.addSeparator();
    QAction *copyButton         = contextMenu.addAction(tr("Copy"));
    QAction *copyAllButton      = contextMenu.addAction(tr("Copy All"));
    contextMenu.addSeparator();
    QAction *selectAllButton    = contextMenu.addAction(tr("Select All"));
    contextMenu.addSeparator();
    QAction *removeSelButton    = contextMenu.addAction(tr("Remove"));

    QAction *action = contextMenu.exec(ui->listView->mapToGlobal(pos));

    if        (action == addButton         ) { add();
    } else if (action == removeButton      ) { remove();
    } else if (action == copyButton        ) { copy();
    } else if (action == copyAllButton     ) { copyAll();
    } else if (action == selectAllButton   ) { selectAll();
    } else if (action == removeSelButton   ) { removeSelected();
    }

}
