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

#include "rangelistmodel.h"

#include <Core/Parser>
#include <Core/Range>
#include <Core/RangeHelper>
#include <Core/RangeList>

RangeListModelPrivate::RangeListModelPrivate()
{

}


/*!
 * \class RangeListModel
 * \brief A simple model that uses a RangeList as its data source.
 *
 * The RangeListModel class provides a model that supplies ranges to views.
 */

RangeListModel::RangeListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_isPacked(true)
{
    this->clear();
}

/***********************************************************************************
 ***********************************************************************************/
/*!
 * \reimp
 */
int RangeListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_displayedList.count();
}

/*!
 * \reimp
 */
QVariant RangeListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_displayedList.size())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_displayedList.at(index.row());
    return QVariant();
}

/***********************************************************************************
 ***********************************************************************************/
void RangeListModel::setPacked(bool packed)
{
    if (m_isPacked == packed)
        return;

    emit beginResetModel();
    m_isPacked = packed;
    _q_synchonize();
    emit endResetModel();
}

bool RangeListModel::isPacked() const
{
    return m_isPacked;
}

void RangeListModel::clear()
{
    emit beginResetModel();
    m_internalRangeList.clear();
    _q_synchonize();
    emit endResetModel();
    emit countChanged(m_internalRangeList.count());
}

/***********************************************************************************
 ***********************************************************************************/
/*!
 * \brief Inserts the given \a text into the model.
 * \sa RangeListModel::remove()
 */
void RangeListModel::add(const QString &text)
{
    if (!text.isEmpty()) {
        emit beginResetModel();
        Parser *p = Parser::instance();
        const RangeListPtr parsedList = p->parse( text );
        m_internalRangeList.add( parsedList );
        _q_synchonize();
        emit endResetModel();
        emit countChanged(m_internalRangeList.count());
    }
}

/*!
 * \brief Removes the given \a text from the model.
 * \sa RangeListModel::add()
 */
void RangeListModel::remove(const QString &text)
{
    if (!text.isEmpty()) {
        emit beginResetModel();
        Parser *p = Parser::instance();
        const RangeListPtr parsedList = p->parse( text );
        m_internalRangeList.remove( parsedList );
        _q_synchonize();
        emit endResetModel();
        emit countChanged(m_internalRangeList.count());
    }
}

/***********************************************************************************
 ***********************************************************************************/
/*! \internal
 *  \brief Synchronize \a m_internalRangeList and \a m_displayedList.
 */
void RangeListModel::_q_synchonize()
{
    RangeHelper *rh = RangeHelper::instance();
    m_displayedList.clear();
    foreach (auto range, m_internalRangeList.ranges()) {
        if (m_isPacked) {
            QString str = rh->toPackedString( range );
            m_displayedList.append( str );
        } else {
            QStringList strlist = rh->toUnpackedStringList( range );
            m_displayedList.append( strlist );
        }
    }
}
