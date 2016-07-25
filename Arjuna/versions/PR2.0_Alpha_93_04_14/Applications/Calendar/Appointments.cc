/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Appointments.cc,v 1.2 1993/03/18 13:51:02 nmcl Exp $
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define APPOINTMENTS_SERVER


#include <time.h>
#include <System/string.h>

#include <Arjuna/AtomicA.h>

#ifdef DEBUG
#include <Common/Debug.h>
#endif

#include "Appointments_stub.h"
#include "Retries.h"

Appointments::Appointments(int &res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::Appointments(int &res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    appointments = NULL;
    number_of_appointments = 0;
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
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


Appointments::Appointments(int &res, Uid u) : LockManager(u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::Appointments(int &res, Uid u)\n" << flush;
#endif

    // set variables to harmless dummy values. If state required it
    // will be first loaded (via restore_state)
    appointments = NULL;
    number_of_appointments = 0;
    res = 1;
}


Appointments::~Appointments()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::~Appointments()\n" << flush;
#endif

    Purge();
}


void Appointments::GetUid(int &res, Uid &u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::GetUid(int &res, Uid &u)\n" << flush;
#endif
    res = 1;
    u = get_uid();
}


void Appointments::Count(int &res, int &count)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::Count(int &res, int &count)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	count = number_of_appointments;
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


void Appointments::GetAppointment(int &res, int appointment_no, AppointmentEntry &entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::GetAppointment(int &res, int appointment_no, AppointmentEntry &entry)\n" << flush;
#endif

    AtomicAction A;
    int i;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	if (appointment_no <= number_of_appointments)
	{
	    AppointmentList *tmp = appointments;
	    for(i = 0; i < appointment_no; i++)
	    {
		tmp = tmp->next;
	    }
	    entry = tmp->entry;

#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	    debug_stream << "Appointments::GetAppointment -> entry restored\n";
	    debug_stream << "                  entry.start       = " << ctime(&(entry.start));
	    debug_stream << "                  entry.end         = " << ctime(&(entry.end));
	    debug_stream << "                  entry.description = " << entry.description << "\n";
	    if (entry.confirmed)
		debug_stream << "                    entry.confirmed = TRUE\n" << flush;
	    else
		debug_stream << "                    entry.confirmed = FALSE\n" << flush;
#endif

	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -1;					    /* out of range appointment number */
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}

void Appointments::GetCurrAppointment(int &res, time_t time, AppointmentEntry &entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::GetAppointment(int &res, time_t time, AppointmentEntry &entry)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
	debug_stream << "Appointments::GetCurrAppointment -> looking for appointment containing time = " << ctime(&time) << flush;
#endif

	AppointmentList *tmp = appointments;

	entry.start = 0;
	entry.end   = 0;
	entry.description = "";

	while (tmp != NULL)
	{

#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	    debug_stream << "Appointments::GetCurrAppointment -> checking entry\n";
	    debug_stream << "                  entry.start       = " << ctime(&(tmp->entry.start));
	    debug_stream << "                  entry.end         = " << ctime(&(tmp->entry.end));
	    debug_stream << "                  entry.description = " << tmp->entry.description << "\n";
	    if (tmp->entry.confirmed)
		debug_stream << "                    entry.confirmed = TRUE\n" << flush;
	    else
		debug_stream << "                    entry.confirmed = FALSE\n" << flush;
#endif

	    if (tmp->entry.start <= time)
	    {
		if (tmp->entry.end >= time)	// found appointment for 'time'
		{
#ifdef DEBUG
		    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
		    debug_stream << "Appointments::GetCurrAppointment -> appointment match found\n" << flush;
#endif

		    entry = tmp->entry;
		    break;

		}
		else
		    tmp = tmp->next;
	    }
	    else
		break;				// stop search -> all further appointments for later times
	}
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


void Appointments::GetNextAppointment(int &res, time_t time, AppointmentEntry &entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::GetAppointment(int &res, time_t time, AppointmentEntry &entry)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(READ), RETRIES) == GRANTED)
    {
	AppointmentList *tmp = appointments;

	entry.start = 0;
	entry.end   = 0;
	entry.description = "";

	while (tmp != NULL)
	{
	    if (tmp->entry.start <= time)         // appointment starts before given time
		    tmp = tmp->next;
	    else
	    {					  // found first appointment starting after given time
		entry = tmp->entry;
		break;
	    }
	}
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


void Appointments::AddAppointment(int &res, AppointmentEntry entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::AddAppointment(int &res, AppointmentEntry entry)\n" << flush;
#endif
    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
    {
	if (Add(entry))
	{
	    if (A.End() == COMMITTED)
		res = 1;
	    else
		res = -1;
	}
	else
	{
	    res = -2;
	    A.Abort();
	}
    }
    else
    {
	res = -1;
	A.Abort();
    }
}


void Appointments::DelAppointment(int &res, int appointment_no)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Appointments::DelAppointment(int &res, int appointment_no)\n";
    debug_stream << "                             appointment_no = " << appointment_no << "\n" << flush;
#endif

    AtomicAction A;
    AppointmentList *here, *trail;
    int i;

    A.Begin();
    if (setlock(new Lock(WRITE), RETRIES) == GRANTED)
    {
	if (appointment_no <= number_of_appointments)
	{
	    trail = NULL;
	    here  = appointments;

	    for(i = 0; i < appointment_no; i++)
	    {
		trail = here;
		here = here->next;
	    }

	    if (trail == NULL)
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
		debug_stream << "Appointments::DelAppointment -> deleting first entry\n" << flush;
#endif
		appointments = here->next;
		delete(here->entry.description);
		delete(here);
	    }
	    else
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
		debug_stream << "Appointments::DelAppointment -> deleting entry\n" << flush;
#endif
		trail->next = here->next;
		delete(here->entry.description);
		delete(here);
	    }

	    number_of_appointments--;

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


Boolean Appointments::save_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "Appointments::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    int i;
    AppointmentList *tmp;

    os.pack(number_of_appointments);
    tmp = appointments;
    for(i = 0; i < number_of_appointments; i++)
    {

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	debug_stream << "Appointments::save_state -> saving entry\n";
	debug_stream << "                  entry.start       = " << ctime(&(tmp->entry.start));
	debug_stream << "                  entry.end         = " << ctime(&(tmp->entry.end));
	debug_stream << "                  entry.description = " << tmp->entry.description << "\n";
	if (tmp->entry.confirmed)
	    debug_stream << "                    entry.confirmed = TRUE\n" << flush;
	else
	    debug_stream << "                    entry.confirmed = FALSE\n" << flush;
#endif

	os.pack(tmp->entry.start);
	os.pack(tmp->entry.end);
	os.pack(tmp->entry.description);
	os.pack(tmp->entry.confirmed);
	tmp = tmp->next;
    }

    return TRUE;
}


Boolean Appointments::restore_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "Appointments::restore_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    int i;
    AppointmentList *tail;

    Purge();					     // get rid of previous state
    os.unpack(number_of_appointments);
    tail = NULL;
    for(i = 0; i < number_of_appointments; i++)
    {
	AppointmentList *newentry = new AppointmentList;
	newentry->next = NULL;
	os.unpack(newentry->entry.start);
	os.unpack(newentry->entry.end);
	os.unpack(newentry->entry.description);
	os.unpack(newentry->entry.confirmed);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	debug_stream << "Appointments::restore_state -> entry restored\n";
	debug_stream << "                  entry.start       = " << ctime(&(newentry->entry.start));
	debug_stream << "                  entry.end         = " << ctime(&(newentry->entry.end));
	debug_stream << "                  entry.description = " << newentry->entry.description << "\n";
	if (newentry->entry.confirmed)
	    debug_stream << "                    entry.confirmed = TRUE\n" << flush;
	else
	    debug_stream << "                    entry.confirmed = FALSE\n" << flush;
#endif

	if (tail == NULL)
	    appointments = newentry;
	else
	    tail->next = newentry;
	tail = newentry;
    }

    return TRUE;
}


const TypeName Appointments::type() const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "Appointments::type()\n" << flush;
#endif

    return "/StateManager/LockManager/Appointments";
}


