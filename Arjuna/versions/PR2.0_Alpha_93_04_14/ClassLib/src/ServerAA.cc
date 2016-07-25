/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerAA.cc,v 1.1 1993/04/05 13:23:19 ngdp Exp $
 */

/*
 *
 * ServerAtomicAction class
 *
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifdef DEBUG 
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <Arjuna/ObjStore.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

static const char RCSid[] = "$Id: ServerAA.cc,v 1.1 1993/04/05 13:23:19 ngdp Exp $";

ServerAtomicAction* ServerAtomicAction::current_svr_act  = 0;
char*               ServerAtomicAction::coordin_nodename = 0;

/* Constructors and destructor */
    
ServerAtomicAction::ServerAtomicAction (const Uid& U, ActionType at) 
                                       : AtomicAction(U, at),
					 saved_intention_list(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::ServerAtomicAction (const Uid&)\n" << flush;
#endif
    parent_server_action   = 0;
    previousPrepareOutcome = PREP_NOTOK;
}

ServerAtomicAction::~ServerAtomicAction()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::~ServerAtomicAction()\n" << flush;
#endif

    if (coordin_nodename)
    {
	delete [] coordin_nodename;
	coordin_nodename = 0;
    }
}

/* User Atomic action operations */
 
ActionStatus ServerAtomicAction::Begin()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::Begin()\n" << flush;
#endif

    if (status() == CREATED)
    {
        parent_server_action = current_svr_act;
        current_svr_act = this;

        return AtomicAction::Begin();
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::End() Incorrent action status (" << status() << ")\n";
        return status();
    }
  
}

ActionStatus ServerAtomicAction::End()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::End()\n" << flush;
#endif

    if (status() == RUNNING)
        return AtomicAction::End();
    else
    {
        error_stream << WARNING << "ServerAtomicAction::End() Incorrent action status (" << status() << ")\n";
	return status();
    }
}

ActionStatus ServerAtomicAction::Abort()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::Abort()\n" << flush;
#endif

    if (status() == RUNNING)
        return AtomicAction::Abort();
    else
    {
        error_stream << WARNING << "ServerAtomicAction::Abort() Incorrent action status (" << status() << ")\n";
        return status();
    }
}

void ServerAtomicAction::set_coordin_nodename ( const char *name )
{
    if (coordin_nodename)
    {
	delete [] coordin_nodename;
	coordin_nodename = 0;
    }
    
    if (name)
    {
	int nameLen = ::strlen(name);
	coordin_nodename = new char[nameLen+1];
	
	::strcpy(coordin_nodename, name);
    }
}

char *ServerAtomicAction::get_coordin_nodename ()
{
    char *name = 0;
    
    if (coordin_nodename)
    {
	int nameLen = ::strlen(coordin_nodename);
	name = new char[nameLen+1];
	
	::strcpy(name, coordin_nodename);
    }

    return name;
}

Uid ServerAtomicAction::get_saving_uid () const
{
    return saving_uid;
}

void ServerAtomicAction::Finish()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::Finish()\n" << flush;
#endif

    if (this != current_svr_act)
    {
        error_stream << WARNING << "ServerAtomicAction::Finish() Not on Current action\n";
	return;
    }

    switch (status())
    {
    case CREATED:
	break;
    case RUNNING:
        AtomicAction::Abort();
	break;
    case ABORTED:
	break;
    case COMMITTED:
	break;
    case UNPREPARED :
	AtomicAction::phase2Abort();
	break;
    case PREPARED :
	AtomicAction::phase2Abort();
	break;
    default:
        error_stream << WARNING << "ServerAtomicAction::Finish() Unexpected action status (" << status() << ")\n";
	break;
    }
    current_svr_act = parent_server_action;
}

ServerAtomicAction* ServerAtomicAction::Current()
{
    return current_svr_act;
}

ServerAtomicAction* ServerAtomicAction::parent()
{
    return parent_server_action;
}

