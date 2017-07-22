#ifndef _basic_
#define _basic_

#include <InterViews/world.h>
#include <InterViews/glyph.h>
#include <InterViews/geometry.h>
#include <InterViews/canvas.h>
#include <InterViews/brush.h>

class Basic : public Glyph
{
public:
    Basic(Coord, Coord, 
          Coord = fil, Coord = fil, float = 0.0,
          Coord = fil, Coord = fil, float = 0.0);
    ~Basic();

    virtual void request(Requisition&);
    virtual void allocate(Canvas*, Allocation&, Extension&);
    virtual void draw(Canvas*, Allocation&);
protected:
    Brush *brush;
    Coord width, height;
    Coord hstretch, hshrink, halign, vstretch, vshrink, valign;
};

#endif
