#include "EditorCheckBox.h"

#include "DataBoolean.h"
EditorCheckBox::EditorCheckBox(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Boolean");
    this->setName("EditorCheckBox");
    this->setKey("EditorCheckBox");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"default.bool");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");

    box = new QCheckBox;

    if(!QObject::connect(box, SIGNAL(stateChanged(int)),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorCheckBox and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void EditorCheckBox::geInformation(){
    test = true;
    this->apply();
}

CControl * EditorCheckBox::clone(){
    return new EditorCheckBox();
}

string EditorCheckBox::toString(){
    if(box->isChecked()){
        return "TRUE";
    }else{
        return "FALSE";
    }
}

void EditorCheckBox::fromString(string str){
    if(str.compare("TRUE") == 0){
        box->setChecked(true);
    }else{
        box->setChecked(false);
    }
    test = true;
}

void EditorCheckBox::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        DataBoolean * b = new DataBoolean;
        b->setValue(box->isChecked());
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}

void EditorCheckBox::updatePlugInControl(int ,CData* data,CPlug::State state){
    if(state==CPlug::NEW){
        if(DataBoolean * b = dynamic_cast<DataBoolean *>(data))
        {
            box->setChecked(b->getValue());
            this->update();
        }
    }
}
