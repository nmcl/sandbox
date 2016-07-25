/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Control.h,v 1.1 1993/11/03 12:22:41 nmcl Exp $
 */

#include <System/time.h>

class Session;
class StringBrowser;
class ServerList;
class ArjunaName;
class ApplicationWindow;
class Patch;
class Glyph;

#define MINUTE 60
#define RESCAN_TIMEOUT (5*MINUTE)

class CalControl
{
public:
    CalControl(Session *s);
    ~CalControl();

    void Run();

    void Rescan();

    void ReadAppointments();
    void AppointmentMaker();

    void Quit();

protected:
    void UpdatePersonal();
    void UpdateBrowser();
    char* MakeEntry(ArjunaName *server);

private:
    Session *session;
    StringBrowser *location_browser;
    ServerList *servers;
    ApplicationWindow *window;
    Glyph *user_label;
    Glyph *next_appointment_label;
    Patch *personal_patch;
    time_t time_of_last_rescan;
    int enable_quit;
};

ActionCallbackdeclare(CalControl);
