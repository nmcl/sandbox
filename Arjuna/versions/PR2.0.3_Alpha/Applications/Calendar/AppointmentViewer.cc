/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: AppointmentViewer.cc,v 1.5 1993/09/21 15:23:20 nmcl Exp $
 */

// C++
#include <System/string.h>

#include <System/time.h>
#ifdef sun
extern "C" {
    // Sun version of time.h header file fails to define strftime
    int strftime(char *, int, const char *, const struct tm *);
}
#endif

// InterViews
#include <InterViews/background.h>
#include <InterViews/border.h>
#include <InterViews/box.h>
#include <InterViews/display.h>
#include <InterViews/event.h>
#include <IV-X11/xevent.h>
#include <InterViews/glue.h>
#include <InterViews/glyph.h>
#include <InterViews/margin.h>
#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/window.h>
#include <IV-look/2.6/InterViews/button.h>
#include <IV-look/2.6/InterViews/scroller.h>
#include <IV-look/2.6/InterViews/strbrowser.h>
#include <IV-look/button.h>
#include <IV-look/mono_kit.h>
#include <OS/leave-scope.h>

// Arjuna
#ifdef DEBUG
#include <Common/Debug.h>
#endif
#include <Arjuna/AtomicA.h>

// application
#include "Calendar_stub.h"
#include "AppointmentViewer.h"
#include "MakeAppointment.h"

#include <StubGen/ClientRpc.h>

#define START_FIELD    20
#define END_FIELD      8
#define NOT_CONFIRMED  "* "

ActionCallbackimplement(AppointmentViewer);

AppointmentViewer::AppointmentViewer(Session *s, Window *p, ArjunaName *u, Boolean owner)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "AppointmentViewer::AppointmentViewer(Session *s, Window *p, ArjunaName *u, Boolean owner)\n" << flush;
#endif

    int res;

    session  = s;
    parent   = p;
    is_owner = owner;
    enable_buttons = 0;

    res = 0;
    ClientRpc  *RPCClient = new ClientRpc(u);
    RpcControl *RPCCtl    = new RpcControl(RPCClient);
    calendar  = new Calendar(res, u, RPCCtl);
    if (res <= 0)
    {
	delete calendar;
	calendar = NULL;
    }
}

     
AppointmentViewer::~AppointmentViewer()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "AppointmentViewer::~AppointmentViewer()\n" << flush;
#endif

    delete viewer;
    delete calendar;
}


void AppointmentViewer::run()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void AppointmentViewer::run()\n" << flush;
#endif

    if (calendar != NULL)
    {
	make_viewer();
	scan_appointments();
	viewer->map();
	flag = 0;
	while (flag == 0)
	{
	    Event event;
	    
	    event.read();
	    event.handle();
	}
	viewer->unmap();
    }
}


void AppointmentViewer::make_viewer()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void AppointmentViewer::make_viewer()\n" << flush;
#endif
    char* user_name = NULL;
    int res;

    Style *style = session->style();
    Kit *kit = MonoKit::instance();

    Glyph *title_label = kit->label("Appointments for : ", style);

    res = 0;
    calendar->GetUserName(res, user_name);
    if (res <= 0)
	user_name = "(name not available)";

    Glyph *title_name  = kit->label(user_name, style);
    Glyph *title = new LRBox(title_label, title_name, new HGlue());

    ButtonState *browser_state = new ButtonState(0);
    browser = new StringBrowser(browser_state, 10, 64, true);
    Glyph *margined_browser = new Margin(browser, 3.0);
    Glyph *bordered_browser = new Border(margined_browser, style->foreground(), 1.0);

#include <InterViews/2.6/_enter.h>
    Scroller *v_scroller = new VScroller(browser);
#include <InterViews/2.6/_leave.h>
    Glyph *margined_scroller = new Margin(v_scroller, 3.0);
    Glyph *bordered_scroller = new Border(margined_scroller, style->foreground(), 1.0);

    Glyph *browser_box = new LRBox(bordered_browser, bordered_scroller);

    Action *exit_action = new AppointmentViewer_ActionCallback(this, AppointmentViewer::exit_viewer);
    Glyph *exit_label = kit->label("Done", style);
    Glyph *exit_button = kit->push_button(exit_label, style, exit_action);

    Action *add_action = new AppointmentViewer_ActionCallback(this, AppointmentViewer::add_appointment);
    Glyph *add_label = kit->label("Add Appointment", style);
    Glyph *add_button = kit->push_button(add_label, style, add_action);

    Action *rescan_action = new AppointmentViewer_ActionCallback(this, AppointmentViewer::scan_appointments);
    Glyph *rescan_label = kit->label("Rescan", style);
    Glyph *rescan_button = kit->push_button(rescan_label, style, rescan_action);
    
    Glyph *hspace = new HGlue(2.0, 0.0, 0.0);

    Glyph *buttons_box;

    if (is_owner)
    {

	Action *delete_action = new AppointmentViewer_ActionCallback(this, AppointmentViewer::delete_appointment);
	Glyph *delete_label = kit->label("Delete Appointment", style);
	Glyph *delete_button = kit->push_button(delete_label, style, delete_action);

	Action *confirm_action = new AppointmentViewer_ActionCallback(this, AppointmentViewer::confirm_appointment);
	Glyph *confirm_label = kit->label("Confirm", style);
	Glyph *confirm_button = kit->push_button(confirm_label, style, confirm_action);

	buttons_box = new LRBox(add_button, hspace, delete_button, hspace, confirm_button, hspace, rescan_button, new HGlue());
	buttons_box->append(exit_button);
    }
    else
	buttons_box= new LRBox(add_button, hspace, rescan_button, new HGlue(), exit_button);

    Glyph *actives = new TBBox(title, new VGlue(4.0), browser_box, new VGlue(3.0), buttons_box);
    Glyph *margined_actives = new Margin(actives, 4.0);
    Glyph *contents = new Background(margined_actives, style->background());

    viewer = new TopLevelWindow(contents);
    viewer->name("Appointments Viewer");
}


