#-------------------------------------------------
#
# Project created by QtCreator 2011-06-14T18:24:41
#
#-------------------------------------------------

QT       += core gui

TARGET = LED_Table_Simulator
TEMPLATE = app


SOURCES +=\
    main.cpp \
    gui/ledtablewidget.cpp \
    gui/mainwindow.cpp \
    ../../animations/*.c \
    /home/kevin/workspace/avr/LEDTabelControlPanelFW01/led_graphics/*.c

HEADERS  +=\
    gui/ledtablewidget.h \
    gui/mainwindow.h \
    ../../animations/headers/*.h \
    /home/kevin/workspace/avr/LEDTabelControlPanelFW01/led_graphics/*.h

FORMS    += gui/mainwindow.ui

INCLUDEPATH +=\
    ../../animations/headers/ \
    ../../animations/ \
    /home/kevin/workspace/avr/LEDTabelControlPanelFW01/led_graphics/

QMAKE_CFLAGS += -std=c99
