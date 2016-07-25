/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef INTERFACESTATE_H_
#include "InterfaceState.h"
#endif

State::State (State &ptr)
{
    Initialize();

    memcpy(interfaces, ptr.interfaces, strlen(ptr.interfaces));
    memcpy(hostname, ptr.hostname, strlen(ptr.hostname));
    memcpy(pathname, ptr.pathname, strlen(ptr.pathname));
    memcpy(servers, ptr.servers, strlen(ptr.servers));
}

State::State () { Initialize(); }

State::~State () {}

void State::Add (State *a)
{
    next = a;
    next->prev = this;
}
	
void State::Initialize ()
{
    prev = 0;
    next = 0;

    memset(interfaces, '\0', BUF_SIZE);
    memset(hostname, '\0', NAME);
    memset(pathname, '\0', PATH);
    memset(servers, '\0', PATH);
    u_id = 0;
}

void State::Set_State (char *what, char *name, char *where, char *lcn, int id)
{
    memcpy(interfaces, what, strlen(what));
    memcpy(hostname, name, strlen(name));
    memcpy(pathname, where, strlen(where));
    memcpy(servers, lcn, strlen(lcn));
    u_id = id;
}
