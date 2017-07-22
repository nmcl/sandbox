#include <IV-look/kit.h>
#include <IV-look/ol_kit.h>
#include <IV-look/adjustable.h>
#include <InterViews/style.h>
#include <InterViews/patch.h>
#include <InterViews/geometry.h>
#include <InterViews/coord.h>
#include <IV-look/field.h>
#include <IV-look/scroller.h>

#include <stdio.h>

class S_Text : public Adjustable, public Patch
{
    Coord _lower, _upper, _span, _curvalue;
//    Patch *me;
public:
    S_Text(Coord, Coord, World*, Patch*);
    ~S_Text();
    virtual Coord lower(DimensionName) const;
    virtual Coord upper(DimensionName) const;
    virtual Coord length(DimensionName) const;
    virtual Coord cur_lower(DimensionName) const;
    virtual Coord cur_upper(DimensionName) const;
    virtual Coord cur_length(DimensionName) const;
//    virtual void scroll_to(DimensionName, Coord position);
};

S_Text::S_Text(Coord lower, Coord upper, World* wd, Patch* pt) : Patch(pt)
{
    _lower = lower;
    _upper = upper;
    _span = _upper - _lower;
    _curvalue = (_lower + _lower)/2;

    const Font *f = wd->font();
    const Color *fg = wd->foreground();
}

S_Text::~S_Text()
{
}

Coord S_Text::lower(DimensionName) const {return _lower;}

Coord S_Text::upper(DimensionName) const {return _lower + _span;}

Coord S_Text::length(DimensionName) const {return _span;}

Coord S_Text::cur_lower(DimensionName dm) const
{
    return _curvalue - cur_length(dm)/2;
}

Coord S_Text::cur_upper(DimensionName dm) const
{
    return cur_lower(dm) + cur_length(dm);
}

Coord S_Text::cur_length(DimensionName) const {return 10.0;}
/*
void S_Text::scroll_to(DimensionName dm, Coord position)
{
    Coord p = position;

    constrain(dm, p);
    if (p != _curvalue)
    {
	_curvalue = p;
	notify();
    }
}
*/
