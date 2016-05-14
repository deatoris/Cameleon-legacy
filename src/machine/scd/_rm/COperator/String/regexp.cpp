#include "regexp.h"


regexp::regexp(){
    this->structurePlug().addPlugIn(DataString::KEY,"string.str");
    this->structurePlug().addPlugIn(DataString::KEY,"regexp.str");
    this->structurePlug().addPlugOut(DataVector::KEY,"v_out.str");
    this->path().push_back("String");
    this->setKey("stringregexpstring");
    this->setName("SplitRegExpString");
    this->setInformation("Splits the string into substrings wherever the regular expression regexp matches\nThe substrings are include in a vector of DataString");
}

void regexp::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string v2 = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();

    QString str(v1.c_str());
    QString pa(v2.c_str());

    QRegExp rx(pa);//ex: "^[\\w|\\.]+@[\\w]+\\.[\\w]{2,4}$"

    //exemple from here: http://doc.qt.nokia.com/latest/qregexp.html
    QStringList list;
    int pos = 0;
    vector<shared_ptr<CData> > * v = new vector<shared_ptr<CData> > ;

    QString sr = str;
    while ((pos = rx.indexIn(sr, pos)) != -1) {
        pos += rx.matchedLength();
        QStringList capts = rx.capturedTexts();

        QString result;
        foreach (const QString &str2, capts) {
            result += str2;
        }

        list << result;
        DataString* st = new DataString();
        st->setValue(result.toStdString());
        v->push_back(shared_ptr<CData>(st));

    }
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(DataVector::DATAVECTOR(v));
}

COperator * regexp::clone(){
    return new regexp();
}
