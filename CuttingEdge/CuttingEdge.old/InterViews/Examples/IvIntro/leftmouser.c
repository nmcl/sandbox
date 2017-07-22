#include <InterViews/sensor.h>

#include <leftmouser.h>

LeftMouser::LeftMouser(Glyph *m) : Listener(m, this)
{
    sensor()->button(true, Event::left);
}
    
void LeftMouser::allocate(Canvas *c, Allocation &a, Extension &ext)
{
    Listener::allocate(c, a, ext);
    alloc = a;
}

void LeftMouser::commit(Event &e)
{
    if (e.pointer_x() >= alloc.left() && e.pointer_x() <= alloc.right() &&
	e.pointer_y() >= alloc.bottom() && e.pointer_y() <= alloc.top())
    {
	doit(e);
    }
}

void LeftMouser::doit(Event&) {}
