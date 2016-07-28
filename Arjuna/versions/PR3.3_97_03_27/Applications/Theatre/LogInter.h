/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogInter.h,v 1.2 1994/10/31 12:07:19 nsmw Exp $
 */

#ifndef LOGINTER_H_
#define LOGINTER_H_

#include <IV-2_6/InterViews/box.h>
#include <IV-2_6/InterViews/interactor.h>
#include <IV-2_6/InterViews/viewport.h>
#include <IV-2_6/InterViews/scroller.h>

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
