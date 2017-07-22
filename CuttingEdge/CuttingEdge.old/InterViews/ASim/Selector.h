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
    char *name;
    Patch *me;
    const Font *ft;
    const Color *cl;
public:
    Selector(Patch *body, int r, char* nm, const Font*, const Color*);
    virtual ~Selector();

    virtual void event(Event&);
    void increment_value(int);
    void set_value(int);
    void display();
};
