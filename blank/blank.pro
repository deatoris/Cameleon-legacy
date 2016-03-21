#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T11:01:48
#
#-------------------------------------------------
!include(../src/configuration.pri)
BLANKPATH = $${SOURCEPATH}/../blank

QT       += core gui


TARGET        = $$qtLibraryTarget(blank)
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $${BUILDPATHLIBS}
LIBS += -L"$${BUILDPATH}"
LIBS += -lcvm
#MACHINE KERNEL INCLUDES (NEEDED BY BLANKLIB)
!include(../src/machine/kernel/includekernel.pri)
!include(../src/machine/scd/includescd.pri)

#POPULATIONPATH = $${SOURCEPATH}/dictionary/population
#!include(../population/PopulationBindingCameleon/PopulationBindingCameleonInclude.pri)
#!include(../population/PopulationCommon/PopulationCommonIncludes.pri)
#!include(../population/PopulationCommon/GPDynamic/GPDynamic.pri)

INCLUDEPATH += \
    $${BLANKPATH}/Control \
    $${BLANKPATH}/Operator  \
    $${BLANKPATH}/Data

SOURCES += \
    $${BLANKPATH}/blankplugin.cpp \
    $${BLANKPATH}/blankDictionnary.cpp \
    $${BLANKPATH}/Operator/BlankOperatorExample.cpp \
    $${BLANKPATH}/Data/BlankDataExample.cpp \
    $${BLANKPATH}/Control/BlankViewExample.cpp \
    $${BLANKPATH}/Control/BlankEditorExample.cpp

HEADERS += \
    $${BLANKPATH}/blankplugin.h \
    $${BLANKPATH}/blankDictionnary.h \
    $${BLANKPATH}/Operator/BlankOperatorExample.h \
    $${BLANKPATH}/Data/BlankDataExample.h \
    $${BLANKPATH}/Control/BlankViewExample.h \
    $${BLANKPATH}/Control/BlankEditorExample.h
