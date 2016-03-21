#include "EditorTable.h"

#include <CData.h>
#include "DataString.h"
#include "DataTable.h"

QString EditorTable::defaultString(){
    return "";
}
QString EditorTable::defaultHeaderColumn(int col){
    return QString::number(col);
}

QString EditorTable::defaultHeaderRaw(int raw){
    return QString::number(raw);
}

EditorTable::EditorTable(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Table");
    this->setName("EditorTable");
    this->setKey("EditorTable");
    this->structurePlug().addPlugOut(DataTable::KEY,"out.table");

    box = new QTableWidget;
    box->setRowCount(0);
    box->setColumnCount(0);

    buttoncolumn = new QPushButton("add col");
    buttonraw = new QPushButton("add raw");
    rmbuttoncolumn = new QPushButton("rm col");
    rmbuttonraw = new QPushButton("rm raw");

    save = new QPushButton("save");

    if(!QObject::connect(buttonraw, SIGNAL(clicked()),this, SLOT(addRaw()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }

    if(!QObject::connect(buttoncolumn, SIGNAL(clicked()),this, SLOT(addColumn()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }
    if(!QObject::connect(rmbuttonraw, SIGNAL(clicked()),this, SLOT(rmRaw()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }

    if(!QObject::connect(rmbuttoncolumn, SIGNAL(clicked()),this, SLOT(rmColumn()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }


    if(!QObject::connect(save, SIGNAL(clicked()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorTable and box" ;
    }

    QHBoxLayout* bLay = new QHBoxLayout();
    bLay->addWidget(buttonraw);
    bLay->addWidget(buttoncolumn);
    bLay->addWidget(rmbuttonraw);
    bLay->addWidget(rmbuttoncolumn);
        bLay->addWidget(save);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addLayout(bLay);
    lay->addWidget(box);
//    lay->addWidget(save);
    test = false;
    this->setLayout(lay);
    this->setMinimumWidth(300);
    currentEdit = -1;
}

void EditorTable::geInformation(){
    test = true;
    this->apply();
}

void EditorTable::showEdit(int columnIndex){
    if(currentEdit == -1){
        QLineEdit* edit = mapEdit.value(columnIndex);
        int x = box->horizontalHeader()->sectionPosition(columnIndex);
        edit->setGeometry(x,0,edit->width(),edit->height());
        QTableWidgetItem* item = box->horizontalHeaderItem(columnIndex);
        edit->setText(item->text());
        edit->show();
        currentEdit = columnIndex;
    }
}

void EditorTable::changeColumnName(){
    if(currentEdit != -1){
        QTableWidgetItem* item = box->horizontalHeaderItem(currentEdit);
        QLineEdit* edit = mapEdit.value(currentEdit);
        item->setText(edit->text());
        edit->hide();
        currentEdit = -1;
    }
}
void EditorTable::rmRaw(){
    int count = box->rowCount();
    if(count>0){
    box->setRowCount(count-1);
    this->update();
    }
}
void EditorTable::rmColumn(){
    int count = box->columnCount();
    if(count>0){
    box->setColumnCount(count-1);
    this->update();
    }
}
void EditorTable::addColumn(){
    int count = box->columnCount();
    box->setColumnCount(count+1);

    for(int i = 0;i<box->rowCount();i++){
        QTableWidgetItem* item = new QTableWidgetItem(defaultString());
        box->setItem(i,count,item);
    }

    //header editor managment
    QString cName = defaultHeaderColumn(count);
    QTableWidgetItem* itemName = new QTableWidgetItem(cName);
    QLineEdit* edit = new QLineEdit(box->horizontalHeader());
    mapEdit.insert(count,edit);
    connect(box->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(showEdit(int)));
    //    connect(edit, SIGNAL(returnPressed()), this, SLOT(changeColumnName()));
    connect(edit, SIGNAL(editingFinished()), this, SLOT(changeColumnName()));
    box->setHorizontalHeaderItem(count,itemName);
    this->update();
}

void EditorTable::addRaw(){
    int count = box->rowCount();
    box->setRowCount(count+1);
    for(int i = 0;i<box->columnCount();i++){
        QTableWidgetItem* item = new QTableWidgetItem(defaultString());
        box->setItem(count,i,item);
    }
    //row editor managment
    QString cName = defaultHeaderRaw(count);
    QTableWidgetItem* itemName = new QTableWidgetItem(cName);
//    QLineEdit* edit = new QLineEdit(box->horizontalHeader());
//    connect(edit, SIGNAL(editingFinished()), this, SLOT(changeColumnName()));
    box->setVerticalHeaderItem(count,itemName);

    this->update();
}

CControl * EditorTable::clone(){
    return new EditorTable();
}

string EditorTable::toString(){
    int rcount = box->rowCount();
    int ccount = box->columnCount();
    Table t(ccount,rcount);
    for(int i=0;i<rcount;i++){
        for(int j=0;j<ccount;j++){
            QTableWidgetItem* item = box->item(i,j);
            t(j,i)=item->text().toStdString();
        }

    }
    for(int j=0;j<ccount;j++){
        QTableWidgetItem* item = box->horizontalHeaderItem(j);
        string header = item->text().toStdString();
        t.setHeader(j,header);
    }

    std::ostringstream oss;
    oss<<t;
    return oss.str();
}

void EditorTable::fromString(string str){
    if(str.compare("")!=0){
        Table t;
        std::istringstream iss(str);
        iss >> t;
        while(box->columnCount()!=t.sizeCol()){
            if(box->columnCount()<t.sizeCol()){
                addColumn();
            }else{
                rmColumn();
            }
        }
        while(box->rowCount()!=t.sizeRow()){
            if(box->rowCount()<t.sizeRow()){
                addRaw();
            }else{
                rmRaw();
            }
        }
        for(int i=0;i<t.sizeCol();i++){
            for(int j=0;j<t.sizeRow();j++){
                QTableWidgetItem* item = box->item(j,i);
                item->setText(t(i,j).c_str());
            }
        }

        test = true;
    }
}

void EditorTable::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        int rcount = box->rowCount();
        int ccount = box->columnCount();
        Table* t = new Table(ccount,rcount);
        for(int i=0;i<rcount;i++){
            for(int j=0;j<ccount;j++){
                QTableWidgetItem* item = box->item(i,j);
                t->operator ()(j,i)=item->text().toStdString();
            }
        }
        for(int j=0;j<ccount;j++){
            QTableWidgetItem* item = box->horizontalHeaderItem(j);
            string header = item->text().toStdString();
            t->setHeader(j,header);
        }
        DataTable * table = new DataTable;
        shared_ptr<Table> st(t);
        table->setData(st);
        this->sendPlugOutControl(0,table,CPlug::NEW);
    }
}
