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

#ifndef PARSER_H
#define PARSER_H

#include "rangelist.h"

class Parser
{    
    /* Q_DISABLE_COPY(Parser) */
    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;

private:
    explicit Parser();
    static Parser* m_instance;

public:
    static Parser* instance();
    static void deleteInstance();
    ~Parser();

    RangeListPtr parse(const QString &text) const;

private:
    enum TokenType {
        TOKEN_UNKNOWN,
        TOKEN_NUMBER,
        TOKEN_THRU,
        TOKEN_STEP,
        TOKEN_EXCEPT,
        TOKEN_STREAM_END
    };

    struct Token {
        explicit Token() : type(TOKEN_UNKNOWN), value(-1) {}
        explicit Token(TokenType _type, int _value = -1) : type(_type), value(_value) {}
        TokenType type;
        int value;
    };

    QList<Token> tokenize(const QString &text) const;

};



#endif // PARSER_H
