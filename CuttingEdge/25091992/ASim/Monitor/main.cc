#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>
#include <InterViews/border.h>
#include <InterViews/world.h>
#include <InterViews/margin.h>
#include <InterViews/color.h>
#include <InterViews/label.h>
#include <InterViews/handler.h>
#include <InterViews/patch.h>
#include <InterViews/fixedspan.h>
#include <InterViews/session.h>
#include <InterViews/bitmap.h>
#include <InterViews/stencil.h>
#include <IV-look/field.h>

#include <stdio.h>
#include <iostream.h>

#include "common.h"
#include "Message.h"
#include "Selector.h"
#include "Monitor.h"

#define RECTANGLE(w,h,a) \
(new HCenter(new Border(new FixedSpan(new FixedSpan(nil, Dimension_Y, h), \
                                      Dimension_X, w), \
                        world.foreground()), a))

void main(int argc, char **argv)
{
    World world("Test", argc, argv, 0, 0);
    const Font *f = world.font();
    const Color *fg = world.foreground();
    const Color *c = world.background();
    Glyph *vspace = new VGlue(10.0);
    Glyph *hspace = new HGlue(10.0);

// Create bus representation.

    Bitmap *bm = Bitmap::open("blank");
    Stencil *pict = new Stencil(bm, fg);

    Monitor *peg1 = new Monitor(new Patch(Message(f, fg, "|")), 0);
    Monitor *peg2 = new Monitor(new Patch(Message(f, fg, "|")), 1);
    Monitor *peg3 = new Monitor(new Patch(Message(f, fg, "|")), 2);
    Monitor *peg4 = new Monitor(new Patch(Message(f, fg, "|")), 3);
    Monitor *peg5 = new Monitor(new Patch(Message(f, fg, "|")), 4);
    Monitor *peg6 = new Monitor(new Patch(Message(f, fg, "|")), 5);
    Monitor *peg7 = new Monitor(new Patch(Message(f, fg, "|")), 6);
    Monitor *peg8 = new Monitor(new Patch(Message(f, fg, "|")), 7);
    LRBox *top_pos = new LRBox(peg1, peg2, peg3, peg4, peg5, peg6, peg7, peg8);

    TBBox *view = new TBBox(new HCenter(top_pos), RECTANGLE(300.0, 20.0, 0.5));

// Create icon selector.

    Session *s = world.session();

    Selector *txt1 = new Selector(Message(f, fg, "Processor"), "Processor", &Monitor::cmp);
    Selector *txt2 = new Selector(Message(f, fg, "Memory"), "Memory", &Monitor::cmp);
    Selector *txt3 = new Selector(Message(f, fg, "Monitor"), "Monitor", &Monitor::cmp);
    Selector *txt4 = new Selector(Message(f, fg, "Other"), "Other", &Monitor::cmp);
    Selector *txt5 = new Selector(Message(f, fg, "DELETE"), "DELETE", &Monitor::cmp);
    LRBox *slc = new LRBox(txt1, hspace, txt2, hspace, txt3, hspace, txt4, hspace, hspace, txt5);
    HCenter *cn = new HCenter(slc);
    Border *bdr = new Border(new Margin(cn, 0.5), fg);
    
    TBBox *disp = new TBBox(bdr, vspace, vspace, vspace, vspace, view);
    Background *bg1 = new Background(disp, c);
    TopLevelWindow *w1 = new TopLevelWindow(bg1);
    w1->geometry("350x150");
    w1->place(500, 500);
    w1->name("Bus Monitor");
    w1->map();

    world.run();
}

