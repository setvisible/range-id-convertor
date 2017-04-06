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


/*!
 * \class RangeListModel
 * \brief A simple model that uses a RangeList as its data source.
 *
 * The RangeListModel class provides a model that supplies ranges to views.
 */

RangeListModel::RangeListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_packed(true)
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
    return lst.count();
}

/*!
 * \reimp
 */
QVariant RangeListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= lst.size())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return lst.at(index.row());
    return QVariant();
}

/***********************************************************************************
 ***********************************************************************************/
void RangeListModel::setPacked(bool packed)
{
    if (m_packed == packed)
        return;

    emit beginResetModel();
    m_packed = packed;
    _q_synchonize();
    emit endResetModel();
}

bool RangeListModel::isPacked() const
{
    return m_packed;
}

void RangeListModel::_q_synchonize()
{
    RangeHelper *rh = RangeHelper::instance();
    lst.clear();
    foreach (auto range, m_ranges.ranges()) {
        if (m_packed) {
            QString str = rh->toPackedString( range );
            lst.append( str );
        } else {
            QStringList strlist = rh->toUnpackedStringList( range );
            lst.append( strlist );
        }
    }
}

void RangeListModel::clear()
{
    emit beginResetModel();
    m_ranges.clear();
    _q_synchonize();
    emit endResetModel();
    emit countChanged(m_ranges.count());
}

void RangeListModel::add(const QString &text)
{
    if (!text.isEmpty()) {
        emit beginResetModel();
        Parser *p = Parser::instance();
        const RangeListPtr range_list = p->parse( text );
        m_ranges.add( range_list );
        _q_synchonize();
        emit endResetModel();
        emit countChanged(m_ranges.count());
    }
}

void RangeListModel::remove(const QString &text)
{
    if (!text.isEmpty()) {
        emit beginResetModel();
        Parser *p = Parser::instance();
        const RangeListPtr range_list = p->parse( text );
        m_ranges.remove( range_list );
        _q_synchonize();
        emit endResetModel();
        emit countChanged(m_ranges.count());
    }
}

