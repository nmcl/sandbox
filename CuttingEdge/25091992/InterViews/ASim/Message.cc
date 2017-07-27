#include <InterViews/margin.h>
#include <InterViews/color.h>
#include <InterViews/world.h>
#include <InterViews/box.h>
#include <InterViews/label.h>
#include <stdio.h>
#include <string.h>

#include "Message.h"

Glyph *VMessage(const Font *f, const Color *fg, char* buffer, int r)
{
    char *s;
    char *add = " = %d";
    int length;

    length = strlen(buffer)+strlen(add);
    s = (char*) malloc(length+sizeof(r)+1);
    sprintf(s, buffer);
    sprintf(&s[strlen(buffer)], add, r);

    Box *b = new LRBox;
    b->append(new RMargin(new Label(s, f, fg), 0.0, fil, 0.0));

    return b;
}

Glyph *Message(const Font *f, const Color *fg, char* buffer)
{
    Box *b = new LRBox;
    b->append(new RMargin(new Label(buffer, f, fg), 0.0, fil, 0.0));

    return b;
}
