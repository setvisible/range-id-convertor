#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_rangelist
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_rangelist.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../shared/utils.h
SOURCES += ../shared/utils.cpp

HEADERS += ../../src/core/range.h
SOURCES += ../../src/core/range.cpp
HEADERS += ../../src/core/rangelist.h
SOURCES += ../../src/core/rangelist.cpp
