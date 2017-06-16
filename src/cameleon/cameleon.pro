#======================================
#Caméléon Creator & Population 
#
#Copyright 2012 O. Cugnon de Sévricourt & V. Tariel
#
#This software is under the MIT license :
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#======================================
#STATICS DICTIONARIES INCLUDES

QT       += webkit opengl
QT +=   webkit \
        network \
        sql
QT       += core gui qt

#LIBS += -L"$${BUILDPATH}"
#LIBS += -lcvm

TARGET = cameleon

TEMPLATE = app

SOURCES += ../machine/ide/CGIde.cpp

unix:DESTDIR += $$quote($${BUILDPATH})
win32:DESTDIR += $$quote($${BUILDPATH})

#IDE INCLUDES
!include(../machine/ide/includeide.pri)

#MACHINE KERNEL INCLUDES (NEEDED BY BLANKLIB)
!include(../machine/scd/includescd.pri)
!include(../machine/cconnectivity/includecconnectivity.pri)
!include(../machine/kernel/includekernel.pri)
!include(../machine/client/includeclient.pri)

#CAMELEON SOURCES
!include(../machine/client/client.pri)
!include(../machine/kernel/kernel.pri)
!include(../machine/ide/ide.pri)

#LIB SOURCES
!include(../machine/scd/scd.pri)
!include(../machine/cconnectivity/cconnectivity.pri)
