#include "editorstringwithinput.h"

#include "DataString.h"
EditorStringWithInput::EditorStringWithInput(QWidget * parent)
    :EditorString(parent)
{
    this->setName("EditorStringLine(input)");
    this->setKey("LineEditorStringWithInput");
    this->structurePlug().addPlugIn(DataString::KEY,"init.str");

}

CControl * EditorStringWithInput::clone(){
    return new EditorStringWithInput();
}

void EditorStringWithInput::updatePlugInControl(int , CData* data){
    DataString * num = dynamic_cast<DataString *>(data);
    lineEdit->setText(num->getValue().c_str());
    this->update();
    this->apply();
}
