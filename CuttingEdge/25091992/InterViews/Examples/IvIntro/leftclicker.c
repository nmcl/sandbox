#include "leftclicker.h"

#include <stdio.h>

LeftClicker::LeftClicker(Glyph *g) : LeftMouser(g) {}

void LeftClicker::doit(Event &e)
{
    printf("Left mouse clicked at %f %f\n", 
                                    e.pointer_x(), e.pointer_y());
}
