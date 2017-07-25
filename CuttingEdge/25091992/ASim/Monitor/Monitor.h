#include <InterViews/glyph.h>
#include <InterViews/patch.h>
#include <InterViews/window.h>
#include <InterViews/handler.h>
#include <InterViews/listener.h>
#include <InterViews/world.h>
#include <InterViews/color.h>
#include <InterViews/background.h>
#include <InterViews/box.h>
#include <InterViews/bitmap.h>
#include <InterViews/stencil.h>

#include "common.h"

struct connectors
{
    boolean positions[8];
};

class Monitor : public Handler, public Listener
{
    int peg_number;
    Patch *view;
    TopLevelWindow *w;
    boolean mapped;
public:
    static connectors bus;
    static components cmp;
    Monitor(Patch*, int);
    virtual ~Monitor();

    virtual void event(Event&);
};

#ifdef _INIT_
    components Monitor::cmp = Empty;
    connectors Monitor::bus = {false, false, false, false, false, false, false, false};
#endif

