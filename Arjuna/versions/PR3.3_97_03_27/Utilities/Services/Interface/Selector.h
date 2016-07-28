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

#include <InterViews/input.h>
#include <InterViews/glyph.h>
#include <InterViews/window.h>
#include <InterViews/patch.h>
#include <InterViews/monoglyph.h>

#include "Interface_stub.h"

class Commit;
class ScrollingText;

/*
 * This class provides a selectable glyph. By clicking on it it will become active.
 * Because it is for use with register elements it must have a value associated with it.
 */

class Selector : public InputHandler
{
public:
    Selector (FieldEditor*, char*);
    virtual ~Selector ();

    State *my_state ();
    virtual void allocate (Canvas*, const Allocation&, Extension&);
    virtual void draw (Canvas*, const Allocation&) const;
    virtual void press (const Event&);
    
    static void retrieve (Selector *);
    static void cancel ();

private:
    FieldEditor *fld;
    char *desc;
    Commit *cmt, *cnl;
    static ApplicationWindow *pw;
    static ApplicationWindow *error;
};


#endif
