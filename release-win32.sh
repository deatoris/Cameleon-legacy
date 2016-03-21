#!/bin/bash
echo "[INFO] Start releasing $2 for win32 plateform ..."

#usage: release-win32.sh $TARGET_DIR $VERNUM

if [ -d $1/Cameleon-$2-win32 ]; then
echo "[WARN] Delete directory $1/Cameleon-$2-win32"
rm -rf $1/Cameleon-$2-win32
fi
#release cameleon instance
echo "[INFO] Create Cameleon instance in $1/Cameleon-$2-win32"
mkdir $1/Cameleon-$2-win32
cp deliverable/instance/cameleon.bat $1/Cameleon-$2-win32/ 
cp deliverable/instance/licence.txt $1/Cameleon-$2-win32/ 
cp deliverable/instance/readme.txt $1/Cameleon-$2-win32/ 

mkdir $1/Cameleon-$2-win32/bin/
mkdir $1/Cameleon-$2-win32/lib/
mkdir $1/Cameleon-$2-win32/log/
mkdir $1/Cameleon-$2-win32/pattern/
mkdir $1/Cameleon-$2-win32/ressource/
mkdir $1/Cameleon-$2-win32/project/

echo "[INFO] Release Cameleon instance in $1/Cameleon-$2-win32"
cp -R deliverable/instance/bin/* $1/Cameleon-$2-win32/bin/ 
cp -R deliverable/instance/pattern/* $1/Cameleon-$2-win32/pattern/ 
cp -R deliverable/instance/ressource/* $1/Cameleon-$2-win32/ressource/ 
cp -R deliverable/instance/lib/ccustomplot.dll $1/Cameleon-$2-win32/lib/ 

#release projects
echo "[INFO] Release demo projects in $1/Cameleon-$2-win32/project"
mkdir $1/Cameleon-$2-win32/project/Hello
cp -R deliverable/instance/project/Hello/* $1/Cameleon-$2-win32/project/Hello/
#mkdir $1/Cameleon-$2-win32/project/germgrain
#cp -R deliverable/instance/project/population/germgrain/* $1/Cameleon-$2-win32/project/germgrain/
#mkdir $1/Cameleon-$2-win32/project/segmentation
#cp -R deliverable/instance/project/population/segmentation/* $1/Cameleon-$2-win32/project/segmentation/

#clean release instance
find $1/Cameleon-$2-win32/ -name .svn -print0 | xargs -0 rm -rf

echo "[INFO] Cameleon release $2 has been successfully generated in $1/Cameleon-$2-win32 directory"
