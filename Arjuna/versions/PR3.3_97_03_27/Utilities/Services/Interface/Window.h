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

#include <InterViews/adjust.h>
#include <InterViews/patch.h>
#include <InterViews/handler.h>
#include <InterViews/glyph.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/box.h>
#include <IV-look/field.h>

#ifndef STRING_H_
#  include <System/string.h>
#endif

#include "Selector.h"

/*
 * This is a simple scrolling text window.
 */

class ScrollingText
{
public:
    ScrollingText (char*, FieldEditor*);
    virtual ~ScrollingText ();

    void change_text (char*);

private:
    void convertText ();

    char* text;
    FieldEditor* fld;
};

#endif
