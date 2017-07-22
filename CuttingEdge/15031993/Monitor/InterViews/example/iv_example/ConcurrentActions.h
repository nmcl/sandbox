#ifndef CONCURRENTACTIONS_H_
#define CONCURRENTACTIONS_H_

#include <InterViews/polyglyph.h>

#include "ActionStructure.h"

class ConcurrentActions : public ActionStructure
{
public:
    ConcurrentActions();
    virtual ~ConcurrentActions();

    virtual int AddAbove(ActionStructure*);
    virtual int AddBelow(ActionStructure*);

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
    PolyGlyph* vbox;
    Brush*     _brush;
};

#endif
