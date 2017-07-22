#ifndef MONITOR_H_
#define MONITOR_H_

#include <rpcsvc/rstat.h>

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef OBJSTATE_H_
#include <Arjuna/ObjState.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

const unsigned int sleepTime = 60;

class Monitor : public LockManager
{
public:
    Monitor (int&);
    Monitor (Uid&, int&);
    ~Monitor ();

    void doMonitor (int number = 0);

    timeval* GetMTTF (Boolean&);
    timeval* GetMTTR (Boolean&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    Boolean Calculate (Boolean);
    void getFailure (timeval, timeval);
    void getRecovery (timeval, timeval);
    void getLoad (statstime);

    Boolean putTime ();
    void initTimeval (timeval&);
    unsigned long sampleNumber;

    timeval MTTF, MTTR, currTime, lastBootTime;
    long avenrun[3];
};

#endif