void AppointmentViewer::exit_viewer()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void AppointmentViewer::exit_viewer()\n" << flush;
#endif

    if (enable_buttons == 0)
	flag = 1;
    else
	session->default_display()->ring_bell(100);
}


void AppointmentViewer::scan_appointments()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void AppointmentViewer::scan_appointments()\n" << flush;
#endif

    int res;
    AtomicAction A;
    int no_appointments;

    browser->Clear();

    A.Begin();
    
    res = 0;
    calendar->AppointmentCount(res, no_appointments);
    if (res > 0)
    {
	AppointmentEntry entry;
	entry.description = "";

	int i;

	for(i = 0; i < no_appointments; i++)
	{
	    res = 0;
	    calendar->GetAppointment(res, i, entry);
	    if (res > 0)
	    {
		struct tm *app_start_time, *app_end_time;

		// note that localtime returns a pointer to a static structure
		// which will get overwritten by next usage.

		char* start_string = new char[START_FIELD];
		app_start_time = localtime(&entry.start);
		strftime(start_string,  START_FIELD, "%d/%b/%y %H:%M ", app_start_time);

		char* end_string   = new char[END_FIELD];
		app_end_time   = localtime(&entry.end);
		strftime(end_string, END_FIELD, "%H:%M ", app_end_time);
		
		char* app_string = new char[strlen(start_string) + strlen(end_string) + strlen(NOT_CONFIRMED) + strlen(entry.description) + 1];
		strcpy(app_string, start_string);
		strcat(app_string, end_string);
		if (!entry.confirmed)
		    strcat(app_string, NOT_CONFIRMED);
		strcat(app_string, entry.description);
		browser->Append(app_string);
		
		delete start_string;
		delete end_string;
		delete app_string;
	    }
	    else
		browser->Append("(this appointment entry unavailable)");
	}
    }

    A.End();
}


void AppointmentViewer::add_appointment()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void AppointmentViewer::add_appointment()\n" << flush;
#endif

    if (enable_buttons == 0)
    {
	AppointmentEntry *entry;
	
	enable_buttons++;
	MakeAppointment maker(session, viewer);
	entry = maker.run();
	if (entry != NULL)
	{
	    int res;

	    if (is_owner)
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
		debug_stream << "AppointmentViewer::add_appointment -> adding own appointment\n" << flush;
#endif
		entry->confirmed = TRUE;
	    }
	    else
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
                debug_stream << "AppointmentViewer::add_appointment -> adding appointment to other user\n" << flush;
#endif
		entry->confirmed = FALSE;
	    }
	    res = 0;
	    calendar->AddAppointment(res, *entry);
	    if (res < 1)
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
	        debug_stream << "AppointmentViewer::add_appointment -> failed to add appointment\n" << flush;
#endif
		session->default_display()->ring_bell(100);
	    }
	    scan_appointments();
	}
	enable_buttons--;
    }
    else
	session->default_display()->ring_bell(100);
}


void AppointmentViewer::delete_appointment()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void AppointmentViewer::delete_appointment()\n" << flush;
#endif

    if (enable_buttons == 0)
    {
	if (calendar != NULL)
	{
	    if (browser->Selections() > 0)
	    {
		int appointment_number = browser->Selection(0);
		int res;

		res = 0;
		calendar->DelAppointment(res, appointment_number);
		if (res > 0)
		    scan_appointments();
	    }
	}
    }
    else
	session->default_display()->ring_bell(100);
}


void AppointmentViewer::confirm_appointment()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void AppointmentViewer::confirm_appointment()\n" << flush;
#endif

    if (enable_buttons == 0)
    {
	if (calendar != NULL)
	{
	    if (browser->Selections() > 0)
	    {
		int appointment_number = browser->Selection(0);
		int res;
		AppointmentEntry entry;
		entry.description = "";
		AtomicAction A;

		A.Begin();
		res = 0;
		calendar->GetAppointment(res, appointment_number, entry);
		if (res > 0)
		{
		    res = 0;
		    calendar->DelAppointment(res, appointment_number);
		    if (res > 0)
		    {
			entry.confirmed = TRUE;
			res = 0;
			calendar->AddAppointment(res, entry);
			if (res > 0)
			{
			    if (A.End() == COMMITTED)
				scan_appointments();
			}
			else
			    A.Abort();
		    }
		    else
			A.Abort();
		}
		else
		    A.Abort();
	    }
	}
    }
    else
	session->default_display()->ring_bell(100);
}

