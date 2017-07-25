#ifndef _normalpage_
#define _normalpage_

#include <InterViews/page.h>

class NormalPageInfo
{
public:
    NormalPageInfo();

    float x_;
    float y_;
};

class NormalPage : public Page
{
public:
    NormalPage::NormalPage(Glyph*);
    
    void allocate(Canvas*, Allocation&, Extension&);
    void append(Glyph*);
    void prepend(Glyph*);
    void insert(int, Glyph*);
    void remove(int);
    void location(int, float&, float&);
    void move(int, float, float);
protected:
    class NormalPageInfo_List* info_;
};

#endif
