CONFIG += console
QT+= core
TARGET = lcm
TEMPLATE = app

SOURCES += cli/CGLcm.cpp

!include(kernel/includekernel-cli.pri)
!include(scd/includescd-cli.pri)
!include(cli/includecli.pri)
!include(client-cli/includeclient-cli.pri)

#CAMELEON SOURCES
!include(kernel/kernel-cli.pri)
!include(cli/cli.pri)
!include(client-cli/client-cli.pri)

#LIB SOURCES
!include(scd/scd-cli.pri)

win32:DESTDIR += $$quote($${BUILDPATH})
unix:DESTDIR += $$quote($${BUILDPATH})

unix:QMAKE_CXXFLAGS += -std=c++98
