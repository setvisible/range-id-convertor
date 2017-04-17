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

#include "rangelist.h"

#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QtCore/QSet>
#include <algorithm>

RangeList::RangeList()
{
}

/*!
 * \brief Clear the range list.
 */
void RangeList::clear()
{
    m_canonicalRanges.clear();
}

/*!
 * \brief Returns the number of unique identifiers in the RangeList.
 */
int RangeList::count() const
{
    int count = 0;
    foreach (auto item, m_canonicalRanges) {
        count += item.count();
    }
    return count;
}

/*!
 * \brief [FOR UNIT TEST ONLY] Returns the number of the ranges.
 *
 * \code
 *   // assuming RangeList ranges = { "1:5:1", "8", "9" }
 *   ranges.count();       // returns 7 (contains 7 identifiers: 1,2,3,4,5,8,9)
 *   ranges.countRanges(); // returns 3 (contains 3 ranges)
 * \endcode
 *
 */
int RangeList::countRanges() const
{
    return m_canonicalRanges.count();
}

/*!
 * \brief Returns the packed (canonical) ranges stored in this range list.
 */
inline QList<Range> RangeList::ranges() const
{
    return m_canonicalRanges;
}


/***********************************************************************************
 ***********************************************************************************/
void RangeList::add(const RangeListPtr other)
{
    this->add(other->ranges());
}

void RangeList::add(const Range &range)
{
    QList<Range> ranges;
    ranges << range;
    this->add( ranges );
}

void RangeList::add(const QList<Range> &ranges)
{
    if (ranges.isEmpty())
        return;

    QSet<int> currentSet = _q_expand( m_canonicalRanges );
    QSet<int> addedSet = _q_expand( ranges );

    currentSet += addedSet;

    QList<Range> p = _q_collapse(currentSet);

    m_canonicalRanges.clear();
    m_canonicalRanges.append(p);
}

/***********************************************************************************
 ***********************************************************************************/
void RangeList::remove(const RangeListPtr other)
{
    this->remove(other->ranges());
}

void RangeList::remove(const Range &range)
{
    QList<Range> ranges;
    ranges << range;
    this->remove( ranges );
}

void RangeList::remove(const QList<Range> &ranges)
{
    if (ranges.isEmpty())
        return;

    QSet<int> currentSet = _q_expand( m_canonicalRanges );
    QSet<int> removedSet = _q_expand( ranges );

    currentSet -= removedSet;

    QList<Range> p = _q_collapse(currentSet);

    m_canonicalRanges.clear();
    m_canonicalRanges.append(p);
}

/***********************************************************************************
 ***********************************************************************************/
bool RangeList::operator==(const RangeList &other) const
{
    return m_canonicalRanges == other.ranges();
}

bool RangeList::operator!=(const RangeList &other) const
{
    return ((*this) == other) ? false : true;
}



/***********************************************************************************
 ***********************************************************************************/
/*!
 * \brief Expand the given \a ranges to an unsorted set of unique identifiers.
 *
 * Example:
 *    {"5" "10:12:1" "20" "25"} -> {5, 10, 11, 12, 20, 25}
 *
 * \remark
 * Since this method handles QSet<int>, it can take a notable time for adding large
 * ranges. Noticable issues appear with ranges containing more than 1 million of
 * identifiers (ex: "1:1000000").
 *
 * \sa _q_collapse()
 */
inline QSet<int> RangeList::_q_expand(const QList<Range> &ranges)
{
    QSet<int> res;
    foreach (auto item, ranges) {
        for (Identifier i = item.from(); i <= item.to(); i += item.by()) {
            res.insert(i);
        }
    }
    return res;
}

/***********************************************************************************
 ***********************************************************************************/
/*!
 * \brief Collapse the given \a identifiers.
 * Return a list of ranges corresponding to compacted \a identifiers.
 *
 * Example:
 *    {5, 10, 11, 12, 20, 25} -> {"5" "10:12:1" "20" "25"}
 *
 * \remark It must only collapse if at least 3 values are continuous.
 *
 * \sa _q_expand()
 */
inline QList<Range> RangeList::_q_collapse(const QSet<int> &identifiers)
{
    QList<Range> res;

    QList<int> list = identifiers.toList();
    if (list.isEmpty()) {
        return res;
    }

    qSort(list);
    // Rem: At this point, the list is sorted and duplicate-free.

    const int count = list.count();

    int range_begin = 0;
    int range_end = 0;
    int range_step = 0;
    int previous_val_0 = -1;
    int val_0 = 0;
    int val_1 = 0;
    int val_2 = 0;
    int delta_1 = 0;
    int delta_2 = 0;

    for (int i = 0; i < count; ++i) {

        val_0 = list.at(i);

        if (val_0 == previous_val_0)   // Skip duplicate
            continue;

        previous_val_0 = val_0;

        if (i < count - 2) {

            val_1 = list.at(i+1);
            val_2 = list.at(i+2);
            delta_1 = val_1 - val_0;
            delta_2 = val_2 - val_1;

            if ( delta_2 == delta_1 ) {
                if ( range_begin == 0) {
                    range_begin = val_0;
                }
                range_end  = val_2;
                range_step = delta_2;
                continue;
            }
        }

        if (range_begin != 0) { // Compact insertion

            Range r(range_begin, range_end, range_step);
            res << r;
            range_begin = 0;
            i++;

        } else { // Normal insertion

            Range r(val_0, val_0, 1);
            res << r;

        }
    }
    return res;
}
