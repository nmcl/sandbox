/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Calendar.h,v 1.1 1993/11/03 12:22:35 nmcl Exp $
 */

#include <System/sys/time.h>

#include <Arjuna/LockMan.h>
#include <ArjServers/ArjName.h>

#include "Appointments.h"


// @Remote @NoMarshall
class Calendar : public LockManager
{
public:
    Calendar(int &res, const char* server, const char* user, const char* location);
    Calendar(int &res, ArjunaName* AN);
    ~Calendar();

    void GetUserName(int &res, char* &user);
    void SetUserName(int &res, char* user);

    void GetCurrLocation(int &res, time_t now, char* &location);

    void GetDefLocation(int &res, char* &location);
    void SetDefLocation(int &res, char* location);

    void GetNameLocation(int &res, time_t now, char* &user, char* &location);

    void GetUpdateCount(int &res, int &count);
    void SetUpdateCount(int &res);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

    // methods to contact appointments object
    void AppointmentCount(int &res, int &count);
    void GetAppointment(int &res, int appointment_no, AppointmentEntry &entry);
    void GetNextAppointment(int &res, time_t now, AppointmentEntry &entry);
    void AddAppointment(int &res, AppointmentEntry entry);
    void DelAppointment(int &res, int appointment_no);

private:
    char*       default_location;
    char*       user_name;
    Appointments *appointments;
    int          update_count;
};
