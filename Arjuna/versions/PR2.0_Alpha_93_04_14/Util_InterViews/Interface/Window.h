/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#ifndef WINDOW_H_
#define WINDOW_H_

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
#include <IV-look/field.h>

#ifndef STRING_H_
#include <System/string.h>
#endif

#include "Selector.h"
#include "Common.h"

/* This is a simple scrolling text window.
 */

class ScrollingText : public Adjustable, public Adjuster, public Patch
{
public:
    ScrollingText (char*, Glyph*, FieldEditor*);
    virtual ~ScrollingText ();

    virtual Coord lower (DimensionName) const;
    virtual Coord upper (DimensionName) const;
    virtual Coord length (DimensionName) const;
    virtual Coord cur_lower (DimensionName) const;
    virtual Coord cur_upper (DimensionName) const;
    virtual Coord cur_length (DimensionName) const;

    virtual void scroll_to (DimensionName, Coord position);
    virtual void scroll_forward (DimensionName);
    virtual void scroll_backward (DimensionName);

    virtual void  set_value (Coord);
    virtual Coord get_value () const;

    virtual void update (DimensionName);
    virtual void update_all ();

    void redraw_area ();
    void add_text (char*);
    void change_text (char*);

private:
    char *text;
    FieldEditor *fld;
    textpoints *page_top, *page_ptrs;
    int lines;
    const Color *fg;
    const Font *f;
    Coord curvalue_;
    
    void alter_text ();
    void calc_pos (DimensionName, Coord);
};

#endif
