#ifndef DEPLIST_H_
#define DEPLIST_H_

#ifndef NODEDATA_H_
#include "NodeData.h"
#endif

#include <InterViews/input.h>
#include <InterViews/polyglyph.h>
#include <InterViews/window.h>
#include <InterViews/label.h>

class DepList : public InputHandler
{
public:
    DepList (NodeData*, const char*);
    ~DepList ();
    
    virtual void draw (Canvas*, const Allocation&) const;
    virtual void press (const Event&);
    
private:
    char hostName[MAXHOSTNAMELEN];
    Glyph* vbox;
    Label* contents;
    ApplicationWindow *aw, *tw;
    NodeData* groupData;
};

#endif
