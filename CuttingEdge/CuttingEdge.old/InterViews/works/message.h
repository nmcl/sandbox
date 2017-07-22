#include <stdio.h>
#include <string.h>
#include <InterViews/box.h>

extern "C" {
    char *malloc(int);
}

static Glyph *Message(Font *f, Color *fg, char* buffer, int r)
{
    char *s, *s2;
    char *add = " = %d";

    s = (char*) malloc(strlen(buffer)+strlen(add)+sizeof(r)+1);
    s2 = (char*) malloc(strlen(buffer)+strlen(add));

    strncpy(s2, buffer, strlen(buffer));
    strncpy(s2+strlen(buffer), add, strlen(add));
    sprintf(s, s2, r);
    s[strlen(s)] = '\0';

    Box *b = new LRBox;
    b->append(new RMargin(new Label(s, f, fg), 0.0, fil, 0.0));

    return b;
}
