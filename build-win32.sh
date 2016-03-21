#!/bin/bash
#usage: release-sdk.sh $TARGET_DIR $VERNUM
echo "[INFO] Start build Caméléon Creator $2 ..."
export PATH="C:\\QtSDK\\mingw\\bin\\;c:\\qtsdk\\desktop\\qt\\4.7.3\\mingw\bin\\"
echo $PATH
"C:\QtSDK\mingw\bin\mingw32-make.exe" clean
"c:\qtsdk\desktop\qt\4.7.3\mingw\bin\qmake.exe" cameleon.pro -r -spec win32-g++
"C:\QtSDK\mingw\bin\mingw32-make.exe"
echo "[INFO] Build finished"