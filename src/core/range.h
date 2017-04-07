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

#ifndef RANGE_H
#define RANGE_H

#include <QtCore/QString>

typedef int Identifier;

class Range
{
public:
    explicit Range(const Identifier _from = 0,
                   const Identifier _to = 0, const int _by = 0);

    int count() const;
    bool isEmpty() const;
    void clear();

    /* Setters */
    void setRange(const Identifier _from,
                  const Identifier _to = 0, const int _by = 0);

    /* Getters */
    inline Identifier from() const { return m_from; }
    inline Identifier to() const { return m_to; }
    inline int by() const { return m_by; }

    /* Boolean Operations */
    bool operator==(const Range &other) const;
    bool operator!=(const Range &other) const;

private:
    Identifier m_from;
    Identifier m_to;
    int m_by;
    void simplify();

};

#endif // RANGE_H
