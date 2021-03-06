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

    void clear();
    int count() const;
    int countRanges() const;

    QList<Range> ranges() const;

    void add(const RangeListPtr other);
    void add(const Range &range);
    void add(const QList<Range> &ranges);

    void remove(const RangeListPtr other);
    void remove(const Range &range);
    void remove(const QList<Range> &ranges);

    bool operator==(const RangeList &other) const;
    bool operator!=(const RangeList &other) const;

protected:
    static QSet<int> _q_expand(const QList<Range> &ranges);
    static QList<Range> _q_collapse(const QSet<int> &identifiers);

private:
    QList<Range> m_canonicalRanges; ///< Canonical ranges.
    ///  This is the shortest sorted list of
    ///  ranges without duplicates identifiers.

};

#endif // RANGELIST_H
