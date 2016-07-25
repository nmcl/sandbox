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

#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <InterViews/listener.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/sensor.h>
#include <InterViews/window.h>
#include <InterViews/patch.h>
#include <InterViews/world.h>
#include <InterViews/action.h>
#include <IV-look/field.h>

#include "Interface_stub.h"

class Commit;
class ScrollingText;

/* This class provides a selectable glyph. By clicking on it it will become active.
 * Because it is for use with register elements it must have a value associated with it.
 */

class Selector : public Handler, public Listener
{
public:
    Selector (Patch *body, FieldEditor*, char*, ScrollingText*);
    virtual ~Selector ();

    State *my_state ();
    virtual void event (Event&);
    static void retrieve (Selector *);
    static void cancel ();

private:
    Patch *me;        // The patch which displays the information
    FieldEditor *fld;
    char *desc;
    static class ScrollingText *w;
    Commit *cmt, *cnl;
    static class PopupWindow *pw;
    static PopupWindow *error;
};

#ifdef INIT_
#undef INIT_
    class PopupWindow *Selector::pw = 0;
    class ScrollingText *Selector::w = 0;
    class PopupWindow *Selector::error = 0;
#endif
#endif
