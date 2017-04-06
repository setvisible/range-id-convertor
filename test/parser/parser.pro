#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_parser
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_parser.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS += ../shared/utils.h
SOURCES += ../shared/utils.cpp

HEADERS += ../../src/core/parser.h
SOURCES += ../../src/core/parser.cpp
HEADERS += ../../src/core/range.h
SOURCES += ../../src/core/range.cpp
HEADERS += ../../src/core/rangelist.h
SOURCES += ../../src/core/rangelist.cpp
