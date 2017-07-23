#ifndef Replicated_
#define Replicated_

#include "Common.h"
#include "Transaction.h"

class ObjectInfo
{
public:
    ObjectInfo();
    ~ObjectInfo();

    boolean grant;
    int quorum;
    boolean Map[TotalMachines];
    lockcode Mode;
    int ID;
};



class ReplicatedObject
{
    boolean grant;
    int needed, attempt;
    ObjectInfo Data;
    double AvailabilityRequirement; // percentage of time that the object must be available
public:
    ReplicatedObject(int, double);
    ~ReplicatedObject();

    ObjectInfo Lock(Transaction*, lockcode);
    void Unlock(Transaction*);
    boolean Check(ObjectInfo&, lockcode);
    int ObjectID();
    boolean IsMapped(int);
    lockcode LockType();

    int ReplicaCount, ReadQuorum, WriteQuorum;
};

#endif

