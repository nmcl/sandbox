#include <iostream.h>
#include <InterViews/action.h>
#include <InterViews/background.h>
#include <InterViews/patch.h>
#include <InterViews/glyph.h>
#include <InterViews/center.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/font.h>
#include <InterViews/color.h>
#include <InterViews/label.h>
#include <InterViews/margin.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>

#include <IV-look/menu.h>
#include <IV-look/telltale.h>

class foo {
public:
   foo();
   void button1() { cout << "button 1 pressed" << endl; }
   void button2() { cout << "button 2 pressed" << endl; }
};

World *world = 0;
Glyph *Message(const Font *f, const Color *fg, char* buffer)
{
    Box *b = new LRBox;
    b->append(new RMargin(new Label(buffer, f, fg), 0.0, fil, 0.0));

    return b;
}

ActionCallbackdeclare(foo);

foo::foo()
{
    const Font *f = world->font();
    const Color *fg = world->foreground();
    const Color *c = world->background();

    Patch *p1 = new Patch(new VCenter(Message(f, fg, "Button 1"), 1.0));
    Patch *p2 = new Patch(new VCenter(Message(f, fg, "Button 2"), 1.0));

    foo_ActionCallback *a1 = new foo_ActionCallback(this, foo::button1);
    foo_ActionCallback *a2 = new foo_ActionCallback(this, foo::button2);
    Telltale          *t1 = new Telltale(p1);
    Telltale          *t2 = new Telltale(p2);

    Menu *m = new Menu(new TBBox(p1,p2), 0.0, 0.0, 10.0, 10.0);
    m->item(2, t1, a1, 0);
    m->item(3, t2, a2, 0);

    Glyph *g1 = new Background(m, c);

    TopLevelWindow *w1 = new TopLevelWindow(g1);
    w1->geometry("150x80");
    w1->place(500, 500);
    w1->name("The Window");
    w1->map();
}

ActionCallbackimplement(foo);



main(int argc, char **argv)
{
    world = new World(argv[0], argc, argv, 0, 0);
    foo f;

    world->run();
}

