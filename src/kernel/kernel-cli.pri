#======================================
#Caméléon Creator & Population 
#
#Copyright 2012 O. Cugnon de Sévricourt & V. Tariel
#
#This software is under the MIT license :
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#More informations can be found here: http://www.shinoe.org/cameleon
#======================================
MACHINEPATH = kernel

HEADERS += \
    $${MACHINEPATH}/CCore/CData/CData.h \
    $${MACHINEPATH}/CCore/CData/CDataByFile.h \
    $${MACHINEPATH}/CCore/CData/CDataByValue.h \
    $${MACHINEPATH}/CCore/CPlug.h \
    $${MACHINEPATH}/CCore/COperator.h \
#    $${MACHINEPATH}/CCore/COperatorThread.h \
    $${MACHINEPATH}/CTool/CFactoryData.h \
 #   $${MACHINEPATH}/CCore/CConnectorDirect.h \
    $${MACHINEPATH}/CTool/CSingleton.h \
    $${MACHINEPATH}/CTool/CFactoryOperator.h \
    $${MACHINEPATH}/CTool/CDisplayComposition.h \
    $${MACHINEPATH}/CCore/CProcessor.h \
    $${MACHINEPATH}/CTool/CError.h \
    $${MACHINEPATH}/CGlossary/CDictionnary-cli.h \
    $${MACHINEPATH}/CGlossary/CGlossary-cli.h \
    $${MACHINEPATH}/CTool/CFactory.h \
    $${MACHINEPATH}/CTool/CTree.h \
    $${MACHINEPATH}/CMachine/CMachine.h \
    $${MACHINEPATH}/CTool/CFactoryControl.h \
    $${MACHINEPATH}/CTool/CLogger.h \
    $${MACHINEPATH}/CCore/CProcessorObject.h \
    $${MACHINEPATH}/CCore/CProcessorThread.h \
    $${MACHINEPATH}/CTool/CPerf.h \
    $${MACHINEPATH}/CMachine/CMode.h \
    $${MACHINEPATH}/CGlossary/ICDictionary-cli.h\
    $${MACHINEPATH}/CTool/CException.h

SOURCES +=  \
    $${MACHINEPATH}/CCore/CData/CData.cpp \
    $${MACHINEPATH}/CCore/CPlug.cpp \
    $${MACHINEPATH}/CCore/COperator.cpp \
 #   $${MACHINEPATH}/CCore/COperatorThread.cpp \
 #   $${MACHINEPATH}/CCore/CConnectorDirect.cpp \
    $${MACHINEPATH}/CTool/CDisplayComposition.cpp \
    $${MACHINEPATH}/CCore/CProcessor.cpp \
    $${MACHINEPATH}/CTool/CError.cpp \
    $${MACHINEPATH}/CGlossary/CGlossary-cli.cpp \
    $${MACHINEPATH}/CGlossary/CDictionnary-cli.cpp \
    $${MACHINEPATH}/CMachine/CMachine.cpp \
    $${MACHINEPATH}/CTool/CLogger.cpp \
    $${MACHINEPATH}/CCore/CProcessorObject.cpp \
    $${MACHINEPATH}/CCore/CProcessorThread.cpp \
    $${MACHINEPATH}/CTool/CPerf.cpp \
    $${MACHINEPATH}/CTool/CException.cpp



