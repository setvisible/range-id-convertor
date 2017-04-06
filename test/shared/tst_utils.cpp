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

#include <QtTest/QtTest>
#include <QtCore/QDebug>

#include "utils.h"

class tst_Utils : public QObject
{
    Q_OBJECT
private slots:
    void test_toRangeList();
    void test_toIntSet();

    /// \todo toIntSet : test ASSERT
};

/*************************************************************************
 *************************************************************************/
void tst_Utils::test_toRangeList()
{
    // Given
    QList<Range> expected;
    expected << Range(1);
    expected << Range(2);
    expected << Range(3);
    expected << Range(4);
    expected << Range(10,20,5);
    expected << Range(300,321);
    expected << Range(666,660,-2);

    // When
    RangeListPtr actual
            = Tests::Utils::toRangeList(
                QLatin1String("1 2 3 4 10:20:5     300:321 666:660:-2"));

    // Then
    QCOMPARE(actual->ranges(), expected);
}

/*************************************************************************
 *************************************************************************/
void tst_Utils::test_toIntSet()
{
    // Given
    QSet<int> expected;
    expected << 1 << 2 << 3 << 4 << 10 << 20 << 300 << 321 << -66 << 0;

    // When
    QSet<int>  actual
            = Tests::Utils::toIntSet(
                QLatin1String("1 2 3 4 10 20     300 321 -66 0"));

    // Then
    QCOMPARE(actual, expected);
}

/*************************************************************************
 *************************************************************************/

QTEST_APPLESS_MAIN(tst_Utils)

#include "tst_utils.moc"

