#ifndef _elaboration_
#define _elaboration_

#include "basic.h"

class Elaboration : public Basic
{
public:
    Elaboration(Coord, Coord);

    void resize(Coord, Coord);
protected:
    float scale_x, scale_y;
};

#endif

