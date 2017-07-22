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

extern "C" {
    char *malloc(int);
}

class SButton : public Listener, public Handler
{
    void (*fnc)();
public:
    SButton(Patch*, void (*call)());
    ~SButton();

    virtual void event(Event&);
};

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
