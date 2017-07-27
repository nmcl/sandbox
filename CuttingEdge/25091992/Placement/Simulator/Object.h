#ifndef OBJECT_
#define OBJECT_

// Define an AllObjects class which encompasses Objects

#include <Process.h>
#include "Protocol.h"

const int MAX_OBJECTS = 5; // Maximum number of objects in a replica group.

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
    boolean PerformOperation (lock_code, Client*);
    boolean CheckQuorum (lock_code);

    long NumberOfObjects;
    ObjectData Replicas[MAX_OBJECTS];

private:
    Protocol* RepProt;
    boolean Valid (long);
};

#endif
