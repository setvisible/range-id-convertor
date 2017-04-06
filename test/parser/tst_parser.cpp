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

#include <Core/Parser>
#include "../shared/utils.h"

class tst_Parser : public QObject
{
    Q_OBJECT
    void _q_trivial();
    void _q_simple();
    void _q_invalid();
    void _q_nastran_bulk_file();
    void _q_nastran_bulk_file_except();
    void _q_nastran_bulk_file_continuation();
    void _q_patran_gui();
    void _q_patran_session_group_file();
    void _q_abaqus_inp_file();
    void _q_femap_gui();
    void _q_hypermesh_gui();
    void _q_calc_excel_gui();

private slots:
    void test_parse();
    void test_parse_data();

};

void tst_Parser::test_parse_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("rangelist");
    this->_q_trivial();
    this->_q_simple();
    this->_q_invalid();
    this->_q_nastran_bulk_file();
    this->_q_nastran_bulk_file_except();
    this->_q_nastran_bulk_file_continuation();
    this->_q_patran_gui();
    this->_q_patran_session_group_file();
    this->_q_abaqus_inp_file();
    this->_q_femap_gui();
    this->_q_hypermesh_gui();
    this->_q_calc_excel_gui();
}

void tst_Parser::test_parse()
{
    // Given
    QFETCH(QString, input);
    QFETCH(QString, rangelist);
    RangeListPtr expected = Tests::Utils::toRangeList(rangelist);

    // When
    RangeListPtr actual = Parser::instance()->parse( input );

    // Then
    QCOMPARE( actual->ranges(), expected->ranges() );
}

/*************************************************************************
 *************************************************************************/
void tst_Parser::_q_trivial()
{
    QTest::newRow("empty")<< "" << "";
    QTest::newRow("whitespaces")<< " " << "";
    QTest::newRow("whitespaces")<< "\t" << "";
    QTest::newRow("whitespaces") << "\r" << "";
    QTest::newRow("whitespaces") << "\n" << "";
    QTest::newRow("whitespaces") << "\r\n" << "";

}

void tst_Parser::_q_simple()
{
    QTest::newRow("trivial") << "10" << "10";

    QTest::newRow("duplicate") << "10 10 10"        << "10";
    QTest::newRow("duplicate") << "10,10,10"        << "10";
    QTest::newRow("duplicate") << "10 THRU 10,10"   << "10";
    QTest::newRow("duplicate") << "10;10;10"        << "10";
    QTest::newRow("duplicate") << "10\n10\n10"      << "10";

    QTest::newRow("text") << "-1 10:20 texte123!?>|" << "10:20:1";
    QTest::newRow("text")<< "SET 1000 = ALL" << "1000";

    QTest::newRow("direction") << "20:15"      << "15:20";
    QTest::newRow("direction") << "20:15:-1"   << "15:20";
    QTest::newRow("direction") << "20:12:-3"   << "14:20:3";

    QTest::newRow("sign") << "15:20"   << "15:20";
    QTest::newRow("sign") << "15:20:1" << "15:20";
    QTest::newRow("sign") << "15:20:2" << "15:19:2";

    QTest::newRow("misformed range") << "15:20:0"    << "15:20";
    QTest::newRow("misformed range") << "15:20:-2"   << "15:19:2";
    QTest::newRow("misformed range") << "20:15:-99"  << "20";
    QTest::newRow("misformed range") << "15:20:-99"  << "15";

    QTest::newRow("dash separator") << "15-20" << "15:20";
    QTest::newRow("dash separator") << "20-15" << "15:20";

}

void tst_Parser::_q_invalid()
{
    QTest::newRow("alphanumeric") << "0" << "";
    QTest::newRow("alphanumeric") << "abcd" << "";
    QTest::newRow("alphanumeric") << "100a" << "";
    QTest::newRow("alphanumeric") << "10e2" << "";
    QTest::newRow("alphanumeric") << "   , / ty ?d kj tf" << "";

    QTest::newRow("decimal") << "1.201" << "1 201";
    QTest::newRow("decimal") << "1,201" << "1 201";
    QTest::newRow("decimal") << "111,3" << "3 111";
    QTest::newRow("decimal") << "111.3" << "3 111";
    QTest::newRow("decimal") << "999."  << "999";

    QTest::newRow("colons") << ":" << "";
    QTest::newRow("colons") << "15:" << "";

    QTest::newRow("negative sign") << "-0" << "";
    QTest::newRow("negative sign") << "+10" << "";
    QTest::newRow("negative sign") << "-15" << "";
    QTest::newRow("negative sign") << "+15:+20" << "";
    QTest::newRow("negative sign") << "15:+20" << "";
    QTest::newRow("negative sign") << "15:-20" << "";
    QTest::newRow("negative sign") << "15:-20:" << "";
    QTest::newRow("negative sign") << "15:-20:1" << "";
    QTest::newRow("negative sign") << "15:-20:-1" << "";
    QTest::newRow("negative sign") << "-15:20" << "";
    QTest::newRow("negative sign") << "-15:20:1" << "";
    QTest::newRow("negative sign") << "-15:20:-1" << "";

    QTest::newRow("wrong range") << "15:20:1:0" << "";
    QTest::newRow("wrong range") << ":20" << "";
    QTest::newRow("wrong range") << "::1" << "";
    QTest::newRow("wrong range") << ":20:1" << "";

    QTest::newRow("dash separator") << "-15-20" << "";
    QTest::newRow("dash separator") << "-15-19-2" << "";
    QTest::newRow("dash separator") << "15-19-2" << "";
    QTest::newRow("dash separator") << "15--20" << "";
    QTest::newRow("dash separator") << "15-" << "";
    QTest::newRow("dash separator") << "-" << "";

}