void Appointments::Purge()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PRIVATE;
    debug_stream << "Appointments::Purge()\n" << flush;
#endif

    AppointmentList *tmp;

    // free memory used by appointments
    while (appointments != NULL)
    {
	tmp = appointments->next;
	delete(appointments->entry.description);
	delete(appointments);
	number_of_appointments--;
	appointments = tmp;
    }
}


Boolean Appointments::Add(AppointmentEntry entry)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1, VIS_PRIVATE;
    debug_stream << "Appointments::Add(AppointmentEntry entry)\n";
    debug_stream << "                  entry.start       = " << ctime(&(entry.start));
    debug_stream << "                  entry.end         = " << ctime(&(entry.end));
    debug_stream << "                  entry.description = " << entry.description << "\n";
    if (entry.confirmed)
	debug_stream << "                    entry.confirmed = TRUE\n" << flush;
    else
	debug_stream << "                    entry.confirmed = FALSE\n" << flush;
#endif

    AppointmentList *newentry = new AppointmentList;
    newentry->entry = entry;
    newentry->next  = NULL;

    if (appointments == NULL)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	debug_stream << "Appointments::Add -> no previous appointments\n" << flush;
#endif
	appointments = newentry;
	number_of_appointments++;
	return TRUE;
    }
    else
    {
	AppointmentList *curr, *prev;

	prev = NULL;
	curr = appointments;
	while (curr != NULL)				    // scan while there are appointments
	{
	    if (entry.start >= curr->entry.end)		    // and new appointment starts after curr appointment ends
	    {
		prev = curr;
		curr = curr->next;
	    }
	    else
		break;
	}
	/* when exiting loop, either curr is NULL, or points to   */
	/* an appointment which ends after new appointment begins */

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	if (prev != NULL)
	    debug_stream << "Appointments::Add -> prev (start,end) = (" << prev->entry.start << ", " << prev->entry.end << ")\n" << flush;
	else
	    debug_stream << "Appointments::Add -> prev == NULL\n" << flush;
	if (curr != NULL)
	    debug_stream << "Appointments::Add -> curr (start,end) = (" << curr->entry.start << ", " << curr->entry.end << ")\n" << flush;
	else
	    debug_stream << "Appointments::Add -> curr == NULL\n" << flush;
#endif

	if (curr == NULL)				    // new appointment is last appointment
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	    debug_stream << "new appointment is last appointment\n" << flush;
#endif
	    prev->next = newentry;
	    number_of_appointments++;
	    return TRUE;
	}

	if (prev == NULL)				    // new appointment potentially is first appointment
	{
	    if (curr->entry.start >= entry.end)		    // check new appointment ends before curr appointment starts
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
		debug_stream << "new appointment is first appointment\n" << flush;
#endif
		newentry->next = curr;
		appointments = newentry;
		number_of_appointments++;
		return TRUE;
	    }
	    else					    // new appointment overlaps with current appointment
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
		debug_stream << "new appointment conflicts (1)\n" << flush;
#endif
		delete newentry;
		return FALSE;
	    }
	}
							    // check no overlap between new appointment & prev & curr appointments
	if ((prev->entry.end <= entry.start) && (curr->entry.start >= entry.end))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	    debug_stream << "new appointment fits between prev & curr\n" << flush;
#endif

	    prev->next = newentry;
	    newentry->next = curr;
	    number_of_appointments++;
	    return TRUE;
	}
	else
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PRIVATE;
	    debug_stream << "new appointment conflicts (2)\n" << flush;
#endif
	    delete newentry;
	    return FALSE;
	}
    }
}

