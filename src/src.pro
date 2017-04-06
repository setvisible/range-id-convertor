#-------------------------------------------------
# RIC - Range ID Convertor
#-------------------------------------------------
TEMPLATE = app
TARGET   = RangeIDConvertor
QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += no_keyword

lessThan(QT_VERSION, 5.0) {
    warning("prefere to build it with Qt 5.0")
}



#-------------------------------------------------
# INCLUDE
#-------------------------------------------------
INCLUDEPATH += $$PWD/../include/


#-------------------------------------------------
# SOURCES
#-------------------------------------------------
include($$PWD/core/core.pri)
include($$PWD/gui/gui.pri)

HEADERS  += \
    $$PWD/about.h \
    $$PWD/globals.h \
    $$PWD/mainwindow.h

SOURCES += \
    $$PWD/mainwindow.cpp \
    $$PWD/main.cpp

FORMS += \
    $$PWD/mainwindow.ui


#-------------------------------------------------
# RESOURCES
#-------------------------------------------------
RESOURCES = $$PWD/resources.qrc

win32|unix {
    RC_FILE = $$PWD/resources_win.rc
}

#macx {
#    ICON = $$PWD/src/icons/logo.icns
#}


#-------------------------------------------------
# BUILD OPTIONS
#-------------------------------------------------

# Rem: On Ubuntu, directories starting with '.' are hidden by default
win32 {
    MOC_DIR =      ./.moc
    OBJECTS_DIR =  ./.obj
    UI_DIR =       ./.ui
} else {
    MOC_DIR =      ./_moc
    OBJECTS_DIR =  ./_obj
    UI_DIR =       ./_ui
}

#-------------------------------------------------
# OUTPUT
#-------------------------------------------------


