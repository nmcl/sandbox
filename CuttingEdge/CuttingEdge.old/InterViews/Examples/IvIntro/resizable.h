#ifndef _resizable_
#define _resizable_

#include <InterViews/monoglyph.h>

class Resizable : public MonoGlyph
{
public:
    virtual void resize(Coord, Coord);
protected:
    Resizable();
    Resizable(Glyph*);
};

#endif
