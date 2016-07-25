/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: AppointmentViewer.h,v 1.1 1993/11/03 12:22:29 nmcl Exp $
 */

#include <InterViews/action.h>
#include <Common/Boolean.h>

class ArjunaName;
class Calendar;
class Session;
class StringBrowser;
class TransientWindow;
class Window;

class AppointmentViewer
{
public:
    AppointmentViewer(Session *s, Window *p, ArjunaName *u, Boolean owner);
    ~AppointmentViewer();

    void run();

protected:
    void make_viewer();
    void exit_viewer();
    void scan_appointments();
    void add_appointment();
    void delete_appointment();
    void confirm_appointment();

private:
    Calendar *calendar;
    Session  *session;
    Window   *parent;
    TopLevelWindow *viewer;
    StringBrowser *browser;
    ArjunaName *user;
    Boolean is_owner;
    int  flag;
    int  enable_buttons;
};

ActionCallbackdeclare(AppointmentViewer);
