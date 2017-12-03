/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CControlRessource.h"
#include "CGInstance.h"
#include<DataString.h>
#include "CGProject.h"
CControlRessource::CControlRessource(QWidget * parent)
    :CControl(parent),_file("")
{
    this->path().push_back("Introspection");
    this->path().push_back("Caméléon");
    this->setName("Ressource");
    this->setKey("Ressource");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");

    label = new QLabel();
    labelIsDir = new QLabel();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(labelIsDir);
    this->setLayout(layout);
    this->setMinimumSize(140,60);
}

CControl * CControlRessource::clone(){
    return new CControlRessource();
}

string CControlRessource::toString(){
    return CGInstance::getInstance()->makeRelative(_file.c_str()).toStdString();
}

void CControlRessource::fromString(string str){
    _file = CGInstance::getInstance()->makeAbsolute(str.c_str()).toStdString();
    QFileInfo info(str.c_str());
    label->setText(info.fileName());
    if(info.isDir())labelIsDir->setText("directory");
    if(info.isFile())labelIsDir->setText("file");

    this->apply();
    this->update();
}

void CControlRessource::apply(){
    if(_file!="")
    {
        DataString * data = new DataString;
        data->setData(&_file);
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }
}

CControlRessourceProject::CControlRessourceProject(QWidget * parent)
    :CControl(parent),_file("")
{
    this->path().push_back("Introspection");
    this->path().push_back("Caméléon");
    this->setName("RessourceProject");
    this->setKey("RessourceProject");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");

    label = new QLabel();
    labelIsDir = new QLabel();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(labelIsDir);
    this->setLayout(layout);
    this->setMinimumSize(140,60);
}

CControl * CControlRessourceProject::clone(){
    return new CControlRessourceProject();
}

string CControlRessourceProject::toString(){
    return CGProject::getInstance()->makeRelative(_file.c_str()).toStdString();
}

void CControlRessourceProject::fromString(string str){
    _file = CGProject::getInstance()->makeAbsolute(str.c_str()).toStdString();
    QFileInfo info(str.c_str());
    label->setText(info.fileName());
    if(info.isDir())labelIsDir->setText("directory");
    if(info.isFile())labelIsDir->setText("file");

    this->apply();
    this->update();
}

void CControlRessourceProject::apply(){
    if(_file!="")
    {
        DataString * data = new DataString;
        data->setData(&_file);
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }
}
