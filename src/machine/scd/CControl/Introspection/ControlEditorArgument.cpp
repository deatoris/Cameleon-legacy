#include "ControlEditorArgument.h"
#include<DataString.h>
#include<CUtilitySTL.h>
string ControlEditorArgument::KEY = "ControlEditorArgument";
ControlEditorArgument::ControlEditorArgument(QWidget * parent )
    :CControl(parent)
{
    this->path().push_back("Cli");
    this->setName("Argument");
    this->setKey(ControlEditorArgument::KEY);
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");


    _l_arg = new QLabel;
    _l_arg->setText("Key");
    _l_default = new QLabel;
    _l_default->setText("Default");
    _l_info = new QLabel;
    _l_info->setText("Info");

    _arg = new QLineEdit;
    _default = new QLineEdit;
    _info = new QLineEdit;


    if(!QObject::connect(_arg, SIGNAL(editingFinished()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }
    _box = new QCheckBox;
    _label = new QLabel;
    _label->setText("Optional");


    QHBoxLayout *v0Layout = new QHBoxLayout;
    v0Layout->addWidget(_label);
    v0Layout->addWidget(_box);

    QVBoxLayout *v1Layout = new QVBoxLayout;

    v1Layout->addWidget(_l_arg);
    v1Layout->addWidget(_l_default);
    v1Layout->addWidget(_l_info);
    QVBoxLayout *v2Layout = new QVBoxLayout;
    v2Layout->addWidget(_arg);
    v2Layout->addWidget(_default);
    v2Layout->addWidget(_info);
    QHBoxLayout *v3layout = new QHBoxLayout;
    v3layout->addLayout(v1Layout);
    v3layout->addLayout(v2Layout);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(v3layout);
    layout->addLayout(v0Layout);

    this->setLayout(layout);
}
void ControlEditorArgument::geInformation(){
    this->apply();
}

CControl * ControlEditorArgument::clone(){
    return new ControlEditorArgument;
}

string ControlEditorArgument::toString(){
    string boxx;
    if(_box->isChecked()){
        boxx =  "TRUE";
    }else{
        boxx = "FALSE";
    }
    return _arg->text().toStdString()+"<$>"+_default->text().toStdString()+"<$>"+_info->text().toStdString()+"<$>"+boxx+"<$>";
}

void ControlEditorArgument::fromString(string str){
    if(str!=""){
        istringstream  in(str.c_str());
        string str =SCD::UtilityString::getline(in,"<$>");
        _arg->setText(str.c_str());
        str =SCD::UtilityString::getline(in,"<$>");
        _default->setText(str.c_str());
        str =SCD::UtilityString::getline(in,"<$>");
        _info->setText(str.c_str());
        str =SCD::UtilityString::getline(in,"<$>");
        if(str.compare("TRUE") == 0){
            _box->setChecked(true);
        }else{
            _box->setChecked(false);
        }

    }
}
bool ControlEditorArgument::getOptional(){
    if(_box->isChecked()){
        return  true;
    }else{
        return false;
    }
}

string ControlEditorArgument::getKeyArgument(){
    return _arg->text().toStdString();
}

void ControlEditorArgument::setArgument(string argument){
    _argument = argument;
}

void ControlEditorArgument::apply(){
    if(_argument!=""){
        DataString * data = new DataString;
        data->setValue(_argument);
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }else if(_default->text()!=""){
        DataString * data = new DataString;
        data->setValue(_default->text().toStdString());
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }
}
