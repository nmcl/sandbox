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
    button(true, Event::any);  // register event handler

    i = r;     // initial value of register to be displayed
    name = nm; // string name of register (needed for redisplaying register value)
    me = body; // actual glyph that comprises the initial register name and value
    ft = f;    // font for displaying in
    cl = c;    // color for displaying in
}

Selector::~Selector()
{
    // do nothing
}

void Selector::set_value(int set)
{
    i = set;   // set register to given value
}

void Selector::increment_value(int increment)
{
    i = i + increment;  // increment register value by given amount
}

void Selector::display()
{
    // create new patch with register name and new value for updating of screen
    Patch *msg = new Patch(new VCenter(VMessage(ft, cl, name, i), 1.0));
    me->replace(0, msg);  // replace old register patch with new patch
    
    me->reallocate();    // redraw patch so that update is seen
    me->redraw();
}

void Selector::event(Event& e)
{
    if (e.type() == Event::down)  // catch relevant event and process it
    {
	increment_value(1);
	display();
    }
}





