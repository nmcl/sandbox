/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: AppointmentEntry.h,v 1.2 1993/03/18 13:50:41 nmcl Exp $
 */

#ifndef APPOINTMENTENTRY_H_
#define APPOINTMENTENTRY_H_

#include <Common/Boolean.h>
#include <System/sys/time.h>

// @NoRemote @AutoMarshall
struct AppointmentEntry
{
    time_t start;
    time_t end;
    char* description;
    Boolean confirmed;
};

#endif
