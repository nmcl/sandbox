#include <stdio.h>

#include "timer.h"

void Ticker::DoTick(long, long)
{
    printf("Tick\n");
};

implement(IOCallback, Ticker);
implement(IOCallback, Tocker);

Tocker::Tocker(Glyph *g, Ticker *ticker) : LeftMouser(g), ticker(ticker)
{
    tock = new IOCallback(Tocker) (this, &Tocker::DoTock);
    tick = new IOCallback(Ticker) (ticker, &Ticker::DoTick);
}
    
void Tocker::doit(Event&)
{
    Dispatcher &dispatcher = Dispatcher::instance();

    dispatcher.startTimer(1L, 0L, tick);
    dispatcher.startTimer(1L, 500000L, tock);
}

void Tocker::DoTock(long, long)
{
    printf("Tock\n");
}    
