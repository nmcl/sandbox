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

#ifndef INTERFACESTATE_H_
#define INTERFACESTATE_H_

#ifdef STUB
#   pragma @NoRename
#endif

const int BUF_SIZE = 200;
const int NAME = 20;
const int PATH = 200;

// @NoRemote, @NoMarshall
class State
{
public:
    State (State&);
    State ();
    ~State ();

    void Set_State (char*, char*, char*, char*, int = 0);
    void Add (State*);
    void Initialize ();

    State *prev;
    State *next;

    char interfaces[BUF_SIZE]; // description of interface
    char hostname[NAME];       // name of machine where interface resides
    char pathname[PATH];       // location of interface
    char servers[PATH];        // location of servers
    int u_id;                  // unique id of this interface (local to browser)
};

#endif
