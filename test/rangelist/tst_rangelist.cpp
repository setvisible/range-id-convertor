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

#include <Core/RangeList>
#include "../shared/utils.h"

class tst_RangeList : public QObject
{
    Q_OBJECT
    void _q_common_data_for_expand_collapse();

private slots:
    /* White Box Tests */
    void test_expand();
    void test_expand_data();
    void test_collapse();
    void test_collapse_data();

    /* Black Box Tests */
    void test_clear();
    void test_count_data();
    void test_count();
    void test_countRanges_data();
    void test_countRanges();

    void test_add_simple();
    void test_add_unsorted();
    void test_add_duplicated();
    void test_add_overlapping();
    void test_add_big_ranges();

    void test_remove_empty();
    void test_remove_simple();
    void test_remove();
    void test_remove_big();
    void test_remove_big_fragmented();

    void test_equals();
    void test_equals_2();

};

class FriendlyRangeList : public RangeList { friend class tst_RangeList; };

/*************************************************************************
 *************************************************************************/
void tst_RangeList::_q_common_data_for_expand_collapse()
{
    /* ************************************************ */
    /* REMARK                                           */
    /* ************************************************ */
    /* This test set is similar for                     */
    /*  - test_collapse_data()                          */
    /*  - test_expand_data()                            */
    /* ************************************************ */

    /* DATA: ... << sorted integers << sorted ranges */
    QTest::newRow("empty") << "" << "";

    QTest::newRow("simple") << "1" << "1";
    QTest::newRow("simple") << "1 2" << "1 2";
    QTest::newRow("simple") << "1 2 3" << "1:3";   // must only collapse if at least 3 values
    QTest::newRow("simple") << "1 2 3 4" << "1:4";
    QTest::newRow("simple") << "1 2 3 4 5" << "1:5";

    QTest::newRow("continuous") << "1 5" << "1 5";
    QTest::newRow("continuous") << "1 5 9" << "1:9:4";
    QTest::newRow("continuous") << "1 6 11" << "1:11:5";
    QTest::newRow("continuous") << "101 114 127 140" << "101:140:13";

    QTest::newRow("discontinued") << "10 15 20 21 22" << "10:20:5 21 22"; // not "10 15 20:22"
    QTest::newRow("discontinued") << "10 15 20 21 22 23" << "10:20:5 21:23";
    QTest::newRow("discontinued") << "1 5 6" << "1 5 6";
    QTest::newRow("discontinued") << "1 5 6 7" << "1 5:7";
    QTest::newRow("discontinued") << "1 5 7 8 9" << "1 5 7:9";

    QTest::newRow("keep_order") << "20 21 22 23 24 25" << "20:25";
    QTest::newRow("keep_order") << "10 20 21 22 23 24 25 30" << "10 20:25:1 30"; // not "10:30:10 21:24:1"
}

void tst_RangeList::test_expand_data()
{
    QTest::addColumn<QString>("expected_int");
    QTest::addColumn<QString>("input_ranges");
    this->_q_common_data_for_expand_collapse();

    QTest::newRow("unsorted") << "1 2 3 4 5 100 105 110 115 500" << " 1:5 100:115:5 500";
    QTest::newRow("duplicate") << "20 21 22 21 20 21 21 22" << "20:22";
}

void tst_RangeList::test_collapse_data()
{
    QTest::addColumn<QString>("input_int");
    QTest::addColumn<QString>("expected_ranges");
    this->_q_common_data_for_expand_collapse();

    QTest::newRow("unsorted") << "2 1" << "1 2";
    QTest::newRow("unsorted") << "3 2 1" << "1:3";
    QTest::newRow("unsorted") << "4 2 5 1 3" << "1:5";

    QTest::newRow("duplicate") << "20 20" << "20";
    QTest::newRow("duplicate") << "20 20 20" << "20";
    QTest::newRow("duplicate") << "20 20 20 20" << "20";
    QTest::newRow("duplicate") << "20 20 20 20 20" << "20";

    QTest::newRow("duplicate_ranges") << "20 20 21 21 22 22" << "20:22";

    QTest::newRow("duplicate_unsorted") << " 1 2 20 21 1 2 21 22 20 21 22 2 1" << " 1 2 20:22";

}

void tst_RangeList::test_expand()
{
    // Given
    QFETCH(QString, input_ranges);
    QFETCH(QString, expected_int);
    RangeListPtr input = Tests::Utils::toRangeList(input_ranges);
    QSet<int> expected = Tests::Utils::toIntSet(expected_int);

    // When
    QSet<int> actual = FriendlyRangeList::_q_expand(input->ranges());

    // Then
    QCOMPARE(actual, expected);
}
void tst_RangeList::test_collapse()
{
    // Given
    QFETCH(QString, input_int);
    QFETCH(QString, expected_ranges);
    QSet<int> input = Tests::Utils::toIntSet(input_int);
    RangeListPtr expected = Tests::Utils::toRangeList(expected_ranges);

    // When
    QList<Range> actual = FriendlyRangeList::_q_collapse(input);

    // Then
    QCOMPARE(actual, expected->ranges());
}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_clear()
{
    RangeList target;
    target.clear();
    QCOMPARE(target.count(), 0);

    target.add( Range(10, 12, 1) );
    QCOMPARE(target.count(), 3);

    target.clear();
    QCOMPARE(target.count(), 0);
}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_count_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("empty") << "" << 0;
    QTest::newRow("simple") << "15" << 1;
    QTest::newRow("simple") << "99999999" << 1;
    QTest::newRow("simple") << "100000000" << 1;

    QTest::newRow("range") << "20:30" << 11;
    QTest::newRow("range") << "50:54:2" << 3;
    QTest::newRow("range") << "60:66:3" << 3;

    QTest::newRow("several range") << "50:54:2 60:66:3" << 6;

    QTest::newRow("one million") << "1:1000000"  << 1000000;

}