void tst_Parser::_q_nastran_bulk_file()
{
    QTest::newRow("nastran")<< "5THRU9" << "";
    QTest::newRow("nastran")<< "41THRU50BY3" << "";
    QTest::newRow("nastran")<< "41THRU50STEP3" << "";

    /* SET xx = */
    QTest::newRow("nastran")<< "1 THRU 100000" << "1:100000";
    QTest::newRow("nastran")<< "5 THRU 9" << "5:9";
    QTest::newRow("nastran")<< "5 thru 9" << "5:9";
    QTest::newRow("nastran")<< "5 tHRu 9" << "5:9";
    QTest::newRow("nastran")<< "41 THRU 50 BY 3" << "41:50:3";
    QTest::newRow("nastran")<< "41 thru 50 by 3" << "41:50:3";
    QTest::newRow("nastran")<< "41 tHRu 50 bY 3" << "41:50:3";
    QTest::newRow("nastran")<< "41 THRU 50 STEP 3" << "41:50:3";
    QTest::newRow("nastran")<< "41 thru 50 step 3" << "41:50:3";
    QTest::newRow("nastran")<< "41 tHRu 50 sTeP 3" << "41:50:3";
    QTest::newRow("nastran tabs")<< "41\tTHRU\t50\tBY\t3" << "41:50:3";
    QTest::newRow("nastran tabs")<< "41\tTHRU\t50\tSTEP\t3" << "41:50:3";
    QTest::newRow("nastran comma")<< "51 THRU 61 2 THRU 5, \n 77 9 THRU 15 " << "2:5 9:15 51:61 77";
    QTest::newRow("nastran") << "SET 1000 = 5, 6, 7, 8, 9, \n10 THRU 55" << "5:55 1000";

    /* COMMA SEPARATED FORMAT */
    QTest::newRow("nastran") << "100,THRU,102,109,110,115,THRU,131,BY,8"
                             << "100:102 109 110 115:131:8";
    QTest::newRow("nastran") << "100, THRU, 102, 109, 110, 115, THRU, 131, BY, 8"
                             << "100:102 109 110 115:131:8";
}

void tst_Parser::_q_nastran_bulk_file_except()
{
    /* **************************************************************************************** */
    /* EXCEPT / SPECIFICATION                                                                   */
    /*                                                                                          */
    /* EXCEPT                                                                                   */
    /*      Set identification numbers following EXCEPT will be deleted from output list as     */
    /*      long as they are in the range of the set defined by the immediately preceding       */
    /*      THRU. An EXCEPT list may not include a THRU list or ALL.                            */
    /*                                                                                          */
    /* [2] Set identification numbers following EXCEPT within the range of the THRU must be in  */
    /*      ascending order.                                                                    */
    /*                                                                                          */
    /* **************************************************************************************** */

    /// \todo
    /// QTest::newRow("nastran except") << "15 THRU 100 EXCEPT 21 THRU 25" << "15:20 26:100";
    /// QTest::newRow("nastran except")
    ///         << "5, 6, 7, 8, 9, 10 THRU 55 EXCEPT 15, 16, 77, 78, 79, 100 THRU 300"
    ///         << "5:14 17:55 77:79 100:300";

}

