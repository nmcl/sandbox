#include "Selector.h"
#include <InterViews/window.h>
#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/label.h>
#include <InterViews/margin.h>
#include <InterViews/glyph.h>
#include <InterViews/center.h>
#include <InterViews/patch.h>
#include <InterViews/glue.h>
#include <stdio.h>

#include "Message.h"

Selector::Selector(Patch* body, int r, char *nm, const Font *f, const Color *c) : Listener(body, this)
{
    button(true, Event::any);

    i = r;
    name = nm;
    me = body;
    ft = f;
    cl = c;
}

Selector::~Selector()
{
}

void Selector::set_value(int set)
{
    i = set;
}

void Selector::increment_value(int increment)
{
    i = i + increment;
}

void Selector::display()
{
    me->remove(0);
    Patch *msg = new Patch(new VCenter(VMessage(ft, cl, name, i), 1.0));
    me->insert(0, msg);
    
    me->reallocate();
    me->redraw();
}

void Selector::event(Event& e)
{
    if (e.type() == Event::down)
    {
	increment_value(1);
	display();
    }
}


