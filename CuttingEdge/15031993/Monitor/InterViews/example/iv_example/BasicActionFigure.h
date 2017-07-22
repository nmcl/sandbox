#ifndef BASICACTIONFIGURE_H_
#define BASICACTIONFIGURE_H_

#include <InterViews/glyph.h>
#include <InterViews/brush.h>

class BasicActionFigure : public Glyph {
public:
    BasicActionFigure(char* name, Coord hight, Coord width);
    virtual ~BasicActionFigure();

    virtual void request(Requisition&) const;
    virtual void allocate(Canvas*, const Allocation&, Extension&);
    virtual void draw(Canvas*, const Allocation&) const;

    void setHight(Coord);
    void setWidth(Coord);
    Coord getHight();
    Coord getWidth();
private:
    Coord  _hight, _width;
    char*  _name;
    Brush* _brush;
};

#endif
