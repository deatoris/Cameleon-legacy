#======================================
#Caméléon
#
#Copyright 2012 O. Cugnon de Sévricourt & V. Tariel
#
#This software is under the MIT license :
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#======================================
!include(configuration.pri)

QT       += webkit opengl
QT +=   webkit \
        network \
        sql
QT       += core gui qt

TARGET = ecm

TEMPLATE = app

unix:DESTDIR += $$quote($${BUILDPATH})
win32:DESTDIR += $$quote($${BUILDPATH})

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

SOURCES += ide/CGIde.cpp

# build must be last:
CONFIG += ordered

unix:QMAKE_CXXFLAGS += -std=c++98

