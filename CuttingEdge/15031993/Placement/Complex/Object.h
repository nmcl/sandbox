#ifndef OBJECT_
#define OBJECT_

// Define an AllObjects class which encompasses Objects

#include <Process.h>
#include "Protocol.h"

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
    Object (Protocol*, long, double, boolean);
    ~Object ();

    void SetReadQuorum (long);
    void SetWriteQuorum (long);
    long ReadQuorum (void);
    long WriteQuorum (void);
    long GetMinimum ();

    boolean PerformOperation (lock_code, Client*);
    boolean CheckQuorum (lock_code);

    double GetReadWriteRatio ();
    boolean LockAll ();
    void UnlockAll ();
    void RemoveObjects ();

    long NumberOfObjects;
    ObjectData Replicas[MAX_OBJECTS];
    boolean UseRandom;

private:
    Protocol* RepProt;
    long n_write, n_read;
    double rw_ratio, margin, value;

    void Reconfigure (lock_code);
};

#endif
