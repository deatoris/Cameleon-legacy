#!/bin/bash
#usage: release-sdk.sh $TARGET_DIR $VERNUM
echo "[INFO] Start releasing sdk $2 ..."
if [ -d $1/Cameleon-$2-sdk ]; then
echo "[WARN] Delete directory $1/Cameleon-$2-sdk ..."
rm -rf $1/Cameleon-$2-sdk
fi
mkdir $1/Cameleon-$2-sdk
echo "[INFO] Copy SDK ..."
cp -R . $1/Cameleon-$2-sdk
echo "[INFO] Clean target SDK release ..."
echo "[INFO] delete svn files ..."
find $1/Cameleon-$2-sdk/ -name .svn -print0 | xargs -0 rm -rf
echo "[INFO] delete log files ..."
find $1/Cameleon-$2-sdk/ -name "*.log" -print0 | xargs -0 rm -rf
echo "[INFO] delete .pro.user files ..."
find $1/Cameleon-$2-sdk/ -name *.pro.user -print0 | xargs -0 rm -rf
echo "[INFO] delete .exe files ..."
find $1/Cameleon-$2-sdk/ -name *.exe -print0 | xargs -0 rm -rf
echo "[INFO] delete .dll files ..."
find $1/Cameleon-$2-sdk/ -name *.dll -print0 | xargs -0 rm -rf
echo "[INFO] delete Makefile files ..."
find $1/Cameleon-$2-sdk/ -name *Makefile* -print0 | xargs -0 rm -rf
echo "[INFO] delete Makefile.* files ..."
find $1/Cameleon-$2-sdk/ -name Makefile.* -print0 | xargs -0 rm -rf
echo "[INFO] delete debug files ..."
find $1/Cameleon-$2-sdk/ -name debug -print0 | xargs -0 rm -rf
echo "[INFO] delete release files ..."
find $1/Cameleon-$2-sdk/ -name release -print0 | xargs -0 rm -rf
echo "[INFO] delete .Debug files ..."
find $1/Cameleon-$2-sdk/ -name *.Debug -print0 | xargs -0 rm -rf
echo "[INFO] delete .Release files ..."
find $1/Cameleon-$2-sdk/ -name *.Release -print0 | xargs -0 rm -rf
echo "[INFO] delete .conf files ..."
find $1/Cameleon-$2-sdk/ -name *.conf -print0 | xargs -0 rm -rf
echo "[INFO] delete .a files ..."
find $1/Cameleon-$2-sdk/ -name *.a -print0 | xargs -0 rm -rf
echo "[INFO] Cameleon SDK release $2 has been successfully generated in $1/Cameleon-$2-sdk directory"
