/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DaemonList.h,v
 */


#ifndef DAEMONLIST_H_
#define DAEMONLIST_H_

class DaemonData;
class Members;

/*
 * Will maintain list of daemons which have been created in lifetime of process - essentially
 * maintains a cache of state daemons.
 */

// @NoRemote, @NoMarshall
class DaemonList
{
public:
    DaemonList  ();
    ~DaemonList ();

    Members* get    (const char*);
    void     put    (Members*, const char*);
    void     remove (const char*);

private:
    DaemonList* next;
    DaemonData* data;
};

#endif
