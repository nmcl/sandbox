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

#include <InterViews/color.h>
#include <InterViews/box.h>
#include <InterViews/label.h>
#include <InterViews/layout.h>

#ifndef STRING_H_
#  include <System/string.h>
#endif

#include "Message.h"

/*
 * This function simply returns a glyph with a string in it.
 */

Glyph *Message (const Font *f, const Color *fg, char* buffer)
{
    Glyph* b = LayoutKit::instance()->hbox(new Label(buffer, f, fg));

    return b;
}
