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

#include "message.h"

Selector::Selector(Patch* body, int r) : Listener(body, this)
{
    printf("In constructor\n");
    sensor()->button(true, Event::any);

    i = r;
    wind = NULL;
    me = body;
}

Selector::~Selector()
{
}

void Selector::event(Event& e)
{
    printf("In event handler\n");
    if (e.type() == Event::down)
    {
	i++;
	printf("Down event detected. Register now: %d\n",i);
	if (wind != NULL)
	{
	    printf("Unmapping\n");
	    me->remove(1);
	    Font *f = wind->world()->font();
	    Color *fg = wind->world()->foreground();
	    Patch *new_val = new Patch(new VCenter(Message(f, fg, "Register1", i), 1.0));
	    me->insert(1, new_val);

	    me->reallocate();
	    me->redraw();
	}
	if (i == 20)
	    wind->world()->quit();
    }
}

void Selector::add(Window *w)
{
    wind = w;
}
