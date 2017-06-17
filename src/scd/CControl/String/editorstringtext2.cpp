#include "editorstringtext2.h"

#include "DataString.h"
editorstringtext2::editorstringtext2(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("String");
    this->setName("EditorStringText2");
    this->setKey("texteditorstringtext2");
    this->structurePlug().addPlugIn(DataString::KEY,"in.str");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");

    textEdit = new QTextEdit;
    textEdit->setAlignment(Qt::AlignRight);
    textEdit->setPlainText("");
    textEdit->setMinimumHeight(100);


    QVBoxLayout *buttonLayout = new QVBoxLayout;

    buttonLayout->addWidget(textEdit);

    if(!QObject::connect(textEdit, SIGNAL(textChanged()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }

    test = false;
    this->setLayout(buttonLayout);
    this->setMinimumHeight(150);
    this->resize(300,150);
}
void editorstringtext2::keyPressEvent ( QKeyEvent * keyEvent ){
    test=true;
    CControl::keyPressEvent(keyEvent);
    this->getInformation();
}


CControl * editorstringtext2::clone(){
    return new editorstringtext2();
}

string editorstringtext2::toString(){
    return textEdit->toPlainText().toStdString();
}
void editorstringtext2::fromString(string str){
    textEdit->setPlainText(str.c_str());
    test = true;
}

void editorstringtext2::geInformation(){
    test = true;
    this->apply();
}

void editorstringtext2::apply(){
    if(test==true)
    {
        DataString * number = new DataString;
        number->setValue(textEdit->toPlainText().toStdString());
        this->sendPlugOutControl(0,number,CPlug::NEW);
    }
}
void editorstringtext2::updatePlugInControl(int , CData* data){

    DataString * num = dynamic_cast<DataString *>(data);
    textEdit->setPlainText(num->getValue().c_str());
    this->update();

}
