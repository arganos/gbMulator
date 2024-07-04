#-------------------------------------------------
#
# Project created by QtCreator 2015-05-28T14:03:41
#
#-------------------------------------------------

QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtgb
TEMPLATE = app


SOURCES += main.cpp\
    glwidget.cpp \
    helper.cpp \
        mainwindow.cpp \
    gb_core/gb_mem.cpp \
    gb_core/gb_gpu.cpp \
    gb_core/gb_cpu.cpp \
    gb_core/gb_cartridge.cpp \
    gb_core/gb_lcd.cpp \
    gb_core/gb_timer.cpp \
    gb_core/gb_interrupt.cpp \
    gb_core/gb_joypad.cpp \
    gb_core/gb_util.cpp \
    gb_worker.cpp \
    mycanvas.cpp \
    qsfmlcanvas.cpp



HEADERS  += mainwindow.h \
    gb_core/gb_mem.h \
    gb_core/gb_cartridge.h \
    gb_core/gb_cpu.h \
    gb_core/gb_gpu.h \
    gb_core/gb_lcd.h \
    gb_core/gb_timer.h \
    gb_core/gb_interrupt.h \
    gb_core/gb_joypad.h \
    gb_core/gb_util.h \
    gb_worker.h \
    glwidget.h \
    helper.h \
    mycanvas.h \
    qsfmlcanvas.h

FORMS    += mainwindow.ui

LIBS += -L"/usr/local/lib"

### TO ALLOW SFML LIBRARY ###
CONFIG_APP_NAME = "GB Emulator"
LIBS += -L"/usr/local/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window
CONFIG(debug, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window

INCLUDEPATH += "/usr/local/include"
INCLUDEPATH += "./SFML"
DEPENDPATH += "/usr/local/include"

DISTFILES += bios.gb \
    2friendly.gb \
    game.gb
