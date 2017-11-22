/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ConnectMan.cc,v 1.6 1995/02/22 14:55:15 ngdp Exp $
 */

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_STAT_H_
#include <System/sys/stat.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef OBJSTOREI_H_
#  include <ObjectStore/ObjStoreI.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef CONNECTMAN_H_
#  include <ConnectMan.h>
#endif

/*
 * Enum: ConnectManStatus
 * Status of connection managed by a ConnectMan
 */

ostream& operator<< (ostream& strm, ConnectManStatus s)
{
    switch (s)
    {
        case DEAF:
            strm << "deaf";
            break;
        case LISTENING:
            strm << "listening";
            break;
        case DOWN:
            strm << "down";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Enum: RPCType
 * Possible RPC mechanisms supported by ConnectMan
 */

ostream& operator<< (ostream& strm, RPCType t)
{
    switch (t)
    {
        case RAJDOOT:
            strm << "Rajdoot";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Class: Connection Manager (ConnectMan)
 * Generic interface to alternative RPC mechanisms. Controls whether a
 * node will receive requests from remote nodes.
 */

//-Public-Functions------------------------------------------------------------

ConnectMan::ConnectMan(ostream& strm)
                      : outStrm(strm),
                        status(UNKNOWN)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ConnectMan::ConnectMan()" << endl;
#endif

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
        rpcType = RAJDOOT;
#else
        rpcType = UNKNOWN_RPC;
#endif
}

ConnectMan::ConnectMan(ostream& strm, RPCType system)
                      : outStrm(strm),
                        status(UNKNOWN),
                        rpcType(system)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ConnectMan::ConnectMan(" << system << ")" << endl;
#endif
}

ConnectMan::~ConnectMan()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ConnectMan::~ConnectMan()" << endl;
#endif
}

Boolean ConnectMan::control(Boolean state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ConnectMan::control(" << state << ")" << endl;
#endif

    Boolean success = FALSE;

    switch(rpcType)
    {
        case RAJDOOT:
        {
            success = switchManager(state);
        }
        break;

        default:
        {
             outStrm << "ConnectMan: *WARNING* Unknown RPC Type" << endl;
        }
        break;
    }

    if (success)
    {
       if (state)
           outStrm << "ConnectMan: External connections enabled" << endl;
       else
           outStrm << "ConnectMan: External connections disabled" << endl;
    }
    return success;
}

Boolean ConnectMan::isAlive()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ConnectMan::isAlive()" << endl;
#endif

    Boolean alive = FALSE;

    switch(rpcType)
    {
        case RAJDOOT:
        {
            alive = pingManager();
        }
        break;

        default:
        {
             outStrm << "ConnectMan: *WARNING* Unknown RPC Type" << endl;
        }
        break;
    }

    if (alive)
    {
	outStrm << "ConnectMan: The connection manager process is alive"
		<< endl;
    }
    
    return alive;
}

void ConnectMan::print(ostream& strm)
{
    strm << "RPC Type = " << rpcType << ", Status = " <<  status << flush;
}

//-Private-Functions-----------------------------------------------------------

// Operations for Rajdoot Manager

Boolean ConnectMan::switchManager(Boolean state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ConnectMan::switchManager(" << state << ")" << endl;
#endif

    Boolean success = FALSE;

    if ( ( (status == LISTENING) && (state) ) ||
         ( (status == DEAF) && (!state) ) )
    {
        // The manager is already in the desired state
        success = TRUE;
    }
    else
    {
        int managerPid = getManagerPid();
        if (managerPid == -1)
        {
            status = DOWN;
            outStrm << "ConnectMan: *WARNING* Could not get manager pid"
                    << endl;
        }
        else
        {
            if (state)
            {
                // Signal the manager to accept requests
                if (kill(managerPid, SIGUSR1) != 0)
                {
                    status = DOWN;
                    outStrm << "ConnectMan: *WARNING* "
                            << "Attempt to signal Rajdoot manager failed"
                            << endl;
                }
                else
                {
                    success = TRUE;
                    status = LISTENING;
                }
            }
            else
            {
                // Signal the manager to refuse requests
                if (kill(managerPid, SIGUSR2) != 0)
                {
                    status = DOWN;
                    outStrm << "ConnectMan: *WARNING* "
                            << "Attempt to signal Rajdoot manager failed"
                            << endl;
                }
                else
                {
                    success = TRUE;
                    status = DEAF;
                }
            }
        }
    }

    return (success);
}

Boolean ConnectMan::pingManager()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ConnectMan::pingManager()" << endl;
#endif

    Boolean alive = FALSE;

    int managerPid = getManagerPid();
    if (managerPid == -1)
    {
	outStrm << "ConnectMan: *WARNING* Could not get manager pid"
		<< endl;
    }
    else
    {
	// Ping the manager process
	if (kill(managerPid, 0) != 0)
	{
	    outStrm << "ConnectMan: *WARNING* "
		    << "Attempt to signal Rajdoot manager failed" << endl;
	}
	else
	{
	    alive = TRUE;
	}
    }

    return (alive);
}

int ConnectMan::getManagerPid ()
{

/* 
 * Read MANPID_FILE file and return the id of the manager process
 * if successful; otherwise -1.
 */

    static int managerPid = -1;

    ifstream in(MANPID_FILE_Q);

    if ( !in )
    {
        outStrm << "ConnectMan: *WARNING* Cannot open MANPID_FILE" << endl;
        managerPid = -1;
    }
    else
    {
        in >> managerPid;
    }

    return(managerPid);
}



