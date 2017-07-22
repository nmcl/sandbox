#ifndef _adaptable_
#define _adaptable_

#include "resizable.h"

class Adaptable : public Resizable
{
public:
    Adaptable(Coord, Coord);

    void resize(Coord, Coord);
};

#endif
