#!/bin/bash
echo "[INFO] Start releasing $2 for linux64 plateform ..."
#PATH_QT_LIB="/home/vincent/QtSDK/Desktop/Qt/4.8.0/gcc/lib/"
#PATH_GLUT_LIB="/usr/lib/x86_64-linux-gnu/"
#usage: release-linux64.sh $TARGET_DIR $VERNUM

if [ -d $1/Cameleon-$2-linux64 ]; then
echo "[WARN] Delete directory $1/Cameleon-$2-linux64"
rm -rf $1/Cameleon-$2-linux64
fi
#release cameleon instance
echo "[INFO] Create Cameleon instance in $1/Cameleon-$2-linux64"
mkdir $1/Cameleon-$2-linux64
cp deliverable/instance/cameleon.sh $1/Cameleon-$2-linux64/ 
cp deliverable/instance/licence.txt $1/Cameleon-$2-linux64/ 
cp deliverable/instance/readme.txt $1/Cameleon-$2-linux64/ 

mkdir $1/Cameleon-$2-linux64/bin/
mkdir $1/Cameleon-$2-linux64/lib/
mkdir $1/Cameleon-$2-linux64/log/
mkdir $1/Cameleon-$2-linux64/pattern/
mkdir $1/Cameleon-$2-linux64/ressource/
mkdir $1/Cameleon-$2-linux64/project/

echo "[INFO] Release Cameleon instance in $1/Cameleon-$2-linux64"
cp -R deliverable/instance/bin/* $1/Cameleon-$2-linux64/bin/ 
cp -R deliverable/instance/pattern/* $1/Cameleon-$2-linux64/pattern/ 
cp -R deliverable/instance/ressource/* $1/Cameleon-$2-linux64/ressource/ 
cp -R deliverable/instance/lib/libccustomplot.so $1/Cameleon-$2-linux64/lib/ 
#cp -d ${PATH_QT_LIB}/*.so* $1/Cameleon-$2-linux64/bin/
#cp -d ${PATH_QT_LIB}/*.a $1/Cameleon-$2-linux64/bin/
#cp -d ${PATH_GLUT_LIB}/libglut.* $1/Cameleon-$2-linux64/bin/    
chmod 777 $1/Cameleon-$2-linux64/cameleon.sh
#release projects
echo "[INFO] Release demo projects in $1/Cameleon-$2-linux64/project"
mkdir $1/Cameleon-$2-linux64/project/Hello
cp -R deliverable/instance/project/Hello/* $1/Cameleon-$2-linux64/project/Hello/
#mkdir $1/Cameleon-$2-linux64/project/germgrain
#cp -R deliverable/instance/project/population/germgrain/* $1/Cameleon-$2-linux64/project/germgrain/
#mkdir $1/Cameleon-$2-linux64/project/segmentation
#cp -R deliverable/instance/project/population/segmentation/* $1/Cameleon-$2-linux64/project/segmentation/

#clean release instance
find $1/Cameleon-$2-linux64/ -name .svn -print0 | xargs -0 rm -rf

echo "[INFO] Cameleon release $2 has been successfully generated in $1/Cameleon-$2-linux64 directory"
