/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef OBJECT_H_
#define OBJECT_H_

// Define an AllObjects class which encompasses Objects

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef PROTOCOL_H_
#include "Protocol.h"
#endif


const int MAX_OBJECTS = 10; // Maximum number of objects in a replica group.

class Machine;
class Client;

/* This class represents information which must be maintained for each object. */

class ObjectData
{
public:
    ObjectData ();
    ~ObjectData ();

    lock_code Lock; // Lock currently held on object
    boolean Available; // Is object available?
    Machine* location; // Location (Machine) of object
    Client* User; // ID of user of object
};


/* This class represents the objects which are used in the simulated network. */

class Object
{
public:
    Object (Protocol*, long);
    ~Object ();

    void SetReadQuorum (long);
    void SetWriteQuorum (long);
    long ReadQuorum (void);
    long WriteQuorum (void);
    long GetMinimum ();

    boolean PerformOperation (lock_code, Client*);
    boolean CheckQuorum (lock_code);

    long NumberOfObjects;
    ObjectData Replicas[MAX_OBJECTS];
    double rw_ratio;

private:
    Protocol* RepProt;
    boolean Valid (long);
    long n_write, n_read;
};

#endif
