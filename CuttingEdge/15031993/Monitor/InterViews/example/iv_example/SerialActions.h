#ifndef SERIALACTIONS_H_
#define SERIALACTIONS_H_

#include <InterViews/brush.h>
#include <InterViews/polyglyph.h>

#include "ActionStructure.h"

class SerialActions : public ActionStructure
{
public:
    SerialActions();
    virtual ~SerialActions();

    virtual int AddBefore(ActionStructure*);
    virtual int AddAfter(ActionStructure*);

    virtual int DoAddBefore(ActionStructure*, ActionStructure*);
    virtual int DoAddAfter(ActionStructure*, ActionStructure*);
    virtual int DoAddAbove(ActionStructure*, ActionStructure*);
    virtual int DoAddBelow(ActionStructure*, ActionStructure*);

    virtual int DoRemove(ActionStructure*);
    virtual int DoReplace(ActionStructure*, ActionStructure*);

    virtual void Change();
    virtual void DoChange(ActionStructure*);

    virtual void draw(Canvas* c, const Allocation& a) const;
private:
    PolyGlyph* hbox;
    Brush*     _brush;
};

#endif