/* Primitive Atomic action operations */ 

Boolean ServerAtomicAction::DoNested_Abort (Uid& ActionUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::DoNested_Abort (Uid&) on " << (void*) AtomicAction::Current() << "\n" << flush;
#endif

    Boolean done =  FALSE;

    if (ActionUid == get_uid())
    {
	if ((status() == PREPARED) || (status() == UNPREPARED))
	{
	    AtomicAction::phase2Abort();
	    done =  TRUE;
	}
	else if (status() == RUNNING)
	{
	    AtomicAction::Abort();
	    done =  TRUE;
	}
	else if (status() == ABORTED)
	    done =  TRUE;
	else
	{
	    error_stream << WARNING << "ServerAtomicAction::DoNested_Abort() : Incorrent action status (" << status() << ")\n";
	    done =  FALSE;
	}
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::DoNested_Abort() : Wrong action!\n";
	done =  FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::DoNested_Commit (Uid& ActionUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::DoNested_Commit (Uid&) on " << (void*) AtomicAction::Current() << "\n" << flush;
#endif

    Boolean done =  FALSE;

    if (ActionUid == get_uid())
    {
	if (status() == PREPARED)
	{
	    AtomicAction::phase2Commit();
	    done = TRUE;
	}
	else if (status() == COMMITTED)
	    done =  TRUE;
	else
	{
	    error_stream << WARNING << "ServerAtomicAction::DoNested_Commit() : Incorrent action status (" << status() << ")\n";
	    done =  FALSE;
	}
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::DoNested_Commit() : Wrong action!\n";
	done =  FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::DoNested_Prepare (Uid& ActionUid, PrepareOutcome& OutCome)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::DoNested_Prepare (Uid&, PrepareOutcome&) on " << (void*) AtomicAction::Current() << "\n" << flush;
#endif

    Boolean done =  FALSE;
    OutCome = PREP_OK;

    if (ActionUid == get_uid())
    {
	if (status() == RUNNING)
	{
	    OutCome = AtomicAction::prepare();
	    previousPrepareOutcome = OutCome;
	    done =  TRUE;
	}
	else if ((status() == PREPARED) || (status() == UNPREPARED))
	{
	    OutCome = previousPrepareOutcome;
	    done =  TRUE;
	}
	else
	{
	    error_stream << WARNING << "ServerAtomicAction::DoNested_Prepare() : Incorrent action status (" << status() << ")\n";
	    done = FALSE;
	}
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::DoNested_Prepare() : Wrong action!\n";
	done = FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::DoTopLevel_Abort (Uid& ActionUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::DoTopLevel_Abort (Uid&) on " << (void*) AtomicAction::Current() << "\n" << flush;
#endif

    Boolean done = FALSE;

    if (ActionUid == get_uid())
    {
	if ((status() == PREPARED) || (status() == UNPREPARED))
	{
	    AtomicAction::phase2Abort();
	    done =  TRUE;
	}
	else if (status() == RUNNING)
	{
	    AtomicAction::Abort();
	    done =  TRUE;
	}
	else if (status() == ABORTED)
	    done =  TRUE;
	else
	{
	    error_stream << WARNING << "ServerAtomicAction::DoTopLevel_Abort() : Incorrent action status (" << status() << ")\n";
	    done = FALSE;
	}
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::DoTopLevel_Abort() : Wrong action!\n";
	done = FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::DoTopLevel_Commit (Uid& ActionUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::DoTopLevel_Commit (Uid&) on " << (void*) AtomicAction::Current() << "\n" << flush;
#endif

    Boolean done = FALSE;
    
    if (ActionUid == get_uid())
    {
	if (status() == PREPARED)
	{
	    AtomicAction::phase2Commit();
	    if (saved_intention_list)
	    {
		LocalPersistentObjectStore tempStore;
		char *path = tempStore.genPathName(get_saving_uid(), type());
		

		done = (Boolean)(unlink(path) == 0);
//		done = currentStore->remove_state (get_saving_uid(), type());

		delete [] path;
	    }
	    else
		done =  TRUE;
	}
	if (status() == COMMITTED)
	    done =  TRUE;
	else
	{
	    error_stream << WARNING << "ServerAtomicAction::DoTopLevel_Commit() : Incorrent action status (" << status() << ")\n";
	    done = FALSE;
	}
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::DoTopLevel_Commit() : Wrong action!\n";
	done = FALSE;
    }

    return done;
}

Boolean ServerAtomicAction::DoTopLevel_Prepare (Uid& ActionUid, PrepareOutcome& OutCome)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::DoTopLevel_Prepare (Uid&, PrepareOutcome&) on " << (void*) AtomicAction::Current() << "\n" << flush;
#endif

    Boolean done = FALSE;
    OutCome = PREP_OK;

    if (ActionUid == get_uid())
    {
	if (status() == RUNNING)
	{
	    OutCome = AtomicAction::prepare();
	    previousPrepareOutcome = OutCome;
	    done =  TRUE;
	}
	else if ((status() == PREPARED) || (status() == UNPREPARED))
	{
	    OutCome = previousPrepareOutcome;
	    done =  TRUE;
	}
	else
	{
 	    error_stream << WARNING << "ServerAtomicAction::DoTopLevel_Prepare() : Incorrent action status (" << status() << ")\n";
	    done = FALSE;
	}
    }
    else
    {
        error_stream << WARNING << "ServerAtomicAction::DoTop,Level_Prepare() : Wrong action!\n";
	done = FALSE;
    }

    return done;
}

/* inherited public virtual functions */

Boolean ServerAtomicAction::restore_state (ObjectState&, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::restore_state (ObjectState& s, ObjectType t)\n" << flush;
#endif

    error_stream << WARNING << "ServerAtomicAction::restore_state(ObjectState&, ObjectType) : Called!\n";

    return  TRUE;
}

Boolean ServerAtomicAction::save_state (ObjectState& s, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::save_state (ObjectState& s, ObjectType t)\n" << flush;
#endif

    Boolean   res = TRUE;
    int    record_type;
    Uid    r_u(NIL_UID);
    char*  r_type = 0;
    char*  fname = 0;

    ObjectState os;
    LocalPersistentObjectStore store;

    fname = store.genPathName(get_saving_uid(), type());
    store.createHierarchy (fname, O_WRONLY);

    ofstream outfile (fname, ios::out, 0666);

    outfile << get_uid() << '\0';
    outfile << coordin_nodename << '\0' << getpid() << '\0';

    res = (AtomicAction::save_state(os, ANDPERSISTENT) && os.unpack(record_type));

    while ((record_type != NONE_RECORD) && res)
    {
	if ((record_type != PERSISTENCE) && (record_type != RPCCALL))
	    error_stream << WARNING << "ServerAtomicAction::save_state(): Unknown Record Type: " << record_type << "\n";
	else if (res = (r_u.unpack(os) && os.unpack(r_type)))
	{
	    if (record_type == PERSISTENCE)
		outfile << store.getStoreName() << r_type << '/' << r_u << '\0';

	    res = os.unpack(record_type);
	}
    }
    
    outfile.flush();
    saved_intention_list = TRUE;
    outfile.close();

    if (! (res && outfile.good()))
    {
	res = FALSE;
    }

    delete fname;
    delete r_type;

    return res;

//   If saved as ObjectState Record.
//
//   return (bool) (get_uid().pack(s) && s.pack(coordin_nodename) && 
//		    s.pack(getpid()) && AtomicAction::save_state(s, t));
}

const TypeName ServerAtomicAction::type () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerAtomicAction::::type ()\n" << flush;
#endif

    return "/StateManager/AtomicAction/ServerAtomicAction";
}