void tst_Parser::_q_nastran_bulk_file_continuation()
{
    /// \todo
    ///  QTest::newRow("nastran 8-char fields")
    ///          << "300050  300055  300060+       \n"
    ///             "+         300065  300070  300075  300080  300085  300090  300095  300100+       \n"
    ///             "+         300105  300110  300115  300120  300125"
    ///          << "300050:300125:5";

    /// \todo
    ///  QTest::newRow("nastran continuation")
    ///          <<
    ///             "CHEXA    210     2       221     222     225     224     230     231\n"
    ///             "         234     233\n"
    ///             "CHEXA    211     2       222     223     226     225     231     232\n"
    ///             "         235     234\n"
    ///          << "2 210 211 221:226 230:235";

    /// \todo
    ///  QTest::newRow("nastran continuation")
    ///          << "            100    THRU     102     109+ELM_001\n"
    ///             "+ELM_001    110     115    THRU     131+ELM_002\n"
    ///             "+ELM_002    STEP      8"
    ///          << "100:102 109 110 115:131:8";


    /// \todo
    ///    QTest::newRow("nastran comment")
    ///           << "            100    THRU     102     109+  $ this is comment 999 555 666\n"
    ///              "+ELM_001    110     115    THRU     131+$ this is comment 333\n"
    ///              "+ELM_002    STEP      8"
    ///           << "100:102 109 110 115:131:8";

    /// \todo
    /// QTest::newRow("nastran comment") << "$100"  << "";
    /// QTest::newRow("nastran comment") << "$ 100" << "";
    /// QTest::newRow("nastran comment") << "100$" << "100";
    /// QTest::newRow("nastran comment") << "100 $" << "100";
    /// QTest::newRow("nastran comment") << "100$comment 200" << "100";
    /// QTest::newRow("nastran comment") << "100 comment 200" << "100";


}

void tst_Parser::_q_patran_gui()
{
    QTest::newRow("patran nodes")    << "Node 100:102 109 110 115:131:8" << "100:102 109 110 115:131:8";
    QTest::newRow("patran nodes")   << "n 100:102 109 110 115:131:8"     << "100:102 109 110 115:131:8";
    QTest::newRow("patran elements") << "Elm 100:102 109 110 115:131:8"  << "100:102 109 110 115:131:8";
    QTest::newRow("patran elements") << "e 100:102 109 110 115:131:8"    << "100:102 109 110 115:131:8";
    QTest::newRow("patran mpcs")    << "Mpc 100:102 109 110 115:131:8"   << "100:102 109 110 115:131:8";
    QTest::newRow("patran coords")  << "Coord 100:102 109 110 115:131:8" << "100:102 109 110 115:131:8";
    QTest::newRow("patran mix") << "n 100:102 Elm 109 110 Mpc 115:131:8" << "100:102 109 110 115:131:8";
    QTest::newRow("patran") << "Elm 100:102:-1 Elm 110 109 Elm 115:131:-8" << "100:102 109 110 115:131:8";

}

void tst_Parser::_q_patran_session_group_file()
{
    QTest::newRow("patran ses file")
            << "#\n"
               "$.........$.........$.........$.........$.........$.........$.........$.........\n"
               "ga_group_create( \"Group 10 of Nodes\" )\n"
               //                        ^^
               "ga_group_entity_add( \"Group 10 of Nodes\", \" Node 30100001:30103042 \" // @\n"
               //                            ^^                     ^^^^^^^^^^^^^^^^^
               "\"30110133:30110218 30110264:30110339 30110342 30110345:30110346 \" // @\n"
               // ^^^^^^^^^^^^^^^^^ ^^^^^^^^^^^^^^^^^ ^^^^^^^^ ^^^^^^^^^^^^^^^^^
               "\"30110768:30110772 30110776 \" // @\n"
               // ^^^^^^^^^^^^^^^^^ ^^^^^^^^
               "\"\" )\n"
               "#\n"
               "$.........$.........$.........$.........$.........$.........$.........$.........\n"
               "ga_group_create( \"500\" )\n"
               "ga_group_entity_add( \"500\", \" Node 30110001:30110081 \" // @\n"
               //                      ^^^            ^^^^^^^^^^^^^^^^^
               "\"30110768:30110772 30110776 \" // @\n"
               // ^^^^^^^^^^^^^^^^^ ^^^^^^^^
               "\"Element 30110001:30110064 \" // @\n"
               //         ^^^^^^^^^^^^^^^^^
               "\"30110097:30110160 30110193:30110519 \" )\n"
               // ^^^^^^^^^^^^^^^^^ ^^^^^^^^^^^^^^^^^
               "#\n"
               "$.........$.........$.........$.........$.........$.........$.........$.........\n"
               "ga_group_create( \"Group 300\" )\n"
               //                        ^^^
               "ga_group_entity_add( \"Group 300\", \" Node 30110001:30110081 \" // @\n"
               //                            ^^^            ^^^^^^^^^^^^^^^^^
               "\"Element 30110001:30110064 \" // @\n"
               //         ^^^^^^^^^^^^^^^^^
               "\"\" )\n"
               "#\n"
            << "10 300 500 "
               "30100001:30103042 "
               "30110001:30110081 "
               "30110097:30110519 "
               "30110768:30110772 "
               "30110776 " ;

}
void tst_Parser::_q_abaqus_inp_file()
{
    QTest::newRow("abaqus") << "*ELEMENT,TYPE=S4,ELSET=MY 2100 ELEMENTS" << "2100";

    QTest::newRow("abaqus")
            << "**\n"
               "*ELEMENT,TYPE=S4,ELSET=MY_ELEMENTS_PROP\n"
               "       100,       101,       102,       109,       110\n"
               "       110,       115,       123,       131,      2000**NAME MY_ELEMENTS_PROP\n"
               "*\n"
            << "100:102 109 110 115:131:8";

    QTest::newRow("abaqus")
            << "** \n"
               "*ELSET, ELSET=prop_PSHELL\n"
               "  30950194,  30950195,  30950196,  30950197,  30950198,  30950199,  30950200,  30950201,\n"
               "  30950202,  30950203,  30950204,  30950205,  30950206,  30950207,  30950208,  30950209,\n"
               "  30950210,  30950211,  30950212,  30950213,  30950214,  30950215,  30950216,  30950217,\n"
               "  30950218,  30950219,  30950220,  30950221,  30950222,  30950223,  30950224,  30950542,\n"
               "  30950543,  30950544,  30950545,  30950546,  30950547,  30950548,  30950549,  30950550,\n"
               "  30950551,  30950552,  30950553,  30950554,  30950555,  30950556,  30950557,  30950558,\n"
               "  30950559,  30950560,  30950561,  30950562,  30950563,  30950564,  30950565,  30950566,\n"
               "  30950567,  30950568,  30950569,  30950570,  30950571,  30950572,  30950573,  30950574,\n"
               "  30950575,  30950576,  30950577,  30950578,  30950579,  30950580,  30950581,  30950582,\n"
               "  30950583,  30950584,  30950585,  30950586,  30950587,  30950588,  30950589,  30950590,\n"
               "  30950591,  30950592,  30950593,  30950594,  30950595,  30999999**BEAMSECTS\n"
               "*\n"
               "**\n"
            << "30950194:30950224 30950542:30950595";

}

