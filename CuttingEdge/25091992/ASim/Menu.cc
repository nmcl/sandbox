#include "Menu.h"
#include "Message.h"

ActionCallbackdeclare(foo);

foo::foo(const Font* f1, const Color* c1, const Color* c2)
{
    f = f1;
    fg = c1;
    c = c2;

    Patch *p1 = new Patch(new VCenter(Message(f, fg, "Button 1"), 1.0));
    Patch *p2 = new Patch(new VCenter(Message(f, fg, "Button 2"), 1.0));

    foo_ActionCallback *a1 = new foo_ActionCallback(this, foo::button1);
    foo_ActionCallback *a2 = new foo_ActionCallback(this, foo::button2);
    Telltale          *t1 = new Telltale(p1);
    Telltale          *t2 = new Telltale(p2);

    Menu *m = new Menu(new TBBox(), 0.0, 0.0, 10.0, 10.0);
    m->add_item(t1, a1);
    m->add_item(t2, a2);

    Glyph *g1 = new Background(m, c);

    TopLevelWindow *w1 = new TopLevelWindow(g1);
    w1->geometry("150x80");
    w1->place(700, 500);
    w1->name("The Window");
    w1->map();
}

ActionCallbackimplement(foo);
