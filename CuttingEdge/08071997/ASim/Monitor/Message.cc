#include <InterViews/margin.h>
#include <InterViews/color.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/label.h>
#include <stdio.h>
#include <string.h>

#include "Message.h"

/* This function simply returns a glyph with a string in it.
 */

Glyph *Message(const Font *f, const Color *fg, char* buffer)
{
    Box *b = new LRBox;
    b->append(new RMargin(new Label(buffer, f, fg), 0.0, fil, 0.0));

    return b;
}
