#!/bin/bash
echo "==========================================="
echo "[INFO] Start adding Cameleon licence ..."
echo "[INFO] Are you sure you're not in a trunk directory with uncomitted files ? (CTRL + C)"

HEADERS=`find . -name *.h`
SOURCES=`find . -name *.cpp`
PRIS=`find . -name *.pri`
PROS=`find . -name *.pro`
I=0
for HEADER in $HEADERS; do
	#echo "============================">> ../../../run-test.log
	echo "[INFO]["$I"] Add licence to " $HEADER "..."
	mv $HEADER $HEADER.bak
	touch $HEADER
	echo "/*" >> $HEADER
	echo "Caméléon Creator & Population $1" >> $HEADER
	echo "" >> $HEADER
	echo "Copyright 2012 O. Cugnon de Sévricourt & V. Tariel"  >> $HEADER
	echo "" >> $HEADER
	echo "This software is under the MIT license :"  >> $HEADER
	echo "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:" >> $HEADER
	echo "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software." >> $HEADER
	echo "THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." >> $HEADER
	echo "" >> $HEADER
	echo "More informations can be found here: http://www.shinoe.org/cameleon" >> $HEADER
	echo "======================================*/" >> $HEADER
	cat ./$HEADER.bak >> ./$HEADER
	rm -f ./$HEADER.bak
	I=`expr $I + 1`
done

for SOURCE in $SOURCES; do
	#echo "============================">> ../../../run-test.log
	echo "[INFO]["$I"] Add licence to " $SOURCE "..."
	mv $SOURCE $SOURCE.bak
	touch $SOURCE
	echo "/*======================================" >> $SOURCE
	echo "Caméléon Creator & Population $1" >> $SOURCE
	echo "" >> $SOURCE
	echo "Copyright 2012 O. Cugnon de Sévricourt & V. Tariel"  >> $SOURCE
	echo "" >> $SOURCE
	echo "This software is under the MIT license :"  >> $SOURCE
	echo "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:" >> $SOURCE
	echo "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software." >> $SOURCE
	echo "THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." >> $SOURCE
	echo "" >> $SOURCE
	echo "More informations can be found here: http://www.shinoe.org/cameleon" >> $SOURCE
	echo "======================================*/" >> $SOURCE
	cat ./$SOURCE.bak >> ./$SOURCE
	rm -f ./$SOURCE.bak
	I=`expr $I + 1`
done

for PRI in $PRIS; do
	#echo "============================">> ../../../run-test.log
	echo "[INFO]["$I"] Add licence to " $PRI "..."
	mv $PRI $PRI.bak
	touch $PRI
	echo "#======================================" >> $PRI
	echo "#Caméléon Creator & Population $1" >> $PRI
	echo "#" >> $PRI
	echo "#Copyright 2012 O. Cugnon de Sévricourt & V. Tariel"  >> $PRI
	echo "#" >> $PRI
	echo "#This software is under the MIT license :"  >> $PRI
	echo "#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:" >> $PRI
	echo "#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software." >> $PRI
	echo "#THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." >> $PRI
	echo "" >> $PRI
	echo "#More informations can be found here: http://www.shinoe.org/cameleon" >> $PRI
	echo "#======================================" >> $PRI
	cat ./$PRI.bak >> ./$PRI
	rm -f ./$PRI.bak
	I=`expr $I + 1`
done

for PRO in $PROS; do
	#echo "============================">> ../../../run-test.log
	echo "[INFO]["$I"] Add licence to " $PRO "..."
	mv $PRO $PRO.bak
	touch $PRO
	echo "#======================================" >> $PRO
	echo "#Caméléon Creator & Population $1" >> $PRO
	echo "#" >> $PRO
	echo "#Copyright 2012 O. Cugnon de Sévricourt & V. Tariel"  >> $PRO
	echo "#" >> $PRO
	echo "#This software is under the MIT license :"  >> $PRO
	echo "#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:" >> $PRO
	echo "#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software." >> $PRO
	echo "#THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." >> $PRO
	echo "" >> $PRO
	echo "#More informations can be found here: http://www.shinoe.org/cameleon" >> $SOURCE
	echo "#======================================" >> $PRO
	cat ./$PRO.bak >> ./$PRO
	rm -f ./$PRO.bak
	I=`expr $I + 1`
done
echo "==========================================="
