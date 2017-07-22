#ifndef BASICACTIONINTERACTOR_H_
#define BASICACTIONINTERACTOR_H_

#include <InterViews/event.h>

#include "BasicAction.h"

class BasicActionInteractor : public BasicAction
{
public:
    BasicActionInteractor();
    virtual ~BasicActionInteractor();

    virtual void press(const Event&);
    virtual void keystroke(const Event&);
};

#endif
