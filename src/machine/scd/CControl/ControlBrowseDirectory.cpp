#include "ControlBrowseDirectory.h"

#include "ControlBrowseDirectory.h"
#include<DataString.h>
#include<DataVector.h>
#include <CGProject.h>
#include <DataBoolean.h>
ControlBrowseDirectory::ControlBrowseDirectory(QWidget * parent)
    :CControl(parent),_file("")
{
    this->path().push_back("FileSystem");
    this->setName("OpenDirectory");
    this->setKey("ControlBrowseDirectory");
    this->structurePlug().addPlugIn(DataString::KEY,"name.str");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"record.bool");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");


    applyButton = new QPushButton(tr("Browse"));
    applyButton->setAutoDefault(false);

    if(!QObject::connect(applyButton, SIGNAL( released  ()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(applyButton);
    this->setLayout(buttonLayout);
    filter = "";
    record = true;
}


void ControlBrowseDirectory::geInformation(){
    _file =QFileDialog::getExistingDirectory(this, tr("Open Dir"),CGProject::getInstance()->getPath(),QFileDialog::ShowDirsOnly).toStdString();
    _file+='/';
    this->apply();
}

void ControlBrowseDirectory::updatePlugInControl(int , CData* data){
        if(DataString * b = dynamic_cast<DataString *>(data))
        {
            applyButton->setText(b->getValue().c_str());
            this->update();
       }
        if(DataBoolean * b = dynamic_cast<DataBoolean *>(data))
        {
            record = b->getValue();
       }
}

CControl * ControlBrowseDirectory::clone(){
    return new ControlBrowseDirectory();
}

string ControlBrowseDirectory::toString(){
    if(record) return _file;
    else return "";
}

void ControlBrowseDirectory::fromString(string str){
    _file = str;
}

void ControlBrowseDirectory::apply(){
    if(_file!=""&&this->isPlugOutConnected(0)==true)
    {
        DataString * data = new DataString;
        data->setValue(_file);
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }
}
