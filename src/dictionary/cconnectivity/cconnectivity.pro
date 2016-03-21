#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T11:01:48
#
#-------------------------------------------------
!include(../../configuration.pri)
CONNECTIVITYPATH = $${SOURCEPATH}/dictionary/cconnectivity
QT       += core gui webkit opengl
QT       += core gui qt
QT +=   webkit \
        network \
        sql
INCLUDEPATH +=   core \
    gui \
    lib
TARGET        = $$qtLibraryTarget(cconnectivity)
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $${BUILDPATHLIBS}
LIBS += -L"$${BUILDPATH}"
LIBS += -lcvm
#MACHINE KERNEL INCLUDES (NEEDED BY BLANKLIB)
!include(../../machine/kernel/includekernel.pri)

!include(../../machine/ide/includeide.pri)
!include(../../machine/kernel/includekernel.pri)
!include(../../machine/client/includeclient.pri)
!include(../../machine/scd/includescd.pri)
!include(../../machine/scd/includescd.pri)

INCLUDEPATH += \
    $${CONNECTIVITYPATH}/Control \
    $${CONNECTIVITYPATH}/Operator  \
    $${CONNECTIVITYPATH}/Data

SOURCES += \
    $${CONNECTIVITYPATH}/cconnectivityplugin.cpp \
    $${CONNECTIVITYPATH}/cconnectivityDictionnary.cpp

HEADERS += \
    $${CONNECTIVITYPATH}/cconnectivityplugin.h \
    $${CONNECTIVITYPATH}/cconnectivityDictionnary.h

SOURCES += \
    $${CONNECTIVITYPATH}/Operator/Urls.cpp \
    $${CONNECTIVITYPATH}/Operator/ImagesUrls.cpp \
    $${CONNECTIVITYPATH}/Operator/DownloadFile.cpp \
    $${CONNECTIVITYPATH}/Operator/Source.cpp \
    $${CONNECTIVITYPATH}/Operator/VideoUrls.cpp \
    $${CONNECTIVITYPATH}/Operator/Createdb.cpp \
    $${CONNECTIVITYPATH}/Control/Browser.cpp \
    $${CONNECTIVITYPATH}/Data/DataDB.cpp \
    $${CONNECTIVITYPATH}/Operator/Query.cpp \
    $${CONNECTIVITYPATH}/Operator/DBTables.cpp \
    $${CONNECTIVITYPATH}/Operator/ParametrizeQuery.cpp \
    $${CONNECTIVITYPATH}/Operator/Twitter.cpp

HEADERS += \
    $${CONNECTIVITYPATH}/Operator/Urls.h \
    $${CONNECTIVITYPATH}/Operator/ImagesUrls.h \
    $${CONNECTIVITYPATH}/Operator/DownloadFile.h \
    $${CONNECTIVITYPATH}/Operator/Source.h \
    $${CONNECTIVITYPATH}/Operator/VideoUrls.h \
    $${CONNECTIVITYPATH}/Operator/Createdb.h \
    $${CONNECTIVITYPATH}/Control/Browser.h \
    $${CONNECTIVITYPATH}/Data/DataDB.h \
    $${CONNECTIVITYPATH}/Operator/Query.h \
    $${CONNECTIVITYPATH}/Operator/DBTables.h \
    $${CONNECTIVITYPATH}/Operator/ParametrizeQuery.h \
    $${CONNECTIVITYPATH}/Operator/Twitter.h
