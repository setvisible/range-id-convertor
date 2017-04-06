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

#include "range.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QString>


Range::Range(const Identifier _from,
             const Identifier _to, const int _by)
    : m_from(_from)
    , m_to(_to)
    , m_by(_by)
{
    this->simplify();
}

bool Range::isEmpty() const
{
    return (m_from == 0);
}

void Range::clear()
{
    m_from = 0;
    m_to = 0;
    m_by = 1;

    Q_ASSERT(isEmpty());
    Q_ASSERT(count() == 0);
}


/*!
 * \brief Returns the number of unique identifiers in the Range.
 */
int Range::count() const
{
    if (m_from == 0) /* Empty range */
        return 0;
    Q_ASSERT( m_from >= 0 );
    Q_ASSERT( m_to >= m_from );
    Q_ASSERT( m_by > 0 );
    Q_ASSERT( m_by != 0 );
    return ((m_to - m_from) / m_by) + 1;
}


/***********************************************************************************
 ***********************************************************************************/
void Range::setRange(const Identifier _from,
                     const Identifier _to, const int _by)
{
    m_from = _from;
    m_to = _to;
    m_by = _by;
    this->simplify();
}

/***********************************************************************************
 ***********************************************************************************/
bool Range::operator==(const Range &other) const
{
    return (*this).m_from == other.m_from
            && (*this).m_to == other.m_to
            && (*this).m_by == other.m_by;
}

bool Range::operator!=(const Range &other) const
{
    return ((*this) == other) ? false : true;
}


/***********************************************************************************
 ***********************************************************************************/
void Range::simplify()
{
    if (m_from <= 0) { /* Makes it an empty range */
        clear();
        return;
    }

    if (m_to < 0) {
        m_to *= -1;
    }

    if (m_to == 0) {  /* Makes it a single-value range */
        m_to = m_from;
        m_by = 1;
        Q_ASSERT(count() == 1);
        return;
    }

    if (m_by == 0) {
        m_by = 1;
    }

    if (m_from > m_to ) {
        /* Change direction */
        int tmp = m_from;
        m_from = m_to;
        m_to = tmp;

        if (m_by < 0) {
            m_by *= -1;
        }
        Q_ASSERT(m_by!=0);
        int s = (m_to - m_from) / m_by;
        Q_ASSERT(s>=0);
        m_from = m_to - s * m_by;

    } else {

        if (m_by < 0) {
            m_by *= -1;
        }
        Q_ASSERT(m_by!=0);
        int s = (m_to - m_from) / m_by;
        Q_ASSERT(s>=0);
        m_to = m_from + s * m_by;

    }
    m_by = qMin(m_by, m_to-m_from);

    if (m_by == 0) {
        m_by = 1;
    }

    /* Post-Conditions */
#ifdef QT_DEBUG
    QString msg = QString("range = %0 %1 %2").arg(m_from).arg(m_to).arg(m_by);
    const char* _info = msg.toLatin1().constData();
    Q_ASSERT_X( m_from >= 0     , Q_FUNC_INFO, _info);
    Q_ASSERT_X( m_to >= m_from  , Q_FUNC_INFO, _info);
    Q_ASSERT_X( m_by > 0        , Q_FUNC_INFO, _info);
#endif

}

