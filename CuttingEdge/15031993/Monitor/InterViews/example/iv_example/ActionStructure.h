#ifndef ACTIONSTRUCTURE_H_
#define ACTIONSTRUCTURE_H_

#include <InterViews/input.h>

class ActionStructure : public InputHandler
{
public:
    ActionStructure();
    virtual ~ActionStructure();

    virtual int AddBefore(ActionStructure*);
    virtual int AddAfter(ActionStructure*);
    virtual int AddAbove(ActionStructure*);
    virtual int AddBelow(ActionStructure*);

    virtual int Remove();

    virtual int DoAddBefore(ActionStructure*, ActionStructure*) = 0;
    virtual int DoAddAfter(ActionStructure*, ActionStructure*) = 0;
    virtual int DoAddAbove(ActionStructure*, ActionStructure*) = 0;
    virtual int DoAddBelow(ActionStructure*, ActionStructure*) = 0;

    virtual int DoRemove(ActionStructure*) = 0;
    virtual int DoReplace(ActionStructure*, ActionStructure*) = 0;

    virtual void Change();
    virtual void DoChange(ActionStructure*);
protected:
    ActionStructure* parentStructure;
public:
    void setParent(ActionStructure*);
};

#endif
