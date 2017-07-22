#ifndef SUBSELECT_H_
#define SUBSELECT_H_

#include <InterViews/action.h>
#include <InterViews/label.h>
#include <InterViews/window.h>
#include <IV-look/menu.h>


enum windowType { ADD, MODIFY, DELETE };


class Cancel : public Action
{
public:
    Cancel (windowType);
    ~Cancel ();
    
    virtual void execute ();

private:
    windowType wT;
};

#endif
