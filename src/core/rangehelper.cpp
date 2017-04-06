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

#include "rangehelper.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDebug>

/*!
 * \class RangeHelper
 * \brief The RangeHelper class converts a Range into a QString.
 *
 * RangeHelper is a singleton, you can call it from every where in the code.
 * To do it, you can get the instance with the instance() method.
 *
 * \code
 *   // call it anywhen during the life of the QApplication
 *   Range range(...);
 *   RangeHelper *rh = RangeHelper::instance();
 *   QString str = rh->toPackedString( range );
 * \endcode
 */


RangeHelper* RangeHelper::m_instance = Q_NULLPTR;

RangeHelper::RangeHelper()
{
}

RangeHelper::~RangeHelper()
{
    m_instance = Q_NULLPTR; // needed for unit test
}

/*!
 * \brief Gets the unique instance.
 */
RangeHelper* RangeHelper::instance()
{
    if (!m_instance)
        m_instance = new RangeHelper();
    return m_instance;
}

/*!
 * \brief [FOR UNIT TEST ONLY] Delete the singleton.
 * \todo maybe just clean the libraries ?
 */
void RangeHelper::deleteInstance()
{
    if (m_instance) {
        delete m_instance;
        m_instance = Q_NULLPTR;
    }
}

/*!
 * \brief Converts a Range into packed string (=canonical form).
 */
QString RangeHelper::toPackedString(const Range &range) const
{
    if (range.isEmpty())
        return QString();

    const Identifier from = range.from();
    const Identifier to = range.to();
    const int by = range.by();

    if (from == to) {
        return QString("%0").arg(from);
    }

    if (by == 1) {
        return QString("%0:%1").arg(from).arg(to);
    }

    return QString("%0:%1:%2").arg(from).arg(to).arg(by);
}

/*!
 * \brief Converts a Range into unpacked string (=expanded identifiers).
 */
QStringList RangeHelper::toUnpackedStringList(const Range &range) const
{
    QStringList res;

    if (range.isEmpty())
        return res;

    const Identifier from = range.from();
    const Identifier to = range.to();
    const int by = range.by();

    for (int i = from; i <= to; i += by) {
        res.append(QString("%0").arg(i));
    }

    return res;
}
