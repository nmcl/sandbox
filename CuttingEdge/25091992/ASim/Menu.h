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
    const Font *f;
    const Color *fg, *c;
public:
   foo(const Font*, const Color*, const Color*);
   void button1() { cout << "button 1 pressed" << endl; }
   void button2() { cout << "button 2 pressed" << endl; }
};
