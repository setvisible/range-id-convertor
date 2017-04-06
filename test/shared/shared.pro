#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_utils
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_utils.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += utils.h
SOURCES += utils.cpp

HEADERS += ../../src/core/parser.h
SOURCES += ../../src/core/parser.cpp
HEADERS += ../../src/core/range.h
SOURCES += ../../src/core/range.cpp
HEADERS += ../../src/core/rangelist.h
SOURCES += ../../src/core/rangelist.cpp
