#!/bin/bash
echo "==========================================="
echo "[INFO] Start manually testing Cameleon ..."
./clean-test.sh
d=`date +%d%m%Y%H%M%S` 
IFS=$'\n'
rm -f gui-test.log
rm -f gui-test-error.log
cd deliverable/instance/bin
FILES=`find ../project/Tests/test-scd -name *.cm`
echo "[INFO] Launch first test ..."
I=0
logs=""
for FILE in $FILES; do
	echo "============================">> ../../../gui-test.log
	echo "[INFO]["$I"] Launch test file " $FILE "...">> ../../../gui-test.log
	echo "[INFO]["$I"] Launch test file " $FILE "..."
	./cameleon-cli.exe -mode="IDE" -cm=$FILE 2> ../../../gui-test-error.log
	logf=`dirname $FILE`
	logf=$logf"/log/cameleon.log"
	cat $logf >> ../../../gui-test.log
	err=`cat $logf | grep ERROR | wc -l`
	echo "[INFO]["$I"] Test ended "$FILE " with"$err "errors">> ../../../gui-test.log
	echo "[INFO]["$I"]" $err "error(s) in file "$FILE
	I=`expr $I + 1`
	echo "============================">> ../../../gui-test.log
done
nbTests=`cat ../../../gui-test.log | grep ASSERT | wc -l`
nbTestsError=`cat ../../../gui-test.log | grep ERROR | wc -l`
nbTechError=`cat ../../../gui-test-error.log | wc -l`
echo "==========================================="
echo $nbTests " passed tests"
echo $nbTestsError " tests in error"
echo $nbTechError " technical warning detected" 
echo "==========================================="
