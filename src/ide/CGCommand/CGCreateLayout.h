#ifndef CGCREATELAYOUT_H
#define CGCREATELAYOUT_H
#include "../CGCommandManager.h"
class CGLayout;
class CGCreateLayout: public CGCommand
{
public:
    virtual void exec();
    virtual void unexec();
    CGCreateLayout();
    void setName(QString name);
    void setType(QString item);
private:
    QString name;
    CGLayout* layout;
    QString type;
};

#endif // CGCREATELAYOUT_H
