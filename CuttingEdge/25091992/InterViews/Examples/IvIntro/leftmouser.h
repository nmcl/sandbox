#ifndef _leftmouser_
#define _leftmouser_

#include <InterViews/listener.h>
#include <InterViews/ptrhandler.h>
#include <InterViews/geometry.h>

class LeftMouser : public Listener, public PointerHandler
{
public:
    LeftMouser(Glyph*);

    virtual void doit(Event&);

    void allocate(Canvas*, Allocation&, Extension&);
    void commit(Event&);
private:
    Allocation alloc;
};

#endif
