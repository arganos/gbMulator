#-------------------------------------------------
#
# Project created by QtCreator 2015-05-28T14:03:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtgb
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qsfmlcanvas.cpp \
    mycanvas.cpp \
    gb_mem.cpp \
    gb_gpu.cpp \
    gb_cpu.cpp \
    gb_cartridge.cpp \
    gb_input.cpp \
    gb_lcd.cpp \
    gb_timer.cpp \
    gb_interrupt.cpp \
    gb_joypad.cpp \
    gb_util.cpp


HEADERS  += mainwindow.h \
    qsfmlcanvas.h \
    mycanvas.h \
    gb_mem.h \
    gb_cartridge.h \
    gb_cpu.h \
    gb_gpu.h \
    gb_lcd.h \
    gb_timer.h \
    gb_interrupt.h \
    gb_input.h \
    gb_joypad.h \
    gb_util.h

FORMS    += mainwindow.ui

LIBS += -L"/usr/local/lib"

### TO ALLOW SFML LIBRARY ###
CONFIG_APP_NAME = "GB Emulator"
LIBS += -L"/usr/local/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window
CONFIG(debug, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window

INCLUDEPATH += "/usr/local/include"
DEPENDPATH += "/usr/local/include"
