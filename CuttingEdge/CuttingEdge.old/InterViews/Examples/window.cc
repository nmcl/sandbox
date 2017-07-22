#include <InterViews/action.h>
#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/glue.h>
#include <InterViews/color.h>
#include <InterViews/border.h>
#include <InterViews/margin.h>
#include <InterViews/window.h>
#include <InterViews/world.h>
#include <InterViews/label.h>
#include <stdio.h>
#include <string.h>

extern "C" {
    char *malloc(int);
}

static PropertyData props[] = {
  { nil }
};

static OptionDesc options[] = {
  { nil }
};

static Glyph *Message(Font*, Color*, char*);

int main(int argc, char **argv)
{
    World world("Test", argc, argv, options, props);
    Font *f = world.font();
    Color *fg = world.foreground();
    Color_Intensity r, g, b;
    world.background()->intensities(&world, r, g, b);
    Color *c = new Color(Color_Intensity(0.8*r), Color_Intensity(0.8*g), Color_Intensity(0.8*b));
    Glyph *vspace = new VGlue(5.0);
    Glyph *hspace = new HGlue(10.0);
    Glyph *wind = new Background(new LRBox(new VCenter(vspace, 1.0),
					   new LRBox(hspace,
						     new TBBox(new VCenter(Message(f, fg, "This is an example of a window"),
									   1.0),
							       hspace,
							       new VCenter(Message(f, fg, "using InterViews"),
									   1.0),
							       hspace
							       ), hspace
						     ), vspace
					   ), c
				 );


    Window *w1 = new TopLevelWindow(wind);
    w1->geometry("200x100");
    w1->map();

    world.RingBell(0);
    world.run();
}

static Glyph *Message(Font *f, Color *fg, char* buffer)
{
    Box *b = new LRBox;
    b->append(new RMargin(new Label(buffer, f, fg), 0.0, fil, 0.0));

    return b;
}
