#ifndef _AREA_H_
#define _AREA_H_

#include <InterViews/patch.h>

class Area : public Patch
{
    Coord hight;
    Coord width;
public:
    Area(Coord h, Coord w);
    Area(Glyph* body, Coord h, Coord w);
    ~Area();

    virtual void request(Requisition&) const;
};

#endif  _AREA_H_











