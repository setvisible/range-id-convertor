#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_rangehelper
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_rangehelper.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../../src/core/range.h
SOURCES += ../../src/core/range.cpp
HEADERS += ../../src/core/rangehelper.h
SOURCES += ../../src/core/rangehelper.cpp
