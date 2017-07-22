#ifndef NODEFIGURE_H_
#define NODEFIGURE_H_

#include <InterViews/glyph.h>
#include <InterViews/brush.h>
#include <InterViews/geometry.h>
#include <InterViews/canvas.h>

class NodeFigure : public Glyph
{
public:
    NodeFigure (char*, Coord, Coord);
    virtual ~NodeFigure ();

    virtual void request (Requisition&) const;
    virtual void allocate (Canvas*, const Allocation&, Extension&);
    virtual void draw (Canvas*, const Allocation&) const;

private:
    char* _name;
    Coord _height, _width;
    Brush* _brush;
};

#endif
