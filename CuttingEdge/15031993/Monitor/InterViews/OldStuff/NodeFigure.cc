#ifndef NODEFIGURE_H_
#include "NodeFigure.h"
#endif

#include <strings.h>

#include <InterViews/brush.h>
#include <InterViews/canvas.h>
#include <InterViews/font.h>
#include <InterViews/color.h>
#include <InterViews/brush.h>
#include <IV-look/kit.h>

NodeFigure::NodeFigure (char* name, Coord height, Coord width)
{
    _name = name;
    _height = height;
    _width = width;

    _brush = new Brush(2.0);
    Resource::ref(_brush);
}

NodeFigure::~NodeFigure ()
{
    Resource::unref(_brush);
}

void NodeFigure::request (Requisition& req) const
{
    Coord w = _brush->width();

    Requirement rx(_width + w + w, 0, 0, 0.5);
    Requirement ry(_height + w + w, 0, 0, 0.5);
    req.require(Dimension_X, rx);
    req.require(Dimension_Y, ry);
}

void NodeFigure::allocate (Canvas* c, const Allocation& a, Extension& ext)
{
    ext.set(c, a);
}

void NodeFigure::draw (Canvas* c, const Allocation& a) const
{
    const Coord x = a.x();
    const Coord y = a.y();
    const Coord w = (a.right() - a.left()) / 2.0;
    const Coord h = (a.top() - a.bottom()) / 2.0;

    const Coord xp0 = 1.00000000 * w;
    const Coord xp1 = 0.89657547 * w;
    const Coord xp2 = 0.70710678 * w;
    const Coord xp3 = 0.51763809 * w;
    const Coord xp4 = 0.26794919 * w;

    const Coord yp0 = 1.00000000 * h;
    const Coord yp1 = 0.89657547 * h;   
    const Coord yp2 = 0.70710678 * h;   
    const Coord yp3 = 0.51763809 * h;   
    const Coord yp4 = 0.26794919 * h;  

    c->new_path();
    c->move_to (x+xp0, y);
    c->curve_to(x+xp2, y+yp2, x+xp0, y+yp4, x+xp1, y+yp3);
    c->curve_to(x,     y+yp0, x+xp3, y+yp1, x+xp4, y+yp0);
    c->curve_to(x-xp2, y+yp2, x-xp4, y+yp0, x-xp3, y+yp1);
    c->curve_to(x-xp0, y,     x-xp1, y+yp3, x-xp0, y+yp4);
    c->curve_to(x-xp2, y-yp2, x-xp0, y-yp4, x-xp1, y-yp3);
    c->curve_to(x,     y-yp0, x-xp3, y-yp1, x-xp4, y-yp0);
    c->curve_to(x+xp2, y-yp2, x+xp4, y-yp0, x+xp3, y-yp1);
    c->curve_to(x+xp0, y,     x+xp1, y-yp3, x+xp0, y-yp4);
    c->close_path();
    c->stroke(WidgetKit::instance()->foreground(), _brush);

    int   name_len   = strlen(_name);
    const Font* font = WidgetKit::instance()->font();
    Coord name_width = font->width(_name, name_len);
    Coord p          = x - (name_width / 2.0);

    for (int i = 0; i < name_len; i++)
    {
	Coord character_width = font->width(&_name[i], 1);

	c->character(font, _name[i], character_width, WidgetKit::instance()->foreground(), p, y);
	p += character_width;
    }
}