void tst_Parser::_q_femap_gui()
{
    QTest::newRow("femap") << "100,101,102,109,110,115,123,131" << "100:102 109 110 115:131:8";
    QTest::newRow("femap") << "100:102,109,110,115:131:8"       << "100:102 109 110 115:131:8";
    QTest::newRow("femap") << "102:100:-1,109,110,131:115:-8"   << "100:102 109 110 115:131:8";

}

void tst_Parser::_q_hypermesh_gui()
{
    QTest::newRow("hypermesh") << "el 1,2,3" << "1:3";
    QTest::newRow("hypermesh") << "el 1-45"  << "1:45";

    QTest::newRow("hypermesh") << "el 100,101,102,109,110,115,123,131"
                               << "100:102 109 110 115:131:8";

    QTest::newRow("hypermesh") << "el 31042011,31042013,31042035,31042037"
                               << "31042011 31042013 31042035 31042037";

}

void tst_Parser::_q_calc_excel_gui()
{
    QTest::newRow("csv rows") << "100,101,102,109,110,115,123,131"
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv rows") << "100;101;102;109;110;115;123;131;;;"
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv rows") << "\"\";\"100\";\"101\";\"102\";\"109\";\"110\";"
                                 "\"115\";\"123\";\"131\";\"\";\"\""
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv rows") << "100\t101\t\t\t102\t109\t110\t115\t123\t131\t\t"
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv cols") << "100\n101\n102\n109\n\n110\n\n\n115\n123\n131\n"
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv cols") << "100\r101\r102\r109\r\r110\r\r\r115\r123\r131\r"
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv cols") << "100\r\n101\r\n102\r\n109\r\n\r\n110"
                                 "\r\n\r\n\r\n115\r\n123\r\n131\r\n"
                              << "100:102 109 110 115:131:8";
    QTest::newRow("csv cols") << "\"\"\n\"100\"\n\"101\"\n\"102\"\n\"109\"\n\"\"\n\"110\"\n"
                                 "\"\"\n\"\"\n\"115\"\n\"123\"\n\"131\"\n"
                              << "100:102 109 110 115:131:8";


    QTest::newRow("csv several rows") << "100,101,102,109\n"
                                         "110,115,123,131"
                                      << "100:102 109 110 115:131:8";
    QTest::newRow("csv several rows") << "100;101;102;109\n"
                                         "110;115;123;131;;;"
                                      << "100:102 109 110 115:131:8";
    QTest::newRow("csv several rows") << "\"\";\"100\";\"101\";\"102\";\"109\"\n"
                                         ";\"110\";\"115\";\"123\";\"131\";\"\";\"\""
                                      << "100:102 109 110 115:131:8";

}

QTEST_APPLESS_MAIN(tst_Parser)

#include "tst_parser.moc"
