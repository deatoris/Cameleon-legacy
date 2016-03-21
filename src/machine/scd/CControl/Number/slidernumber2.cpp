#include "slidernumber2.h"

#include "DataNumber.h"
slidernumber2::slidernumber2(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Number");
    this->setName("slider");
    this->setKey("slidernumber2");
    this->structurePlug().addPlugIn(DataNumber::KEY,"min.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"max.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"step.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"num.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"out.num");

    valmax = 256;
    valmin = 0;
    step = 1;

    slider = new QSlider;
    slider->setMinimum(valmin);
    slider->setMaximum(valmax);
    slider->setSingleStep(step);
    slider->setOrientation(Qt::Horizontal);
    min = new QLabel;
    max = new QLabel;
    current = new QLabel;

    min->setText("0");
    max->setText("256");
    current->setText("50");

    slider->setValue(50);

    if(!QObject::connect(slider, SIGNAL( sliderMoved(int)),this, SLOT(sliderMove(int)),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    if(!QObject::connect(slider, SIGNAL( valueChanged(int)),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    QVBoxLayout* vLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(min);
    hLayout->addWidget(slider);
    hLayout->addWidget(max);
    vLayout->addLayout(hLayout);

    QToolBar* bar = new QToolBar("Player bar", this);
    QAction* nextStepAction= new QAction(QIcon(":/icons/control_start.png"),
                                         tr("next step"), this);
    nextStepAction->setStatusTip(tr("next step"));
    connect(nextStepAction, SIGNAL(triggered()),
            this, SLOT(previous()));
    QAction* prevStepAction= new QAction(QIcon(":/icons/control_end.png"),
                                         tr("prev step"), this);
    prevStepAction->setStatusTip(tr("next step"));
    connect(prevStepAction, SIGNAL(triggered()),
            this, SLOT(next()));
    bar->addAction(nextStepAction);
    bar->addAction(prevStepAction);
    hLayout->addWidget(bar);

    //vLayout->addWidget(current);


    test = false;
    this->setLayout(vLayout);
}

void slidernumber2::previous(){
    int v = slider->value();
    slider->setValue(v-1);
}

void slidernumber2::next(){
    int v = slider->value();
    slider->setValue(v+1);

}

void slidernumber2::geInformation(){
    test = true;
    this->apply();
}

void slidernumber2::sliderMove(int m){
    //    current->setText(QString::number(m));
    //    this->update();
}

CControl * slidernumber2::clone(){
    return new slidernumber2();
}

string slidernumber2::toString(){
    //min;max;step;value
    QString out = QString::number(slider->minimum())+";"+QString::number(slider->maximum())+";"+QString::number(slider->singleStep())+";"+QString::number(slider->value());
    return out.toStdString();
}

void slidernumber2::fromString(string str){
    //min;max;step;value
    QString s(str.c_str());
    QStringList sl = s.split(";");
    if(sl.size() == 4){
        QString smin=sl[0];
        QString smax=sl[1];
        QString sstep=sl[2];
        QString svalue=sl[3];

        int imin = smin.toInt();
        int imax = smax.toInt();
        int istep = sstep.toInt();
        int ivalue = svalue.toInt();

        slider->setMinimum(imin);
        slider->setMaximum(imax);
        slider->setSingleStep(istep);
        slider->setValue(ivalue);

        min->setText(smin);
        max->setText(smax);
        current->setText(svalue);

        test = true;
        this->update();
    }
}

void slidernumber2::updatePlugInControl(int indexplugin, CData* data){
    if(indexplugin == 0){ //min
        DataNumber * num = dynamic_cast<DataNumber *>(data);
        slider->setMinimum(num->getValue());
        min->setText(QString::number(num->getValue()));
        this->update();
    }
    if(indexplugin == 1){ //max
        DataNumber * num = dynamic_cast<DataNumber *>(data);
        slider->setMaximum(num->getValue());
        max->setText(QString::number(num->getValue()));
        this->update();
    }
    if(indexplugin == 2){ //step
        DataNumber * num = dynamic_cast<DataNumber *>(data);
        slider->setSingleStep(num->getValue());
        this->update();
    }
    if(indexplugin == 3){ //set value
        DataNumber * num = dynamic_cast<DataNumber *>(data);
        slider->setValue(num->getValue());
        this->update();
    }
}
void slidernumber2::apply(){
    if(test==true)
    {
        DataNumber * number = new DataNumber;
        number->setValue(slider->value());
        current->setText(QString::number(slider->value()));
        this->sendPlugOutControl(0,number,CPlug::NEW);
    }
}
