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

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef PROTOCOL_H_
#  include "Protocol.h"
#endif

const int Hysterisis = 100;  // prevents continuous reconfiguration
const int MaxClients = 10;
const int MAX_OBJECTS = 10; // Maximum number of objects in a replica group.
const double INFINITE = 1000000.0; // r/w ratio when no write operations


class Machine;
class Client;

enum Opcode { failed, lockconflict, ok};

/* This class represents information which must be maintained for each object. */

class ObjectData
{
public:
    ObjectData ();
    ~ObjectData ();

    long GetLoad ();
    Boolean IsUsing (Client*);
    
    lock_code Lock; // Lock currently held on object
    Boolean Available; // Is object available?
    Machine* location; // Location (Machine) of object
    Client* User[MaxClients]; // ID of users of object
};


/* This class represents the objects which are used in the simulated network. */

class Object
{
public:
    Object (Protocol*, long, double, Boolean);
    ~Object ();

    void SetReadQuorum (long);
    void SetWriteQuorum (long);
    long ReadQuorum (void);
    long WriteQuorum (void);
    long GetMinimum ();

    Opcode PerformOperation (lock_code, Client*);
    Boolean CheckQuorum (lock_code, Client*);

    double GetReadWriteRatio ();
    Boolean LockAll ();
    void UnlockAll ();
    void RemoveObjects ();

    double GetLoad (Client*);

    double GetAverageLoad ();
    long GetReplicasInUse (Client*);

    double GetAverageObjects ();
    void SetAverageObjects (double);

    long NumberOfObjects;
    ObjectData Replicas[MAX_OBJECTS];
    Boolean UseRandom;

private:
    Opcode unlockGroup (Client*);
    Opcode lockGroup (Client*, lock_code, long);

    void checkGroup (long&, long&, Client*, lock_code);

    void Reconfigure (lock_code);

    Protocol* RepProt;
    long NumberOfClients;
    double n_write, n_read, n_ops;
    double rw_ratio, margin, value;
    double objectSpread, averageTI;
    Boolean firstAssignment;
};

#endif