void tst_RangeList::test_count()
{
    // Given
    QFETCH(QString, input);
    QFETCH(int, expected);
    RangeListPtr range = Tests::Utils::toRangeList(input);
    RangeList target;
    target.add( range );

    // When
    int actual = target.count();

    // Then
    QCOMPARE( actual, expected );
}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_countRanges_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("empty") << "" << 0;
    QTest::newRow("simple") << "15" << 1;
    QTest::newRow("simple") << "15 16" << 2;
    QTest::newRow("simple") << "15:17" << 1;
    QTest::newRow("simple") << "15:17 19" << 2;

    QTest::newRow("range") << "20:30" << 1;
    QTest::newRow("several range") << "50:54:2 60:66:3" << 2;

    QTest::newRow("one million") << "1:1000000"  << 1;
}

void tst_RangeList::test_countRanges()
{
    // Given
    QFETCH(QString, input);
    QFETCH(int, expected);
    RangeListPtr range = Tests::Utils::toRangeList(input);
    RangeList target;
    target.add( range );

    // When
    int actual = target.countRanges();

    // Then
    QCOMPARE( actual, expected );
}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_add_simple()
{
}

void tst_RangeList::test_add_unsorted()
{
    RangeList target;
    target.add( Range(30, 35, 1) );
    target.add( Range(20, 20, 1) );  // unsorted
    target.add( Range(10, 12, 1) );
    QList<Range> actual = target.ranges();
    QList<Range> expected;
    expected << Range(10, 12, 1);
    expected << Range(20, 20, 1); // sorted
    expected << Range(30, 35, 1);
    QCOMPARE(actual, expected);
}

void tst_RangeList::test_add_duplicated()
{
    RangeList target;
    target.add( Range(10, 14, 2) );
    target.add( Range(10, 20, 1) ); // must erase the previous range
    target.add( Range(15, 19, 2) );
    QList<Range> actual = target.ranges();
    QList<Range> expected;
    expected << Range(10, 20, 1);
    QCOMPARE(actual, expected);
}

void tst_RangeList::test_add_overlapping()
{
    /* INPUT                                              */
    /* 10-15-20-...-25---30------35-----40                */
    /*                   30-32-34-36-38-40-42-...-68-70   */
    /*                                                    */
    /* ExPECTED                                           */
    /* 10-15-20-...-25---30-32-34-36-38-40-42-...-68-70   */
    RangeList target;
    target.add( Range(10, 40, 5) );
    target.add( Range(30, 70, 2) ); // erase just a part of the previous range
    QList<Range> actual = target.ranges();
    QList<Range> expected;
    expected << Range(10, 30, 5);
    expected << Range(32, 32, 1);
    expected << Range(34, 36, 1);
    expected << Range(38, 70, 2);
    QCOMPARE(actual, expected);
}

void tst_RangeList::test_add_big_ranges()
{
    RangeList target;
    target.add( Range(5) );
    target.add( Range(10,999995,5) );
    QList<Range> actual = target.ranges();
    QList<Range> expected;
    expected << Range(5, 999995, 5);
    QCOMPARE(actual, expected);
}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_remove_empty()
{
    RangeList target;
    target.remove( Range(1, 1000000, 1) );
    QCOMPARE( target.count(), 0 );
}

void tst_RangeList::test_remove_simple()
{
    RangeList target;
    target.add( Range(15) );
    target.add( Range(66) );
    target.add( Range(155554) );
    target.remove( Range(1, 1000000, 1) );
    QCOMPARE( target.count(), 0 );
}

void tst_RangeList::test_remove()
{
    // Given
    RangeList target;
    target.add( Range(10, 99, 1) );

    // When
    target.remove( Range(45, 67, 1) );
    QList<Range> actual = target.ranges();

    // Then
    QList<Range> expected;
    expected << Range(10, 44, 1);
    expected << Range(68, 99, 1);
    QCOMPARE(actual, expected);

}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_remove_big()
{
    RangeList target;
    target.add( Range(1, 1000000, 1) );
    target.remove( Range(1, 1000000, 1) );
    QCOMPARE( target.count(), 0 );
}

/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_remove_big_fragmented()
{
    RangeList target;
    target.add(    Range(1, 1000000, 7) ); // ((1000000 - 1) / 7) + 1 == 142,858 entries
    QCOMPARE( target.count(), 142858 );
    target.remove( Range(5, 1000000, 5) ); // ((1000000 - 5) / 5) + 1 == 200,000 entries
    QCOMPARE( target.count(), 114286 );    // ((142,858 - 3) * 4/5 ) + 2 == 114,286 entries
}


/*************************************************************************
 *************************************************************************/
void tst_RangeList::test_equals()
{
    RangeList target1;
    target1.add( Range(20) );
    target1.add( Range(22) );
    target1.add( Range(24) );
    target1.add( Range(26) );
    RangeList target2;
    target2.add( Range(20, 26, 2) );
    QCOMPARE(target1, target2);
}

void tst_RangeList::test_equals_2()
{
    RangeList target1;
    target1.add( Range(3) );
    target1.add( Range(6,12,3) ); // 6 9 12
    RangeList target2;
    target2.add( Range(3, 12, 3) ); // 3 6 9 12
    QCOMPARE(target1, target2);
}

/*************************************************************************
 *************************************************************************/

QTEST_APPLESS_MAIN(tst_RangeList)

#include "tst_rangelist.moc"

