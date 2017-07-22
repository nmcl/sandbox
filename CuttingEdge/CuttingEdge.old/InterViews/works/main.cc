#include <IV-look/bevel.h>
#include <IV-look/button.h>
#include <IV-look/choice.h>
#include <InterViews/action.h>
#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>
#include <InterViews/border.h>
#include <InterViews/world.h>
#include <InterViews/margin.h>
#include <InterViews/event.h>
#include <InterViews/page.h>
#include <InterViews/label.h>
#include <InterViews/canvas.h>
#include <InterViews/handler.h>
#include <InterViews/patch.h>
#include "Selector.h"
#include "message.h"

#include <stdio.h>
#include <string.h>

static PropertyData props[] = {
  { nil }
};

static OptionDesc options[] = {
  { nil }
};

int r1 = 0, r2 = 0, r3 = 0, r4 = 0;
int stack[10], pc = 0;

int main(int argc, char **argv)
{
    World world("Test", argc, argv, options, props);
    Font *f = world.font();
    Color *fg = world.foreground();
    Color *c = world.background();
    Glyph *vspace = new VGlue(5.0);
    Glyph *hspace = new HGlue(10.0);

    Patch *reg1 = new Patch(new VCenter(Message(f, fg, "Register1", r1), 1.0));
    Patch *regs = new Patch(new Background(new LRBox(hspace, reg1, hspace), c));

    Selector *slc = new Selector(regs, r2);
    Window *w1 = new TopLevelWindow(slc);
    w1->geometry("150x80");
    w1->place(500, 500);
    slc->add(w1);
    w1->map();

    world.RingBell(0);
    world.run();
}
