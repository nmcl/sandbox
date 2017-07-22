#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef PROTOS_H_
#include <System/protos.h>
#endif

#ifndef UNISTD_H_
#include <System/unistd.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef MONITOR_H_
#include "Monitor.h"
#endif

extern "C"
{
    int rstat(char*, struct statstime*);
}

Monitor::Monitor (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    cout << "Monitor::Monitor ()\n" << flush;
#endif

    AtomicAction A;
    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	sampleNumber = 0;
	initTimeval(MTTF);
	initTimeval(MTTR);
	initTimeval(currTime);
	initTimeval(lastBootTime);
	for (int j = 0; j < 3; j++)
	    avenrun[j] = 0;

	(void)Calculate(FALSE);
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

Monitor::Monitor (Uid& u, int& res) : LockManager(u)
{
#ifdef DEBUG
    cout << "Monitor::Monitor (" << u << ", " << res << " )\n" << flush;
#endif

    if (Calculate(TRUE))
	res = 0;
}

Monitor::~Monitor ()
{
#ifdef DEBUG
    cout << "Monitor::~Monitor ()\n" << flush;
#endif

    terminate();
}

void Monitor::initTimeval (timeval& toAlter)
{
#ifdef DEBUG
    cout << "void Monitor::initTimeval (timeval& toAlter)\n" << flush;
#endif

    toAlter.tv_sec = 0;
    toAlter.tv_usec = 0;
}

Boolean Monitor::putTime ()
{
#ifdef DEBUG
    cout << "Boolean Monitor::putTime ()\n" << flush;
#endif

    AtomicAction B;
    Boolean result = FALSE;

    B.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	::gettimeofday(&currTime, 0);

	if (B.End() == COMMITTED)
	    result = TRUE;
    }
    else
	B.Abort();

    return result;
}

void Monitor::doMonitor (int number)
{
#ifndef DEBUG
    cout << "void Monitor::doMonitor ()\n" << flush;
#endif

    Boolean status;
    int indx = -1;

    while (indx < number)
    {
	::sleep(sleepTime);

	status = FALSE;
	while (!status)
	    status = putTime();

	if (number != 0)
	    indx++;
    }
}

void Monitor::getFailure (timeval cTime, timeval bTime)
{
#ifdef DEBUG
    cout << "void Monitor::getFailure ()\n" << flush;
#endif

    long temp = (MTTF.tv_sec*1000000 + MTTF.tv_usec) * (sampleNumber -1);
    long tv = (cTime.tv_sec*1000000 + cTime.tv_usec) - (bTime.tv_sec*1000000 + bTime.tv_usec);

#ifdef DEBUG
    cout << "Monitor::getFailure - current time <" << cTime.tv_sec << ", " << cTime.tv_usec << ">\n" << flush;
    cout << "Monitor::getFailure - last boot time <" << bTime.tv_sec << ", " << bTime.tv_usec << ">\n" << flush;
    cout << "Monitor::getFailure - current MTTF <" << MTTF.tv_sec << ", " << MTTF.tv_usec << ">\n" << flush;
#endif

    MTTF.tv_sec = (temp+tv)/(1000000*sampleNumber);
    MTTF.tv_usec = (temp+tv)%(1000000*sampleNumber);
}

void Monitor::getRecovery (timeval cTime, timeval bTime)
{
#ifdef DEBUG
    cout << "void Monitor::getRecovery ()\n" << flush;
#endif

    long temp = (MTTR.tv_sec*1000000 + MTTR.tv_usec) * (sampleNumber -1);
    long tv = (bTime.tv_sec*1000000 + bTime.tv_usec) - (cTime.tv_sec*1000000 + cTime.tv_usec);

#ifdef DEBUG
    cout << "Monitor::getRecovery - bootime is <" << bTime.tv_sec << ", " << bTime.tv_usec << ">\n" << flush;
    cout << "Monitor::getRecovery - current MTTR is <" << MTTR.tv_sec << ", " << MTTR.tv_usec << ">\n" << flush;
    cout << "Monitor::getRecovery - current time is <" << cTime.tv_sec << ", " << cTime.tv_usec << ">\n" << flush;
#endif
    
    MTTR.tv_sec = (temp+tv)/(1000000*sampleNumber);
    MTTR.tv_usec = (temp+tv)%(1000000*sampleNumber);
}

