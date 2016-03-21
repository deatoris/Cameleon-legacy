/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "BlankOperatorExample.h"

#include "CLogger.h"
#include<CMachine.h>
#include "DataString.h"
//#include "DataImageGrid.h"
//#include "DataNumber.h"
//#include "cv.h"
//#include "highgui.h"
#include "BlankDataExample.h"
BlankOperatorExample::BlankOperatorExample(){
    this->path().push_back("blank");
    this->setKey("BlankOperatorExample");
    this->setName("blank");
    this->setInformation("Your blank help!");
    this->structurePlug().addPlugIn(BlankDataExample::KEY,"in.blank");
    this->structurePlug().addPlugOut(BlankDataExample::KEY,"out.blank");
}

void BlankOperatorExample::exec(){
    BlankDataExample* inData = dynamic_cast<BlankDataExample *>(this->plugIn()[0]->getData());
    BlankDataExample* outData = dynamic_cast<BlankDataExample *>(this->plugOut()[0]->getData());
    inData->setValue(outData->getValue());
}

COperator * BlankOperatorExample::clone(){
    return new BlankOperatorExample();
}

