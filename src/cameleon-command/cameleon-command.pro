#-------------------------------------------------
#
# Project created by Olivier Cugnon de Svricourt & Vincent Tariel
# This project and all the sources are the property of his creators & the shinoe software company.
#
#-------------------------------------------------
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

win32:DESTDIR = ../../deliverable/instance/project/devkit/exporter/ressource

LIBS += -L"$${BUILDPATH}"
LIBS += -lcvm

TARGET = command

TEMPLATE = app

SOURCES += main.cpp
unix:DESTDIR += $$quote($${BUILDPATH})/../project/devkit/exporter/ressource
#unix:DESTDIR = ../../../deliverable/instance
#unix:DESTDIR = ../../../
