#ifndef CGDELETELAYOUT_H
#define CGDELETELAYOUT_H
#include "CGLayout.h"
#include "../CGCommandManager.h"
class CGDeleteLayout: public CGCommand
{
public:
    CGDeleteLayout();
    virtual void exec();
    virtual void unexec();
    void setLayout(CGLayout* layout);
private:
    CGLayout* layout;
};

#endif // CGDELETELAYOUT_H
