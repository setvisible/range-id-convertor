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

#include <Core/RangeHelper>

class tst_RangeHelper : public QObject
{
    Q_OBJECT
private slots:
    void test_toPackedString_data();
    void test_toPackedString();

    void test_toUnpackedStringList_data();
    void test_toUnpackedStringList();

};

/*************************************************************************
 *************************************************************************/
void tst_RangeHelper::test_toPackedString_data()
{
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("to");
    QTest::addColumn<int>("by");
    QTest::addColumn<QString>("expected");

    QTest::newRow("empty") << 0 << 0 << 1 << "";
    QTest::newRow("single") << 10 << 0 << 0 << "10";
    QTest::newRow("single") << 10 << 10 << 0 << "10";
    QTest::newRow("single") << 10 << 10 << 1 << "10";
    QTest::newRow("range") << 3 << 5 << 1 << "3:5";
    QTest::newRow("range") << 3 << 5 << 2 << "3:5:2";
    QTest::newRow("negative by") << 50 << 40 << -1 << "40:50";
}

void tst_RangeHelper::test_toPackedString()
{
    // Given
    QFETCH(int, from);
    QFETCH(int, to);
    QFETCH(int, by);
    QFETCH(QString, expected);
    Range range(from, to, by);

    // When
    RangeHelper *rh = RangeHelper::instance();
    QString actual = rh->toPackedString( range );

    // Then
    QCOMPARE( actual, expected );
}

/*************************************************************************
 *************************************************************************/
void tst_RangeHelper::test_toUnpackedStringList_data()
{
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("to");
    QTest::addColumn<int>("by");
    QTest::addColumn<QString>("expected");

    QTest::newRow("empty") << 0 << 0 << 1 << "";
    QTest::newRow("single") << 10 << 0 << 0 << "10";
    QTest::newRow("single") << 10 << 10 << 0 << "10";
    QTest::newRow("single") << 10 << 10 << 1 << "10";
    QTest::newRow("range") << 3 << 5 << 1 << "3 4 5";
    QTest::newRow("range") << 3 << 5 << 2 << "3 5";
    QTest::newRow("negative by") << 50 << 40 << -2 << "40 42 44 46 48 50";

}

void tst_RangeHelper::test_toUnpackedStringList()
{
    // Given
    QFETCH(int, from);
    QFETCH(int, to);
    QFETCH(int, by);
    QFETCH(QString, expected);
    Range range(from, to, by);
    QStringList list = expected.split(' ', QString::SkipEmptyParts);

    // When
    RangeHelper *rh = RangeHelper::instance();
    QStringList actual = rh->toUnpackedStringList( range );

    // Then
    QCOMPARE( actual, list );
}



QTEST_APPLESS_MAIN(tst_RangeHelper)

#include "tst_rangehelper.moc"

