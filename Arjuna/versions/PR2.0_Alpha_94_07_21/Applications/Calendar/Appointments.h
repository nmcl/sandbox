/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Appointments.h,v 1.1 1993/11/03 12:22:32 nmcl Exp $
 */

#include <Common/Uid.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>
#include <Common/Boolean.h>
#include <System/sys/time.h>

#include "AppointmentEntry.h"

// @NoRemote @NoMarshall
struct AppointmentList
{
    AppointmentEntry entry;
    AppointmentList *next;
};


// @NoRemote @NoMarshall
class Appointments : public LockManager
{
public:
    Appointments(int &res);				    // use when creating new instance (returns Uid of instance)
    Appointments(int &res, Uid u);			    // use when accessing persistant object
    ~Appointments();

    void GetUid(int &res, Uid &u);

    void Count(int &res, int &count);

    void GetAppointment(int &res, int appointment_no, AppointmentEntry &entry);
    void GetCurrAppointment(int &res, time_t time, AppointmentEntry &entry);
    void GetNextAppointment(int &res, time_t time, AppointmentEntry &entry);

    void AddAppointment(int &res, AppointmentEntry entry);
    void DelAppointment(int &res, int appointment_no);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    void Purge();
    Boolean Add(AppointmentEntry entry);			    // non-atomic add

    AppointmentList *appointments;
    int number_of_appointments;
};
