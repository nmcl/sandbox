#include <stdio.h>
#include <iostream.h>

#define _INIT_
#include "Monitor.h"
#include "Message.h"

Monitor::Monitor(Patch *body, int pn) : Listener(body, this)
{
    peg_number = pn;
    view = body;
    mapped = false;
    w = 0;

    button(true, Event::any);
}

Monitor::~Monitor()
{
}

void Monitor::event(Event& e)
{
    if (e.type() == Event::down)
    {
	char *filename = 0;
	const Font *f = World::current()->font();
	const Color *fg = World::current()->foreground();
	const Color *c = World::current()->background();

	if (e.pointer_button() == Event::left)
	{
	    switch (cmp)
	    {
	    case Processor: filename = "cpu";
			    break;
	    case Memory: filename = "memory";
			    break;
	    case BusMonitor: filename = "monitor";
			    break;
	    case Other: filename = "other";
			    break;
	    case Empty: break;
	    case DELETE: filename = "blank";
			    break;
	    }

	    if ((cmp == DELETE) || ((cmp != DELETE) && (!bus.positions[peg_number])))
	    {
		Bitmap *bm = Bitmap::open(filename);
		Stencil *pict = new Stencil(bm, World::current()->foreground());
		Patch *p = new Patch(pict);

		view->replace(0, p);
		view->reallocate();
		view->redraw();
	    }

	    if ((cmp != DELETE) && (!bus.positions[peg_number]))
		bus.positions[peg_number] = true;

	    if (cmp == DELETE)
		bus.positions[peg_number] = false;
	}

	if (e.pointer_button() == Event::right)
	{
	    if (mapped)
	    {
		mapped = false;
		w->unmap();
	        delete(w);
	    }
	    else if (bus.positions[peg_number])
	    {
		mapped = true;
		
		LRBox *d = new LRBox(Message(f, fg, "Testing"));
		Background *bg = new Background(d, c);
		w = new TopLevelWindow(bg);
		w->name("Work Sheet");
		w->geometry("50x50");
		w->map();
	    }
	}
    }
}
