#include "SButton.h"

SButton::SButton(Patch* body, void (*call)()) : Listener(body, this)
{
    button(true, Event::any);

    fnc = call;
}

SButton::~SButton()
{
}

void SButton::event(Event& e)
{
    if (e.type() == Event::down)
	fnc();
}
