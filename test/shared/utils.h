/* - Range ID Convertor - Copyright (C) 2017 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_H
#define UTILS_H

#include <Core/Range>
#include <Core/RangeList>

#include <QtCore/QList>
#include <QtCore/QRegularExpression>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QDebug>

namespace Tests {

namespace Utils {

/*static inline QList<Range> toRangeList_OBSOLETE(const QString &str)
{
    QList<Range> ranges;
    QStringList items = str.split(' ', QString::SkipEmptyParts);
    foreach (auto item, items) {
        ranges.append( Range(item) );
    }
    return ranges;
}*/

static inline RangeListPtr toRangeList(const QString &str)
{
    QList<Range> ranges;
    QRegularExpression rx("^(?<from>\\d+)(:(?<to>\\d+)(:(?<by>[+-]?\\d+))?)?$");

    QStringList items = str.split(' ', QString::SkipEmptyParts);
    foreach (auto item, items) {

        QRegularExpressionMatch match = rx.match(item);
        Q_ASSERT(match.hasMatch() && "String input must match formats """
                                     "'from', 'from:to', or 'from:to:by'.");
        if (match.hasMatch()) {
            int from = match.captured("from").toInt();
            int to = match.captured("to").toInt();
            int by = match.captured("by").toInt();
            ranges.append( Range(from, to, by) );
        }
    }
    RangeListPtr res(new RangeList);
    foreach (auto range, ranges) {
        res->add( Range(range) );
    }
    if (res->countRanges() != ranges.count()) {
        QString msg = QString("\n\n\n"
                              "*** FATAL ERROR DETECTED!\n"
                              "*** Bad range!\n"
                              "*** --> Ranges must be sorted without duplicates nor overlaps!\n"
                              "*** Please check the input string:\n"
                              "'%0'\n\n\n").arg(str);
        Q_ASSERT_X(false, Q_FUNC_INFO, msg.toLatin1().constData());
    }
    return res;
}

static inline QSet<int> toIntSet(const QString &str)
{
    QSet<int> res;
    QStringList list = str.split(' ', QString::SkipEmptyParts);
    foreach (const QString item, list) {
        bool ok;
        int value = item.toInt(&ok);
        Q_ASSERT(ok && "this is not an integer");
        res.insert( value );
    }
    return res;
}

} // end namespace Utils

} // end namespace Tests

#endif // UTILS_H
