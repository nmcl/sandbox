#include <InterViews/patch.h>
#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <IV-look/adjuster.h>
#include <IV-look/adjustable.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

/* This is a simple scrolling text window.
 */

class ScrollingText : public Adjustable, public Adjuster, public Patch
{
    char *text;
    points *page_top, *page_ptrs;
    int lines;
    const Color *fg;
    const Font *f;
    Coord curvalue_;
    
    void alter_text();
    void redraw_area();
    void calc_pos(DimensionName, Coord);

public:
    ScrollingText(char*, Glyph*);
    virtual ~ScrollingText();

    virtual Coord lower(DimensionName) const;
    virtual Coord upper(DimensionName) const;
    virtual Coord length(DimensionName) const;
    virtual Coord cur_lower(DimensionName) const;
    virtual Coord cur_upper(DimensionName) const;
    virtual Coord cur_length(DimensionName) const;

    virtual void scroll_to(DimensionName, Coord position);
    virtual void scroll_forward(DimensionName);
    virtual void scroll_backward(DimensionName);

    virtual void  set_value(Coord);
    virtual Coord get_value() const;

    virtual void update(DimensionName);
    virtual void update_all();

    void add_text(char*);
};
