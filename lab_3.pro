#-------------------------------------------------
#
# Project created by QtCreator 2018-12-23T22:00:10
#
#-------------------------------------------------

QT   += core gui
QT   += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts

TARGET = LastCase
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += DEBUG
DEFINES += TEST

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
     chart.cpp \
     chart_drawing_impls.cpp \
     data_reader_impls.cpp \
     ioc_container.cpp \
     main.cpp \
     mainwindow.cpp
HEADERS += \
     chart.h \
     chart_drawing_if.h \
     chart_drawing_impls.h \
     common_container.h \
     data_reader_if.h \
     data_reader_impls.h \
     ioc_container.h \
     mainwindow.h \
     message_box.h \
