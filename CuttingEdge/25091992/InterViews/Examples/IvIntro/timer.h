#ifndef _timer_
#define _timer_

#include <InterViews/handler.h>
#include <InterViews/listener.h>
#include <InterViews/event.h>
#include <InterViews/sensor.h>
#include <Dispatch/dispatcher.h>
#include <Dispatch/iohandler.h>
#include <Dispatch/iocallback.h>

#include "leftmouser.h"

class Ticker
{
public:
    void DoTick(long, long);
};

class Tocker;
declare(IOCallback, Ticker)
declare(IOCallback, Tocker)

class Tocker : public LeftMouser
{
public:
    Tocker(Glyph*, Ticker*);

    void doit(Event&);
    void DoTock(long, long);
private:
    IOCallback(Ticker) *tick;
    IOCallback(Tocker) *tock;
    Ticker *ticker;
};

#endif
