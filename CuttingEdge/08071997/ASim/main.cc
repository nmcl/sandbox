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
#include <InterViews/style.h>
#include <InterViews/session.h>
#include <IV-look/button.h>
#include <IV-look/kit.h>
#include <IV-look/mono_kit.h>

#include <stdio.h>
#include <string.h>

#include "Window.h"
#include "Selector.h"
#include "SButton.h"
#include "Message.h"
#include "Menu.h"

static PropertyData props[] = {
  { nil }
};

static OptionDesc options[] = {
  { nil }
};

int r1 = 0, r2 = 0, r3 = 0, r4 = 0;
int stack[5], pc = 0;

void test1()
{
    printf("step\n");
}

void test2()
{
    printf("run\n");
}

void test3()
{
    printf("quit\n");
}

int main(int argc, char **argv)
{
    World world("Test", argc, argv, options, props);
    const Font *f = world.font();
    const Color *fg = world.foreground();
    const Color *c = world.background();
    Glyph *vspace = new VGlue(5.0);
    Glyph *hspace = new HGlue(10.0);

// Need to set up the initial Patches for displaying the initial register values.

    Patch *msg1 = new Patch(new VCenter(VMessage(f, fg, "Register1", r1), 1.0));
    Patch *msg2 = new Patch(new VCenter(VMessage(f, fg, "Register2", r2), 1.0));
    Patch *msg3 = new Patch(new VCenter(VMessage(f, fg, "Register3", r3), 1.0));
    Patch *msg4 = new Patch(new VCenter(VMessage(f, fg, "Register4", r4), 1.0));
    Patch *prgc = new Patch(new VCenter(VMessage(f, fg, "Program Counter", pc), 1.0));

// Now need to indicate that they are selectable.

    Selector *slr1 = new Selector(msg1, r1, "Register1", f, fg);
    Selector *slr2 = new Selector(msg2, r2, "Register2", f, fg);
    Selector *slr3 = new Selector(msg3, r2, "Register3", f, fg);
    Selector *slr4 = new Selector(msg4, r2, "Register4", f, fg);
    Selector *slpc = new Selector(prgc, pc, "Program Counter", f, fg);

// Do the same for the stack.

    Patch *stk0 = new Patch(new VCenter(VMessage(f, fg, "Stack Element0", stack[0]), 1.0));
    Patch *stk1 = new Patch(new VCenter(VMessage(f, fg, "Stack Element1", stack[1]), 1.0));
    Patch *stk2 = new Patch(new VCenter(VMessage(f, fg, "Stack Element2", stack[2]), 1.0));
    Patch *stk3 = new Patch(new VCenter(VMessage(f, fg, "Stack Element3", stack[3]), 1.0));
    Patch *stk4 = new Patch(new VCenter(VMessage(f, fg, "Stack Element4", stack[4]), 1.0));
    Selector *sk0 = new Selector(stk0, stack[0], "Stack Element0", f, fg);
    Selector *sk1 = new Selector(stk1, stack[1], "Stack Element1", f, fg);
    Selector *sk2 = new Selector(stk2, stack[2], "Stack Element2", f, fg);
    Selector *sk3 = new Selector(stk3, stack[3], "Stack Element3", f, fg);
    Selector *sk4 = new Selector(stk4, stack[4], "Stack Element4", f, fg);

    Glyph *wd1 = new Background(new TBBox(vspace, slr1, slr2, slr3, slr4, vspace, slpc), c);
    Glyph *wd2 = new Background(new TBBox(vspace, sk0, sk1, sk2, sk3, sk4, vspace), c);

    TopLevelWindow *w1 = new TopLevelWindow(wd1);
    w1->geometry("150x80");
    w1->place(500, 500);
    w1->name("Registers");
//    w1->map();

    TopLevelWindow *w2 = new TopLevelWindow(wd2);
    w2->geometry("150x80");
    w2->place(347, 500);
    w2->name("Stack");
//    w2->map();

    FILE *handle;
    handle = fopen("data", "r");
    char *message = malloc(10240), point;
    int j = 0;

    do
    {
	point = (char)fgetc(handle);
	*(message+j) = point;
	j++;
    } while ((point != '\0') && (point != EOF));
    fclose(handle);

    TBBox *bx = new TBBox;
    for (int l = 0; l < LINES; l++)
    {
	Patch *p = new Blank_Line;
	bx->insert(l, p);
    }
    message = blank_space;
    Background *gb = new Background(bx, c);
    ScrollingText *stx = new ScrollingText(message, gb);

    Session *s = world.session();

//    Button *moverUp = Kit::instance()->up_mover(stx, s->style());
//    Button *moverDown = Kit::instance()->down_mover(stx, s->style());
    Glyph *scrollBarText = Kit::instance()->vscroll_bar(stx, s->style());

    LRBox *box = new LRBox(stx, scrollBarText);
//    LRBox *box = new LRBox(stx, new TBBox(moverUp, scrollBarText, moverDown));
    TopLevelWindow *w3 = new TopLevelWindow(box);

    w3->geometry("500x200");
    w3->place(400, 100);
    w3->name("Scrolling Text");
    w3->map();

//    foo x(f, fg, c);

    world.RingBell(0);
    world.run();
}

