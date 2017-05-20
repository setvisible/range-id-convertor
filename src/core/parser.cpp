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

#include "parser.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDebug>

/*!
 * \class Parser
 * \brief The Parser class parses an input text and returns a list of ranges.
 *
 * Parser is a singleton, you can call it from every where in the code.
 * To do it, you can get the instance with the instance() method.
 *
 * \code
 *   // call it anywhen during the life of the QApplication
 *   QString text(...);
 *   Parser *p = Parser::instance();
 *   RangeListPtr result = p->parse( text );
 * \endcode
 */


Parser* Parser::m_instance = Q_NULLPTR;

Parser::Parser()
{
}

Parser::~Parser()
{
    m_instance = Q_NULLPTR; // needed for unit test
}

/*!
 * \brief Gets the unique instance.
 */
Parser* Parser::instance()
{
    if (!m_instance)
        m_instance = new Parser();
    return m_instance;
}

/*!
 * \brief [FOR UNIT TEST ONLY] Delete the singleton.
 * \todo maybe just clean the libraries ?
 */
void Parser::deleteInstance()
{
    if (m_instance) {
        delete m_instance;
        m_instance = Q_NULLPTR;
    }
}

/*!
 * \brief Parses a input text and returns a list of ranges.
 * Rem: the returned ranges might contains duplicates, and entries might be unsorted.
 */
RangeListPtr Parser::parse(const QString &text) const
{
    RangeListPtr ret(new RangeList);

    Token current;
    Token next;
    QList<Token> tokens = tokenize(text);

    const int count = tokens.count();
    for (int i = 0; i < count; ++i) {

        current = tokens.at(i);
        if (i < count-1) { next = tokens.at(i+1); } else { next.type = TOKEN_STREAM_END;}

        if ( next.type == TOKEN_NUMBER ||
             next.type == TOKEN_UNKNOWN ||
             next.type == TOKEN_STREAM_END ) {
            /* Value as Number */

            if (current.type == TOKEN_NUMBER && current.value > 0) {
                Range r(current.value);
                ret->add(r);
                continue;
            }

        } else if (next.type == TOKEN_THRU) {
            /* Value as Range */

            int _from = current.value;
            int _to = 0;
            int _by = 0;

            i++;
            i++;
            if (i < count) {
                current = tokens.at(i);
                if (i < count-1) { next = tokens.at(i+1); } else { next.type = TOKEN_UNKNOWN;}

                _to = current.value;

                if (next.type == TOKEN_STEP) {
                    i++;
                    i++;
                    if (i < count) {
                        current = tokens.at(i);
                        _by = current.value;
                    }
                }
            }

            if ((_by >= 0 && _to >= _from) || (_by < 0 && _from >= _to)) {
                Range r(_from, _to, _by);
                ret->add(r);
            } else {
                // Error message
                // qDebug() << "Cannot parse '" << _from << "to" << _to << "'.";
                // qDebug() <<"The second value MUST be greater than the first value.";
            }
            continue;

        } else {
            // ERROR ?
            Q_ASSERT(false);
        }

    }
    return ret;
}

static inline QString depatranize(const QString &text)
{
    /*
     * Remove the lines markers generally found
     * in the Patran Session files.
     * Example:          [ "Node 681" // @\n"350:681400" ]
     *           becomes [ "Node 681350:681400" ]
     */

    QString ret = text;
    ret.replace(QLatin1String("\" // @\n\""), QLatin1String(""), Qt::CaseInsensitive);
    return ret;
}

QList<Parser::Token> Parser::tokenize(const QString &text) const
{
    QList<Token> ret;

    if (text.isEmpty())
        return ret;

    const QString depatranized = depatranize(text);

    QRegularExpression rx_separation_chars("(\\ |\\,|\\.|\\;|\\'|\\\"|\\t|\\r|\\n)");
    QRegularExpression rx_colon_range("^(?<from>\\d+)(:(?<to>\\d+)(:(?<by>[+-]?\\d+))?)?$");
    QRegularExpression rx_dash_range("^(?<from>\\d+)-(?<to>\\d+)$");
    QRegularExpression rx_negative_number("^-\\d+$");

    QStringList segments = depatranized.split(rx_separation_chars, QString::SkipEmptyParts);

    for(auto segment : segments) {

        if (0 == QString::compare(segment, "THRU", Qt::CaseInsensitive)) {
            ret << Token(TOKEN_THRU);
            continue;
        }
        if (0 == QString::compare(segment, "BY", Qt::CaseInsensitive)) {
            ret << Token(TOKEN_STEP);
            continue;
        }
        if (0 == QString::compare(segment, "STEP", Qt::CaseInsensitive)) {
            ret << Token(TOKEN_STEP);
            continue;
        }
        if (0 == QString::compare(segment, "EXCEPT", Qt::CaseInsensitive)) {
            ret << Token(TOKEN_EXCEPT);
            continue;
        }

        QRegularExpressionMatch match_colon = rx_colon_range.match(segment);
        if (match_colon.hasMatch()) {
            bool ok;
            int from = match_colon.captured("from").toInt(&ok);
            Q_ASSERT(ok);
            int to = match_colon.captured("to").toInt();
            int by = match_colon.captured("by").toInt();
            ret << Token(TOKEN_NUMBER, from);
            if (to) {
                ret << Token(TOKEN_THRU);
                ret << Token(TOKEN_NUMBER, to);
            }
            if (by) {
                ret << Token(TOKEN_STEP);
                ret << Token(TOKEN_NUMBER, by);
            }
            continue;
        }

        QRegularExpressionMatch match_dash = rx_dash_range.match(segment);
        if (match_dash.hasMatch()) {
            bool ok;
            int from = match_dash.captured("from").toInt(&ok);
            Q_ASSERT(ok);
            int to = match_dash.captured("to").toInt(&ok);
            Q_ASSERT(ok);
            ret << Token(TOKEN_NUMBER, from);
            ret << Token(TOKEN_THRU);
            ret << Token(TOKEN_NUMBER, to);
            continue;
        }

        QRegularExpressionMatch match_number = rx_negative_number.match(segment);
        if (match_number.hasMatch()) {
            bool ok;
            int value = segment.toInt(&ok);
            Q_ASSERT(ok);
            ret << Token(TOKEN_NUMBER, value);
            continue;
        }

        ret << Token(TOKEN_UNKNOWN);

    }
    return ret;
}
