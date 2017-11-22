#ifndef CGCLEAR_H
#define CGCLEAR_H
#include "../CGCommandManager.h"

class CGClear : public CGCommand
{
public:
    virtual void exec();
    virtual void unexec();
    CGClear();
};

#endif // CGCLEAR_H
