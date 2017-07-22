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
#include <InterViews/box.h>
#include <InterViews/color.h>

#include <string.h>
#include <stream.h>
#include <stdio.h>

#include "Message.h"

Selector::Selector(Glyph* body, char* n, components *p) : Listener(body, this)
{
    lcn = p;
    name = n;
    button(true, Event::any);  // register event handler
}

Selector::~Selector()
{
    // do nothing
}

void Selector::event(Event& e)
{
    if (e.type() == Event::down)  // catch relevant event and process it
    {
	if (!strcmp(name, "Processor"))
	    *lcn = Processor;
	if (!strcmp(name, "Memory"))
	    *lcn = Memory;
	if (!strcmp(name, "Monitor"))
	    *lcn = BusMonitor;
	if (!strcmp(name, "Other"))
	    *lcn = Other;
	if (!strcmp(name, "DELETE"))
	    *lcn = DELETE;
    }
}


