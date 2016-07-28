/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TheatreDispatch.h,v 1.1 1993/11/11 15:19:01 nmcl Exp $
 */

#ifndef THEATREDISPATCH_H_
#define THEATREDISPATCH_H_

class TheatreDispatch;

#include <InterViews/2.6/InterViews/box.h>
#include <IV-look/2.6/InterViews/banner.h>

#include "MenuItemDispatch.h"
#include "LogInter.h"
#include "TheatreInter.h"

#include <Arjuna/LockMan.h>
#include <Arjuna/AtomicA.h>

#include "TheatrePersis.h"

class TheatreDispatch :public TheatreInter
{
public:
    TheatreDispatch();
    ~TheatreDispatch();

    void MenuDo(enum DispatchOper);
    void SeatDo(int);

    void Run();

private:
    int            selected[100];
    int            num_seats;
    TheatrePersis* ThPersis;

    AtomicAction*  AALevel1;
    int            inLevel1;
    int            usedLevel1[100];

    AtomicAction*  AALevel2;
    int            inLevel2;
    int            usedLevel2[100];

    int            quit;
    LogInter*      actionLog;
    int            log_open;
};

#endif
