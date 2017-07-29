#ifndef OBJECT_
#define OBJECT_

// Define an AllObjects class which encompasses Objects

#include <Process.h>
#include "Protocol.h"

#define MAX_OBJECTS 5

class Machine;
class Client;

class ObjectData
{
public:
    ObjectData();
    ~ObjectData();

    lock_code Lock;
    boolean Available;
    Machine* location;
    Client* User;
};


class Object
{
private:
    Protocol* RepProt;
    boolean Valid(long);

public:
    Object(Protocol*, long);
    ~Object();

    long NumberOfObjects;
    ObjectData Replicas[MAX_OBJECTS];

    void SetReadQuorum(long);
    void SetWriteQuorum(long);
    boolean PerformOperation(lock_code, Client*);
    boolean CheckQuorum(lock_code);
};

#endif
