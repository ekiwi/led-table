#-------------------------------------------------
#
# Project created by QtCreator 2011-06-14T18:24:41
#
#-------------------------------------------------

QT       += core gui

TARGET = simulator
TEMPLATE = app


SOURCES +=\
    main.cpp \
    gui/ledtablewidget.cpp \
    gui/mainwindow.cpp \
    ../animations/*.c \
    ../control_panel/led_graphics/*.c

HEADERS  +=\
    gui/ledtablewidget.h \
    gui/mainwindow.h \
    ../animations/headers/*.h \
    ../control_panel/led_graphics/*.h

FORMS    += gui/mainwindow.ui

INCLUDEPATH +=\
    ../animations/headers/ \
    ../animations/ \
    ../control_panel/led_graphics/

QMAKE_CFLAGS += -std=gnu99
