#include <InterViews/patch.h>
#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <stdio.h>
#include <string.h>

#define Blank TBBox(vspace, vspace, vspace, vspace, vspace, vspace, vspace, vspace, vspace, vspace)
#define Blank_Window TBBox(new Blank, new Blank, new Blank)

extern "C"
{
    void bcopy(char*, char*, int);
}

class ScrollingText : public Listener, public Handler
{
    Patch* me;
    char *text;
    char *page_top, *marker;
    int c_count, skip_lines;
    const Color *fg;
    const Font *f;
    
    char *next_string();
    void alter_text();
    void redraw_area();

public:
    ScrollingText(char*, Patch*, World*);
    virtual ~ScrollingText();

    void update_text(int, char*);
    virtual void event(Event&);
};