void Monitor::getLoad (statstime statp)
{
#ifdef DEBUG
    cout << "void Monitor::getLoad (statstime statp)\n" << flush;
#endif

    long temp_avenrun[3];

    for (int j = 0; j < 3; j++)
    {
	temp_avenrun[j] = (avenrun[j] * (sampleNumber -1)) + statp.avenrun[j];
	avenrun[j] = temp_avenrun[j]/sampleNumber;
    }
}

Boolean Monitor::Calculate (Boolean doMTT)
{
#ifdef DEBUG
    cout << "void Monitor::Calculate ()\n" << flush;
#endif

    AtomicAction B;
    Boolean result = FALSE;

    B.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
#ifdef DEBUG
	cout << "Monitor::Calculate - Sample number is " << sampleNumber << "\n" << flush;
#endif

	statstime statp;
	char hostName[MAXHOSTNAMELEN];

	::memset(hostName, '\0', MAXHOSTNAMELEN);
	::gethostname(hostName, MAXHOSTNAMELEN);

	if (rstat(hostName, &statp) == 0)
	{
	    if (doMTT)
	    {
		sampleNumber++;
		getRecovery(currTime, statp.boottime);
		getFailure(statp.curtime, lastBootTime);
	    }

	    getLoad(statp);

	    currTime.tv_sec = statp.curtime.tv_sec;
	    currTime.tv_usec = statp.curtime.tv_usec;
	    lastBootTime.tv_sec = statp.boottime.tv_sec;
	    lastBootTime.tv_usec = statp.boottime.tv_usec;

	    if (B.End() == COMMITTED)
		result = TRUE;
	}
	else
	    B.Abort();
    }
    else
	B.Abort();

    return result;
}

timeval* Monitor::GetMTTF (Boolean& result)
{
#ifdef DEBUG
    cout << "timeval Monitor::GetMTTF (Boolean& result)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	{
	    result = TRUE;
	    return &MTTF;
	}
    }
    else
	A.Abort();

    return 0;
}

timeval* Monitor::GetMTTR (Boolean& result)
{
#ifdef DEBUG
    cout << "timeval Monitor::GetMTTR (Boolean& result)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	{
	    result = TRUE;
	    return &MTTR;
	}
    }
    else
	A.Abort();

    return 0;
}

Boolean Monitor::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean Monitor::save_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean result;

    result = os.pack(sampleNumber) && os.pack(MTTF.tv_sec);
    result = result && os.pack(MTTF.tv_usec);
    result = result && os.pack(MTTR.tv_sec);
    result = result && os.pack(MTTR.tv_usec);
    result = result && os.pack(currTime.tv_sec);
    result = result && os.pack(currTime.tv_usec);
    result = result && os.pack(lastBootTime.tv_sec);
    result = result && os.pack(lastBootTime.tv_usec);
    for (int i = 0; i < 3; i++)
	result = result && os.pack(avenrun[i]);

    return result;
}

Boolean Monitor::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean Monitor::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean result;

    result = os.unpack(sampleNumber) && os.unpack(MTTF.tv_sec);
    result = result && os.unpack(MTTF.tv_usec);
    result = result && os.unpack(MTTR.tv_sec);
    result = result && os.unpack(MTTR.tv_usec);
    result = result && os.unpack(currTime.tv_sec);
    result = result && os.unpack(currTime.tv_usec);
    result = result && os.unpack(lastBootTime.tv_sec);
    result = result && os.unpack(lastBootTime.tv_usec);
    for (int i = 0; i < 3; i++)
	result = result && os.unpack(avenrun[i]);

    return result;
}

const TypeName Monitor::type () const
{
    return "/StateManager/LockManager/Monitor";
}
