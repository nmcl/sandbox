#include "basic.h"

Basic::Basic(Coord w, Coord h, 
             Coord hstretch, Coord hshrink, float halign, 
             Coord vstretch, Coord vshrink, float valign)
     : width(w), height(h), 
       hstretch(hstretch), hshrink(hshrink), halign(halign), 
       vstretch(vstretch), vshrink(vshrink), valign(valign),
       brush(new Brush(1.0))
{
    ref(brush);
}

Basic::~Basic()
{
    Resource::unref(brush);
}

void Basic::request(Requisition &requisition)
{
    Requirement rx(width, hstretch, hshrink, halign);
    Requirement ry(height, vstretch, vshrink, valign);
    
    requisition.require(Dimension_X, rx);
    requisition.require(Dimension_Y, ry);
}

void Basic::allocate(Canvas*, Allocation &a, Extension &ext)
{
    ext.xy_extents(a);
}

void Basic::draw(Canvas *canvas, Allocation &a)
{
    World *world = World::current();

    canvas->fill_rect(a.left(), a.bottom(), a.right(), a.top(),
                      world->background());

    canvas->new_path();
 
    canvas->move_to(a.left(), a.bottom());
    canvas->line_to(a.left(), a.top());
    canvas->line_to(a.right(), a.top());
    canvas->line_to(a.right(), a.bottom());
    canvas->line_to(a.left(), a.bottom());
    canvas->stroke(world->foreground(), brush);

    canvas->line_to(a.right(), a.top());
    canvas->stroke(world->foreground(), brush);

    canvas->move_to(a.right(), a.bottom());
    canvas->line_to(a.left(), a.top());
    canvas->stroke(world->foreground(), brush);

    canvas->close_path();
}
