/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: MakeAppointment.h,v 1.2 1993/03/18 13:50:52 nmcl Exp $
 */

#include <InterViews/action.h>
#include <OS/leave-scope.h>


class Button;
class FieldEditor;
class Session;
class TransientWindow;
class Window;
struct AppointmentEntry;

class MakeAppointment
{
public:
    MakeAppointment(Session *s, Window *p);
    ~MakeAppointment();

    AppointmentEntry *run();

protected:
    void make_dialog();
    
    void exit_dialog();
    void cancel_dialog();
    
    void date_done();
    void start_done();
    void end_done();
    void description_done();

    int parse_date(const char*, struct tm*);
    int parse_time(const char*, struct tm*);

private:
    Session *session;
    Window *parent;
    TransientWindow *dialog;
    Button *done_button;
    Button *cancel_button;
    FieldEditor *date_editor;
    FieldEditor *start_editor;
    FieldEditor *end_editor;
    FieldEditor *description_editor;
    AppointmentEntry *entry;
    int flag;
};

ActionCallbackdeclare(MakeAppointment);
