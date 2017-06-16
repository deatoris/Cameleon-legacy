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
IDEPATH = ../machine/ide
SOURCES += \
		$${IDEPATH}/CGComposer/CGComposer.cpp \
		$${IDEPATH}/CGCommandManager.cpp \
		$${IDEPATH}/CGProject/CGProject.cpp \
		$${IDEPATH}/CGToolTip.cpp \
		$${IDEPATH}/CGStyle.cpp \
		$${IDEPATH}/CGCommand/CGCreateOperator.cpp \
		$${IDEPATH}/CGCommand/CGDisconnectConnector.cpp \
		$${IDEPATH}/CGCommand/CGDeleteOperator.cpp \
		$${IDEPATH}/CGCommand/CGChangeView.cpp \
		$${IDEPATH}/CGCommand/CGCopyPaste.cpp \
		$${IDEPATH}/CGCommand/CGClear.cpp \
		$${IDEPATH}/CGCommand/CGConnectConnector.cpp \
		$${IDEPATH}/CGComposer/CGCompositionManager.cpp \
		$${IDEPATH}/CGComposer/CGComposition.cpp \
		$${IDEPATH}/CGComposer/CGCompositionView.cpp \
		$${IDEPATH}/CGComposer/CGCompositionFactory.cpp \
		$${IDEPATH}/CGComposer/CGCompositionItem.cpp \
		$${IDEPATH}/CGComposer/CGCompositionModel.cpp \
		$${IDEPATH}/CGController/CGLayoutManager.cpp \
		$${IDEPATH}/CGController/CGController.cpp \
		$${IDEPATH}/CGInstance.cpp \
		$${IDEPATH}/CGController/CGLayoutItem.cpp \
		$${IDEPATH}/CGInstanceProperties.cpp \
		$${IDEPATH}/CGProject/CGProjectProperties.cpp \
		$${IDEPATH}/CGController/CGLayout.cpp \
		$${IDEPATH}/CGController/CGLayoutView.cpp \
		$${IDEPATH}/CGController/CGLayoutFactory.cpp \
		$${IDEPATH}/CGCommand/CGCreateControl.cpp \
		$${IDEPATH}/CGCommand/CGDeleteControl.cpp \
		$${IDEPATH}/CGProject/CGProjectTree.cpp \
		$${IDEPATH}/CGCommand/CGCreateLayout.cpp \
		$${IDEPATH}/CGCommand/CGDeleteLayout.cpp \
		$${IDEPATH}/CGInstanceDelegate.cpp \
		$${IDEPATH}/CGComposer/CGCompositionProperties.cpp \
		$${IDEPATH}/CGGlossary/CGGlossaryTree.cpp \
		$${IDEPATH}/CGGlossary/CGGlossary.cpp \
		$${IDEPATH}/CGTool/CGProgressBar.cpp \
		$${IDEPATH}/CGSerialization/CGInstanceSerialization.cpp \
		$${IDEPATH}/CGSerialization/CGProjectSerialization.cpp \
		$${IDEPATH}/CGTool/CLauncher.cpp \
		$${IDEPATH}/CGTool/CGApplication.cpp \
		$${IDEPATH}/CGTool/CGUpdater.cpp \
                $${IDEPATH}/CGComposer/CGErrorDialog.cpp \
                $${IDEPATH}/CGComposer/CGThreadInProgressDialog.cpp \
                $${IDEPATH}/CGTool/CArguments.cpp \
    $${IDEPATH}/CGTool/CGDoc.cpp

HEADERS  += \
    $${IDEPATH}/CGComposer/CGComposer.h \
    $${IDEPATH}/CGProject/CGProject.h \
    $${IDEPATH}/CGToolTip.h \
    $${IDEPATH}/CGStyle.h \
    $${IDEPATH}/CGCommand/CGCreateOperator.h \
    $${IDEPATH}/CGCommandManager.h \
    $${IDEPATH}/CGCommand/CGDisconnectConnector.h \
    $${IDEPATH}/CGCommand/CGDeleteOperator.h \
    $${IDEPATH}/CGCommand/CGChangeView.h \
    $${IDEPATH}/CGCommand/CGCopyPaste.h \
    $${IDEPATH}/CGCommand/CGClear.h \
    $${IDEPATH}/CGCommand/CGConnectConnector.h \
    $${IDEPATH}/CGComposer/CGCompositionManager.h \
    $${IDEPATH}/CGComposer/CGComposition.h \
    $${IDEPATH}/CGComposer/CGCompositionView.h \
    $${IDEPATH}/CGComposer/CGCompositionFactory.h \
    $${IDEPATH}/CGComposer/CGCompositionItem.h \
    $${IDEPATH}/CGComposer/CGCompositionModel.h \
    $${IDEPATH}/CGController/CGLayout.h \
    $${IDEPATH}/CGController/CGLayoutManager.h \
    $${IDEPATH}/CGController/CGController.h \
    $${IDEPATH}/CGInstance.h \
    $${IDEPATH}/CGController/CGLayoutItem.h \
    $${IDEPATH}/CGInstanceProperties.h \
    $${IDEPATH}/CGProject/CGProjectProperties.h \
    $${IDEPATH}/CGController/CGLayoutView.h \
    $${IDEPATH}/CGController/CGLayoutFactory.h \
    $${IDEPATH}/CGCommand/CGCreateControl.h \
    $${IDEPATH}/CGCommand/CGDeleteControl.h \
    $${IDEPATH}/CGProject/CGProjectTree.h \
    $${IDEPATH}/CGCommand/CGCreateLayout.h \
    $${IDEPATH}/CGCommand/CGDeleteLayout.h \
    $${IDEPATH}/CGInstanceDelegate.h \
    $${IDEPATH}/CGComposer/CGCompositionProperties.h \
    $${IDEPATH}/CGGlossary/CGGlossaryTree.h \
    $${IDEPATH}/CGGlossary/CGGlossary.h \
    $${IDEPATH}/CGTool/CGProgressBar.h \
    $${IDEPATH}/CGSerialization/CGInstanceSerialization.h \
    $${IDEPATH}/CGSerialization/CGProjectSerialization.h \
    $${IDEPATH}/CGTool/CLauncher.h \
    $${IDEPATH}/CGTool/CGApplication.h \
    $${IDEPATH}/CGTool/CGUpdate.h \
    $${IDEPATH}/CGComposer/CGErrorDialog.h \
    $${IDEPATH}/CGComposer/CGThreadInProgressDialog.h \
    $${IDEPATH}/CGTool/CArguments.h \
    $${IDEPATH}/CGTool/CGDoc.h

RESOURCES += $${IDEPATH}/icons.qrc






