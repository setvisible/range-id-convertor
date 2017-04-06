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

#include <Core/Range>

class tst_Range : public QObject
{
    Q_OBJECT
private slots:
    void test_count_data();
    void test_count();
    void test_isEmpty();
    void test_clear();

    /* Getters & Setters*/
    void test_getters_setters();

    /* Booleans */
    void test_equals();

};

/*************************************************************************
 *************************************************************************/
void tst_Range::test_count_data()
{
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("to");
    QTest::addColumn<int>("by");
    QTest::addColumn<int>("expected");

    QTest::newRow("from to zero") << 0    << 0 << 0 << 0;
    QTest::newRow("from to zero") << -1   << 0 << 0 << 0;
    QTest::newRow("from to zero") << -999 << 0 << 0 << 0;
    QTest::newRow("from to zero") << 1    << 0 << 0 << 1;
    QTest::newRow("from to zero") << 2    << 0 << 0 << 1;
    QTest::newRow("from to zero") << 999  << 0 << 0 << 1;

    QTest::newRow("from to ") << 1    << -1  << 0 << 1;
    QTest::newRow("from to ") << 1    << -99 << 0 << 99;

    QTest::newRow("zero to") << 0 << 0    << 0 << 0;
    QTest::newRow("zero to") << 0 << 1    << 0 << 0;
    QTest::newRow("zero to") << 0 << 999  << 0 << 0;
    QTest::newRow("zero to") << 0 << -1   << 0 << 0;
    QTest::newRow("zero to") << 0 << -999 << 0 << 0;

    QTest::newRow("zero by") << 0 << 0 <<   0  << 0;
    QTest::newRow("zero by") << 0 << 0 <<   1  << 0;
    QTest::newRow("zero by") << 0 << 0 <<  99  << 0;
    QTest::newRow("zero by") << 0 << 0 <<  -1  << 0;
    QTest::newRow("zero by") << 0 << 0 << -99  << 0;

    QTest::newRow("zero to") << 0 << 0    << 0 << 0;
    QTest::newRow("zero to") << 0 << 1    << 0 << 0;
    QTest::newRow("zero to") << 0 << 999  << 0 << 0;
    QTest::newRow("zero to") << 0 << -1   << 0 << 0;
    QTest::newRow("zero to") << 0 << -999 << 0 << 0;

    QTest::newRow("1 to 1 by zero")     << 1 << 1    <<   0 << 1;
    QTest::newRow("1 to 1 by negative") << 1 << 1    <<  -1 << 1;
    QTest::newRow("1 to 1 by negative") << 1 << 1    <<  -2 << 1;
    QTest::newRow("1 to 1 by negative") << 1 << 1    << -99 << 1;
    QTest::newRow("1 to 1 by positive") << 1 << 1    <<   1 << 1;
    QTest::newRow("1 to 1 by positive") << 1 << 1    <<   2 << 1;
    QTest::newRow("1 to 1 by positive") << 1 << 1    <<  99 << 1;

    QTest::newRow("3 to 15 by zero")     << 3 << 15    <<   0 << 13;
    QTest::newRow("3 to 15 by negative") << 3 << 15    <<  -1 << 13;
    QTest::newRow("3 to 15 by negative") << 3 << 15    <<  -2 << 7;
    QTest::newRow("3 to 15 by negative") << 3 << 15    <<  -3 << 5; // == [3 6 9 12 15]
    QTest::newRow("3 to 15 by negative") << 3 << 15    <<  -5 << 3; // == [3 8 13]
    QTest::newRow("3 to 15 by negative") << 3 << 15    << -99 << 1;
    QTest::newRow("3 to 15 by positive") << 3 << 15    <<   1 << 13;
    QTest::newRow("3 to 15 by positive") << 3 << 15    <<   2 << 7;
    QTest::newRow("3 to 15 by positive") << 3 << 15    <<   3 << 5;
    QTest::newRow("3 to 15 by positive") << 3 << 15    <<   5 << 3;
    QTest::newRow("3 to 15 by positive") << 3 << 15    <<  99 << 1;


}

void tst_Range::test_count()
{
    // Given
    QFETCH(int, from);
    QFETCH(int, to);
    QFETCH(int, by);
    QFETCH(int, expected);
    Range target(from, to, by);

    // When
    int actual = target.count();

    // Then
    QCOMPARE( actual, expected );
}

/*************************************************************************
 *************************************************************************/
void tst_Range::test_isEmpty()
{
    Range range1(2, 10, 2);
    QCOMPARE( range1.isEmpty(), false );

    Range range2;
    QCOMPARE( range2.isEmpty(), true );
}


/*************************************************************************
 *************************************************************************/
void tst_Range::test_clear()
{
    Range target(2, 10, 2);
    QCOMPARE( target.count(), 5 );
    target.clear();
    QCOMPARE( target.count(), 0 );
}

/*************************************************************************
 *************************************************************************/
void tst_Range::test_getters_setters()
{
    Range target;
    QCOMPARE( target.from(), 0 );
    QCOMPARE( target.to(), 0 );
    QCOMPARE( target.by(), 1 );

    target.setRange(2, 10, 3); // == [2 5 8]

    QCOMPARE( target.from(), 2 );
    QCOMPARE( target.to(), 8 );
    QCOMPARE( target.by(), 3 );
}


/*************************************************************************
 *************************************************************************/
void tst_Range::test_equals()
{
    Range r1, r2;
    QVERIFY( r1 == r2 );
    QCOMPARE( r1, r2 );

    r1.setRange(3);
    r2.setRange(3);
    QVERIFY( r1 == r2 );

    r1.setRange(3);
    r2.clear();
    QVERIFY( r1 != r2 );

    r1.clear();
    r2.clear();
    QVERIFY( r1 == r2 );

    r1.setRange(2, 10, 3); // == [2 5 8]
    r2.setRange(8, 1, -3); // == [2 5 8]
    QVERIFY( r1 == r2 );

}


QTEST_APPLESS_MAIN(tst_Range)

#include "tst_range.moc"

