CONFIG += console

QT       += core gui

TARGET = lcm

TEMPLATE = app

SOURCES += cli/CGLcm.cpp

!include(kernel/includekernel.pri)
!include(scd/includescd.pri)
!include(ide/includeide.pri)
!include(client/includeclient.pri)

#CAMELEON SOURCES
!include(kernel/kernel.pri)
!include(ide/ide.pri)
!include(client/client.pri)

#LIB SOURCES
!include(scd/scd.pri)

win32:DESTDIR += $$quote($${BUILDPATH})
unix:DESTDIR += $$quote($${BUILDPATH})

QMAKE_CXXFLAGS += -std=c++98
