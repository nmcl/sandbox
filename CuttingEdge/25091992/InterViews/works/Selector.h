#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/sensor.h>
#include <InterViews/window.h>
#include <InterViews/patch.h>
#include <InterViews/world.h>

class Selector : public Listener, public Handler
{
    int i;
    Window *wind;
    Patch *me;
public:
    Selector(Patch *body, int r);
    virtual ~Selector();

    virtual void event(Event&);
    void add(Window*);
};
