#include "SelectorFile.h"

#include "DataBoolean.h"
#include "DataString.h"
SelectorFile::SelectorFile(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("FileSystem");
    this->setName("SelectorFile");
    this->setKey("SelectorFile");
    this->structurePlug().addPlugOut(DataString::KEY,"path.str");

    model = new QFileSystemModel();
    tree = new QTreeView();
    model->setRootPath("");

    tree->setModel(model);

    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    tree->setColumnWidth(0,400);
    //    tree.resize(640, 480);

    if(!QObject::connect(tree, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(geInformation(QModelIndex)),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect SelectorFile and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(tree);
    test = false;
    this->setLayout(lay);
    tree->setMinimumSize(400,200);
    tree->show();
}

void SelectorFile::geInformation(const QModelIndex & index){
    test = true;

    currentPath = model->filePath(index);

    this->apply();
}

CControl * SelectorFile::clone(){
    return new SelectorFile();
}


//TODO: http://stackoverflow.com/questions/3253301/howto-restore-qtreeview-last-expanded-state
string SelectorFile::toString(){
    QModelIndex index = tree->currentIndex();
    return model->filePath(index).toStdString();// box->currentText().toStdString();
}

void SelectorFile::fromString(string str){
    if(str.compare("")!=0){
        QModelIndex index = model->index(str.c_str());
        tree->setCurrentIndex(index);
        currentPath = str.c_str();
        test = true;
        this->apply();
        tree->scrollTo(index,QAbstractItemView::PositionAtTop);
    }
}

void SelectorFile::apply(){
    if(test==true)
    {
        DataString * b = new DataString();
        b->setValue(currentPath.toStdString());
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}

