#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_range
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_range.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../../src/core/range.h
SOURCES += ../../src/core/range.cpp
