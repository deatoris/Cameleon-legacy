#!/bin/bash
echo "[INFO] Start generating dictionary $1 ..."

if [ -d $1 ]; then
echo "[WARN] Delete directory $1"
rm -rf $1
fi
#blank.pro
#$${BLANKPATH}/Control \
#$${BLANKPATH}/Operator  \
#$${BLANKPATH}/Data
#$${BLANKPATH}/blankplugin.cpp \
#$${BLANKPATH}/blankDictionnary.cpp \
#$${BLANKPATH}/Operator/BlankOperatorExample.cpp \
#$${BLANKPATH}/Data/BlankDataExample.cpp \
#$${BLANKPATH}/Control/BlankViewExample.cpp \
#$${BLANKPATH}/Control/BlankEditorExample.cpp
#$${BLANKPATH}/blankplugin.h \
#$${BLANKPATH}/blankDictionnary.h \
#$${BLANKPATH}/Operator/BlankOperatorExample.h \
#$${BLANKPATH}/Data/BlankDataExample.h \
#$${BLANKPATH}/Control/BlankViewExample.h \
#$${BLANKPATH}/Control/BlankEditorExample.h
mkdir $1
mkdir $1/Control
mkdir $1/Operator
mkdir $1/Data
var="/blank/$1/"
echo "[INFO] Create $1/$1.pro"
cat blank/blank.pro | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/$1.pro
echo "[INFO] Create $1/$1plugin.cpp"
cat blank/blankplugin.cpp | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/$1plugin.cpp
echo "[INFO] $1/$1plugin.h"
cat blank/blankplugin.h | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/$1plugin.h
echo "[INFO] Create $1/$1Dictionnary.cpp"
cat blank/blankDictionnary.cpp | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/$1Dictionnary.cpp
echo "[INFO] Create $1/$1Dictionnary.h"
cat blank/blankDictionnary.h | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/$1Dictionnary.h
echo "[INFO] Create $1DataExample.cpp"
cat blank/Data/BlankDataExample.cpp | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Data/$1DataExample.cpp
echo "[INFO] Create $1DataExample.h"
cat blank/Data/BlankDataExample.h | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Data/$1DataExample.h
echo "[INFO] Create $1OperatorExample.cpp"
cat blank/Operator/BlankOperatorExample.cpp | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Operator/$1OperatorExample.cpp
echo "[INFO] Create $1OperatorExample.h"
cat blank/Operator/BlankOperatorExample.h | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Operator/$1OperatorExample.h
echo "[INFO] Create $1ViewExample.cpp"
cat blank/Control/BlankViewExample.cpp | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Control/$1ViewExample.cpp
echo "[INFO] Create $1ViewExample.h"
cat blank/Control/BlankViewExample.h | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Control/$1ViewExample.h
echo "[INFO] Create $1EditorExample.cpp"
cat blank/Control/BlankEditorExample.cpp | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Control/$1EditorExample.cpp
echo "[INFO] Create $1EditorExample.h"
cat blank/Control/BlankEditorExample.h | sed -e "s/blank/$1/g" | sed -e "s/Blank/$1/g" | sed -e "s/BLANK/$1/g" >> $1/Control/$1EditorExample.h
echo "[INFO] Dictionary $1 has been successfully generated in directory $1"
