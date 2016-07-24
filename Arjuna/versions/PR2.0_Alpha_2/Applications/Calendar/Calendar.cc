/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Calendar.cc,v 1.1 1993/11/03 12:22:33 nmcl Exp $
 */

#define  CALENDAR_SERVER
#define  LOCKMANAGER_SERVER
#define  STATEMANAGER_SERVER
#define  NAMEDINT_SERVER

#include <System/string.h>
#include <System/time.h>


#include <Arjuna/AtomicA.h>

#ifdef DEBUG
#include <Common/Debug.h>
#endif

#include "Calendar_stub.h"

#include "Retries.h"

Calendar::Calendar(int &res, const char* server, const char* user, const char* location) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS <<  FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::Calendar(int& res, char* server, char* user, char* location)\n";
    debug_stream << "server   = " << server << "\n";
    debug_stream << "user     = " << user   << "\n";
    debug_stream << "location = " << location << "\n" << flush; 
#endif

    ArjunaName AN;
    AtomicAction A;
    int r;

    A.Begin();
    default_location = NULL;
    user_name = NULL;
    appointments = NULL;
    update_count = 0;
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
    {
	AN.Register(server, this);

	default_location = strdup(location);
	user_name        = strdup(user);
	update_count     = 0;

	r = 0;
	appointments = new Appointments(r);
	if (r < 1)
	{
	    res = -1;
	    A.Abort();
	}
	else
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


Calendar::Calendar(int &res, ArjunaName* AN) : LockManager(AN->GetRefObjUid())
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::Calendar(int& res, ArjunaName* AN)\n" << flush;
#endif

    res = 1;
    appointments = NULL;
}


Calendar::~Calendar()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::~Calendar()\n" << flush;
#endif

    if (appointments)
	delete appointments;

    terminate();
}


void Calendar::GetUserName(int &res, char*& user)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetUserName(int &res, char* user)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	user = user_name;
	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::SetUserName(int &res, char* user)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::SetUserName(int &res, char* user)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
    {
	if (user != NULL)
	    delete user_name;
	user_name = user;
	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::GetCurrLocation(int &res, time_t now, char*& location)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetCurrLocation(int &res, time_t now, char* location)\n" << flush;
#endif

    AtomicAction A;
    AppointmentEntry entry;
    entry.description = "";
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	ret = 0;
	appointments->GetCurrAppointment(ret, now, entry);
	if (ret > 0)
	{
	    if (entry.start != 0)
		location = entry.description;
	    else
		location = default_location;
	}
	else
	    location = "(LOCKED)";

	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::GetDefLocation(int &res, char*& location)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetDefLocation(int &res, char* location)\n" << flush;
#endif

   AtomicAction A;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	location = default_location;
	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::SetDefLocation(int &res, char* location)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::SetDefLocation(int &res, char* location)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
    {
	if (location != NULL)
	    delete default_location;
	default_location = location;
	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::GetNameLocation(int &res, time_t now, char* &user, char* &location)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetNameLocation(int &res, time_t now, char* &user, char* &location)\n" << flush;
#endif

    AtomicAction A;
    AppointmentEntry entry;
    entry.description = "";
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	user = user_name;

	ret = 0;
	appointments->GetCurrAppointment(ret, now, entry);
	if (ret > 0)
	{
	    if (entry.start != 0)
		location = entry.description;
	    else
		location = default_location;
	}
	else
	    location = "(LOCKED)";

	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::GetUpdateCount(int &res, int &count)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetUpdateCount(int &res, int &count)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	count = update_count;

	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::SetUpdateCount(int &res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::SetUpdateCount(int &res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
    {
	update_count++;

	if (A.End() == COMMITTED)
	    res = 1;
	else
	    res = -1;
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


Boolean Calendar::save_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "Calendar::save_state(ObjectState& os, ObjectType)\n";
    debug_stream << "     default_location = " << default_location << "\n";
    debug_stream << "            user_name = " << user_name << "\n";
    debug_stream << "         update_count = " << update_count << "\n" << flush;
#endif

    Uid u(NIL_UID);
    int res;

    os.pack(default_location);
    os.pack(user_name);
    os.pack(update_count);

    res = 0;
    appointments->GetUid(res, u);
    if (res < 1)
	return FALSE;
    u.pack(os);
    
    return TRUE;
}


Boolean Calendar::restore_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "Calendar::restore_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Uid u(NIL_UID);
    int res;

    os.unpack(default_location);
    os.unpack(user_name);
    os.unpack(update_count);

    u.unpack(os);
    if (appointments == NULL)
    {
	res = 0;
	appointments = new Appointments(res, u);
	if (res < 1)
	    return FALSE;
    }

#ifdef DEBUG
    debug_stream << "     default_location = " << default_location << "\n";
    debug_stream << "            user_name = " << user_name << "\n";
    debug_stream << "         update_count = " << update_count << "\n" << flush;
#endif

    return TRUE;
}


const TypeName Calendar::type() const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "Calendar::type()\n" << flush;
#endif

    return "/StateManager/LockManager/Calendar";
}


void Calendar::AppointmentCount(int &res, int &count)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::AppointmentCount(int &res, int &count)\n" << flush;
#endif

    AtomicAction A;
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	ret = 0;
	appointments->Count(ret, count);
	if (ret > 0)
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -1;
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::GetAppointment(int &res, int appointment_no, AppointmentEntry &entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetAppointment(int &res, int appointment_no, AppointmentEntry &entry)\n" << flush;
#endif


    AtomicAction A;
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	ret = 0;
	appointments->GetAppointment(ret, appointment_no, entry);
	if (ret > 0)
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -1;
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::GetNextAppointment(int &res, time_t now, AppointmentEntry &entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::GetNextAppointment(int &res, time_t now, AppointmentEntry &entry)\n" << flush;
#endif


    AtomicAction A;
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	ret = 0;
	appointments->GetNextAppointment(ret, now, entry);
	if (ret > 0)
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -1;
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::AddAppointment(int &res, AppointmentEntry entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::AddAppointment(int &res, AppointmentEntry entry)\n" << flush;
#endif


    AtomicAction A;
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	ret = 0;
	appointments->AddAppointment(ret, entry);
	if (ret > 0)
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -1;
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Calendar::DelAppointment(int &res, int appointment_no)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Calendar::DelAppointment(int &res, int appointment_no)\n" << flush;
#endif


    AtomicAction A;
    int ret;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	ret = 0;
	appointments->DelAppointment(ret, appointment_no);
	if (ret > 0)
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -1;
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}
