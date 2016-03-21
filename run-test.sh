#!/bin/bash
echo "==========================================="
echo "[INFO] Start testing Cameleon ..."
./clean-test.sh
d=`date +%d%m%Y%H%M%S` 
IFS=$'\n'
rm -f run-test.log
rm -f run-test-error.log
cd deliverable/instance/bin
FILES=`find ../project/Tests/test-auto -name *.cm`
echo "[INFO] Launch first test ..."
I=0
logs=""
for FILE in $FILES; do
	echo "============================">> ../../../run-test.log
	echo "[INFO]["$I"] Launch test file " $FILE "...">> ../../../run-test.log
	echo "[INFO]["$I"] Launch test file " $FILE "..."
	./cameleon-cli.exe -mode="CLI" -cm=$FILE 2> ../../../run-test-error.log
	logf=`dirname $FILE`
	logf=$logf"/log/cameleon.log"
	cat $logf >> ../../../run-test.log
	err=`cat $logf | grep ERROR | wc -l`
	echo "[INFO]["$I"] Test ended "$FILE " with"$err "errors">> ../../../run-test.log
	echo "[INFO]["$I"]" $err "error(s) in file "$FILE
	I=`expr $I + 1`
	echo "============================">> ../../../run-test.log
done
nbTests=`cat ../../../run-test.log | grep ASSERT | wc -l`
nbTestsError=`cat ../../../run-test.log | grep ERROR | wc -l`
nbTechError=`cat ../../../run-test-error.log | wc -l`
echo "==========================================="
echo $nbTests " passed tests"
echo $nbTestsError " tests in error"
echo $nbTechError " technical warning detected" 
echo "==========================================="
