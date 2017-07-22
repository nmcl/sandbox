#include <InterViews/listener.h>
#include <InterViews/color.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/sensor.h>
#include <InterViews/window.h>
#include <InterViews/patch.h>
#include <InterViews/world.h>

#include "common.h"

/* This class provides a selectable glyph. By clicking on it it will become active.
 * Because it is for use with register elements it must have a value associated with it.
 */

class Selector : public Listener, public Handler
{
    components *lcn;
    char *name;
public:
    Selector(Glyph*, char*, components*);
    virtual ~Selector();

    virtual void event(Event&);
};
