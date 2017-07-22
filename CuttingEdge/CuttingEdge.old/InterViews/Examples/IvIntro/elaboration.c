#include "elaboration.h"

Elaboration::Elaboration(Coord w, Coord h)
           : Basic(w, h), scale_x(0.0), scale_y(0.0) {}

void Elaboration::resize(Coord w, Coord h)
{
    if (scale_x == 0.0 && scale_y == 0.0)
    {
        scale_x = width / w;
        scale_y = height / h;
    }
    else
    {
        width = w * scale_x;
        height = h * scale_y;
    }
}
