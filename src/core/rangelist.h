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

#ifndef RANGELIST_H
#define RANGELIST_H

#include "range.h"

#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>

class RangeList;
typedef QSharedPointer<RangeList> RangeListPtr;

class RangeList
{
public:
    explicit RangeList();

    inline QList<Range> ranges() const { return m_ranges; }

    void clear() {  m_ranges.clear(); }
    int count() const;
    int countRanges() const; /* for TESTS purpose */

    /* BOOLEAN OPERATION */
    bool add(const RangeListPtr other);
    bool add(const Range &range);
    bool add(const QList<Range> ranges);

    bool remove(const RangeListPtr other);
    bool remove(const Range &range);
    bool remove(const QList<Range> ranges);


    bool operator==(const RangeList &other) const;
    bool operator!=(const RangeList &other) const;

    //  Range *first() const { return m_first; }
    //  Range *constBegin() const { return m_first; }
    //  Range *constEnd() const { return m_last; }

protected:
    static inline QSet<int> _q_expand(const QList<Range> ranges);
    static inline QList<Range> _q_collapse(const QSet<int> &identifiers);

private:
    /* CANONICAL RANGES */
    // Returns the canonical ranges, i.e. the shortest sorted
    // list of ranges without duplicates identifiers.
    QList<Range> m_ranges;

};





#endif // RANGELIST_H
