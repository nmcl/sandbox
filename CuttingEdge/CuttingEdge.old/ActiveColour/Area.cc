// #include <InterViews/geometry.h>

#include "Area.h"

Area::Area(Coord h, Coord w) : Patch (nil)
{
    hight = h;
    width = w;
}

Area::Area(Glyph* body, Coord h, Coord w) : Patch(body)
{
    hight = h;
    width = w;
}

Area::~Area()
{
}

void Area::request(Requisition& req) const
{
    req.requirement(Dimension_X).natural(width);
    req.requirement(Dimension_X).shrink(0);
    req.requirement(Dimension_X).stretch(0);
    req.requirement(Dimension_Y).natural(hight);
    req.requirement(Dimension_Y).shrink(0);
    req.requirement(Dimension_Y).stretch(0);
}
