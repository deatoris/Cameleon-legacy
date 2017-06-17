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
    buttonLayout->addWidget(textEdit);
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
void EditorText::apply(){
    if(test==true)
    {
        DataString * number = new DataString;
        number->setValue(textEdit->toPlainText().toStdString());
        this->sendPlugOutControl(0,number,CPlug::NEW);
    }
}
