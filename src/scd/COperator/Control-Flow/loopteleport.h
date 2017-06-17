#ifndef LOOPTELEPORT_H
#define LOOPTELEPORT_H

#include<COperator.h>
#include<CData.h>


class forteleport :public COperator
{
private:
    bool _firstiteration;
    bool _callbyexitloop;
    int _nbr_iteration;
public:
    bool sendData(vector<CData*> v_c);
    forteleport();
    virtual void exec();
    virtual COperator * clone();
    bool executionCondition();
    void action(string key);
};

#endif // LOOPTELEPORT_H
