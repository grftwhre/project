#-------------------------------------------------
#
# Project created by QtCreator 2019-05-15T16:51:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chart_Draw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lexer.cpp \
    parser.cpp \
    polynome.cpp \
    polynomewidget.cpp

HEADERS  += mainwindow.h \
    lexer.h \
    parser.h \
    polynome.h \
    polynomewidget.h

FORMS    += mainwindow.ui
