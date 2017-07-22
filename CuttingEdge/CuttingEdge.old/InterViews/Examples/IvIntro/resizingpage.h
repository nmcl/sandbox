#ifndef _resizingpage_
#define _resizingpage_

#include "normalpage.h"

class ResizingPageInfo
{
public:
    ResizingPageInfo();

    boolean isResizable;
};

class ResizingPage : public NormalPage
{
public:
    ResizingPage(Glyph*);

    void allocate(Canvas*, Allocation&, Extension&);
    virtual void appendResizable(Glyph*);
    virtual void prependResizable(Glyph*);
    virtual void insertResizable(int, Glyph*);
    virtual void removeResizable(int);

    void append(Glyph*);
    void prepend(Glyph*);
    void insert(int, Glyph*);
    void remove(int);
    
    boolean IsResizable(int);

protected:
    class ResizingPageInfo_List *resizables;
};

#endif
