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

#ifndef GLOBALS_H
#define GLOBALS_H

#include "builddefs.h"
#include <QtCore/QString>

const QString STR_APPLICATION_NAME("RangeIDConvertor");
const QLatin1String STR_APPLICATION_VERSION("1.1.0");
const QLatin1String STR_APPLICATION_DATE("2017");
const QString STR_APPLICATION_AUTHOR("Sébastien Vavassori");

/* Something like "2017-05-20_15:36:58" */
const QString STR_APPLICATION_BUILD =
        QString("%1-%2-%3_%4:%5:%6")
        .arg(BUILD_YEAR, 4, 10, QChar('0'))
        .arg(BUILD_MONTH, 2, 10, QChar('0'))
        .arg(BUILD_DAY, 2, 10, QChar('0'))
        .arg(BUILD_HOUR, 2, 10, QChar('0'))
        .arg(BUILD_MIN, 2, 10, QChar('0'))
        .arg(BUILD_SEC, 2, 10, QChar('0'));


#endif // GLOBALS_H
