#ifndef CSDCOLLECTOR_H
#define CSDCOLLECTOR_H
#include<CDictionnary.h>
class CSDDictionnary : public CDictionnary
{
public:
    CSDDictionnary();
    void collectData();
    void collectOperator();
    void collectControl();
};

#endif // CSDCOLLECTOR_H
