/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogInter.h,v 1.1 1993/11/11 15:18:34 nmcl Exp $
 */

#ifndef LOGINTER_H_
#define LOGINTER_H_

#include <InterViews/2.6/InterViews/box.h>
#include <InterViews/2.6/InterViews/interactor.h>
#include <InterViews/2.6/InterViews/viewport.h>
#include <IV-look/2.6/InterViews/scroller.h>
#include <InterViews/2.6/_enter.h>

#define historySize 48

extern const int unitSize;

class LogInter :public VBox
{
public:
    LogInter();
    ~LogInter();
    
    void AddEvent(Interactor*, int, int, int, int);
private:
    int withMark;

    int circle_queue_end;
    Interactor* circle_queue[historySize];
    HBox*       Log;
    Viewport*   view;
    HScroller*  scroller;
};

#endif
