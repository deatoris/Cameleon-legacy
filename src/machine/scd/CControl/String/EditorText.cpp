#include "EditorText.h"

#include "DataString.h"
EditorText::EditorText(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("String");
    this->setName("EditorStringText");
    this->setKey("textEditorText");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");

    textEdit = new QTextEdit;
    textEdit->setAlignment(Qt::AlignRight);
    textEdit->setPlainText("");
    textEdit->setMinimumHeight(100);


    QVBoxLayout *buttonLayout = new QVBoxLayout;
    QPushButton* save = new QPushButton("save");

    buttonLayout->addWidget(textEdit);
    buttonLayout->addWidget(save);

    if(!QObject::connect(save, SIGNAL(clicked()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }

    test = false;
    this->setLayout(buttonLayout);
    this->setMinimumHeight(150);
    this->resize(300,150);
}
void EditorText::keyPressEvent ( QKeyEvent * keyEvent ){
    test=true;
    CControl::keyPressEvent(keyEvent);
}


CControl * EditorText::clone(){
    return new EditorText();
}

string EditorText::toString(){
    return textEdit->toPlainText().toStdString();
}
void EditorText::fromString(string str){
    textEdit->setPlainText(str.c_str());
    test = true;
}

void EditorText::geInformation(){
    test = true;
    this->apply();
}

void EditorText::apply(){
    if(test==true)
    {
        DataString * number = new DataString;
        number->setValue(textEdit->toPlainText().toStdString());
        this->sendPlugOutControl(0,number,CPlug::NEW);
    }
}
