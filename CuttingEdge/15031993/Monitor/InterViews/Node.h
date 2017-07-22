#ifndef NODE_H_
#define NODE_H_

#ifndef NODEDATA_H_
#include "NodeData.h"
#endif

#ifndef COMMON_H_
#include "Common.h"
#endif

#include <InterViews/input.h>
#include <InterViews/polyglyph.h>
#include <InterViews/window.h>
#include <InterViews/label.h>
#include <InterViews/scrbox.h>

class Node : public InputHandler
{
public:
    Node (const char*, NodeData*);
    Node (const char*, NodeData*, DependencyLevel);
    ~Node ();

    char* getNodeName () const;
    void changeNodeName (const char*);

    virtual void draw (Canvas*, const Allocation&) const;
    virtual void press (const Event&);

private:
    void displayNode (DependencyLevel, Boolean);
    
    ApplicationWindow* depWindow;
    Label* contents;
    Glyph* vbox;
    NodeData* myData;
    ScrollBox* listWindow;
    char* hostName;
    Boolean displayMethod;
};

#endif
