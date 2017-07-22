#ifndef BASICACTION_H_
#define BASICACTION_H_

#include <InterViews/brush.h>
#include <InterViews/polyglyph.h>

#include "BasicActionFigure.h"
#include "ActionStructure.h"

class BasicAction : public ActionStructure
{
public:
    BasicAction();
    virtual ~BasicAction();

    virtual int DoAddBefore(ActionStructure*, ActionStructure*);
    virtual int DoAddAfter(ActionStructure*, ActionStructure*);
    virtual int DoAddAbove(ActionStructure*, ActionStructure*);
    virtual int DoAddBelow(ActionStructure*, ActionStructure*);

    virtual int DoRemove(ActionStructure*);
    virtual int DoReplace(ActionStructure*, ActionStructure*);

    virtual void Change();
    virtual void DoChange(ActionStructure*);

    void setHight(Coord);
    void setWidth(Coord);
    Coord getHight();
    Coord getWidth();

    virtual void draw(Canvas* c, const Allocation& a) const;
private:
    BasicActionFigure* contents;
    Brush*             _brush;
    Glyph*             vbox;
};

#endif
