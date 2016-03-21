CONFIG += console

#LOCAL CONFIGURATION
!include(../configuration.pri)

#IDE INCLUDES
!include(../machine/ide/includeide.pri)

#MACHINE KERNEL INCLUDES (NEEDED BY BLANKLIB)
!include(../machine/kernel/includekernel.pri)
!include(../machine/client/includeclient.pri)
!include(../machine/scd/includescd.pri)

#STATICS DICTIONARIES INCLUDES

QT       += core gui

LIBS += -L"$${BUILDPATH}"
LIBS += -lcvm

TARGET = cameleon-cli

TEMPLATE = app

SOURCES += ../machine/ide/CGIde.cpp

win32:DESTDIR += $$quote($${BUILDPATH})
unix:DESTDIR += $$quote($${BUILDPATH})
