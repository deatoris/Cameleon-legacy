QT       += core gui opengl

TEMPLATE = lib
CONFIG      += plugin
TARGET = cvm
QT += core \
      gui

INCLUDEPATH +=   core \
    gui \
    lib
DESTDIR += $$quote($${BUILDPATH})

CONFIG+=CAMELEON

#IDE INCLUDES
!include($${SOURCEPATH}/machine/ide/includeide.pri)

#MACHINE KERNEL INCLUDES (NEEDED BY BLANKLIB)
!include($${SOURCEPATH}/machine/kernel/includekernel.pri)
!include($${SOURCEPATH}/machine/client/includeclient.pri)
!include($${SOURCEPATH}/machine/scd/includescd.pri)

#CAMELEON SOURCES
!include($${SOURCEPATH}/machine/client/client.pri)
!include($${SOURCEPATH}/machine/kernel/kernel.pri)
!include($${SOURCEPATH}/machine/ide/ide.pri)

#LIB SOURCES
!include($${SOURCEPATH}/machine/scd/scd.pri)


