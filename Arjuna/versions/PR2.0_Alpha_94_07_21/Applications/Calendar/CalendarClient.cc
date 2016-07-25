/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: CalendarClient.cc,v 1.1 1993/11/03 12:22:36 nmcl Exp $
 */

#include <Configure.h>

#include <OS/enter-scope.h>
#include <InterViews/session.h>
#include <OS/leave-scope.h>

#include "Control.h"

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    Session *session = new Session("Calendar", argc, argv);

    CalControl calendar_controller(session);

    return 0;
}
