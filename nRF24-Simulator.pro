#-------------------------------------------------
#
# Project created by QtCreator 2014-12-18T14:25:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = nRF24-Simulator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    nRF24interface.cpp \
    nRF24l01plus.cpp \
    nRF24registers.cpp \
    RF24.cpp \
    ether.cpp

HEADERS += \
    nRF24bits_struct.h \
    nRF24interface.h \
    nRF24L01.h \
    nRF24l01plus.h \
    nRF24registers.h \
    RF24.h \
    RF24_config.h \
    ether.h \
    msgframe.h
