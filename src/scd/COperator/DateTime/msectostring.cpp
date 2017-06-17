#include "msectostring.h"

msectostring::msectostring(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"input.num");
    this->structurePlug().addPlugOut(DataString::KEY,"time.str");
    this->path().push_back("DateTime");
    this->setKey("datetimemsectostring");
    this->setName("msectostring");
}

void msectostring::exec(){
    DataNumber* input = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData());
    DataString* output = dynamic_cast<DataString *>(this->plugOut()[0]->getData());

    output->setValue(this->getPrintableProbe(input->getValue()));
}

COperator * msectostring::clone(){
    return new msectostring();
}

string msectostring::getPrintableProbe(int lastPerf){
    string sperf = "";
    long perfSecondes = lastPerf/1000;
    long perfMin = perfSecondes/60;
    long perfH = perfMin/60;
    if(perfMin > 1){
        ostringstream str;
        str << perfMin;
        sperf += str.str();
        sperf += "min";
        return sperf;
    }
    if(perfH > 1){
        ostringstream str;
        str << perfH;
        sperf += str.str();
        sperf += "h";
        return sperf;
    }
    if(perfSecondes < 1){
        ostringstream str;
        str << lastPerf;
        sperf += str.str();
        sperf += "ms";
        return sperf;
    }
    if(perfSecondes >= 1){
        ostringstream str;
        str << perfSecondes;
        sperf += str.str();
        sperf += "s";
        return sperf;
    }
    ostringstream str;
    str << lastPerf;
    sperf += str.str();
    sperf += "ms";
    return sperf;
}
