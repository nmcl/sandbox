#include <InterViews/event.h>
#include <InterViews/sensor.h>
#include <stdio.h>

#include "responder.h"

void Upper::Up()
{
    printf("Left mouse clicked\n");
}

implement(ActionCallback, Upper);

Responder::Responder(Glyph *m, Upper *u)
         : Listener(m, this), upper(u)
{
    sensor()->button(true, Event::left);

    up = new ActionCallback(Upper) (upper, &Upper::Up);
}
    
void Responder::event(Event &e)
{
    if (e.type() == Event::up)
    {
	up->execute();
    }
}
