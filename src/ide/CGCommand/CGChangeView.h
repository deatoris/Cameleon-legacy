#ifndef CGCHANGEVIEW_H
#define CGCHANGEVIEW_H
#include "../CGCommandManager.h"
class CGCompositionModel;
class CGLayout;

class CGChangeView : public CGCommand
{
public:
    CGChangeView();
    virtual void exec();
    virtual void unexec();

    void setCompositionModel(CGCompositionModel* compositionModel);
    void setLayout(CGLayout* layout);
private:
    CGCompositionModel* compositionModel;
    CGCompositionModel* previousModel;
    CGLayout* layout;
    CGLayout* previousLayout;
};

#endif // CGCHANGEVIEW_H
