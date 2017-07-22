#include <InterViews/background.h>
#include <InterViews/label.h>
#include <InterViews/margin.h>
#include <InterViews/glyph.h>
#include <InterViews/center.h>
#include <InterViews/patch.h>
#include <InterViews/glue.h>
#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <stdio.h>

class SButton : public Listener, public Handler
{
    void (*fnc)();
public:
    SButton(Patch*, void (*call)());
    ~SButton();

    virtual void event(Event&);
};
