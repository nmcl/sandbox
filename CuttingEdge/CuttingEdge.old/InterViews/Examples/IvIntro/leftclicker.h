#ifndef _leftclicker_
#define _leftclicker_

#include "leftmouser.h"

class LeftClicker : public LeftMouser
{
public:
    LeftClicker(Glyph*);

    void doit(Event&);
};

#endif
