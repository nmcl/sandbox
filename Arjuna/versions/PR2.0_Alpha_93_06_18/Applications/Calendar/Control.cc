/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Control.cc,v 1.5 1993/09/21 15:23:24 nmcl Exp $
 */

 // C++
#include <System/string.h>

// InterViews
#include <InterViews/action.h>
#include <InterViews/background.h>
#include <InterViews/border.h>
#include <InterViews/box.h>
#include <InterViews/display.h>
#include <InterViews/event.h>
#include <InterViews/glue.h>
#include <InterViews/margin.h>
#include <InterViews/patch.h>
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
#include <RPC/ArjServers/ArjName.h>

// application
#include "Alert.h"
#include "AppointmentViewer.h"
#include "Calendar_stub.h"
#include "Control.h"
#include "MakeAppointment.h"
#include "ServerList.h"

#include <StubGen/ClientRpc.h>

// strings used in formatting entries in name-location browser
// note that SPACES must contain at least as many spaces as NAME_FIELD_SIZE
#define PADDING " : "
#define SPACES "                         "

// field sizes used in the formatting of the name-location browser
#define NAME_FIELD_SIZE     25
#define PADDING_FIELD_SIZE  3


ActionCallbackimplement(CalControl);

CalControl::CalControl(Session *s)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::CalControl(Session *s)\n" << flush;
#endif

    session = s;
    enable_quit = 0;

    Style *style = session->style();

    Kit *kit = MonoKit::instance();
    
    user_label = new LRBox(kit->label("Could not locate/contact your calendar server", style), new HGlue());

    next_appointment_label = new TBBox(kit->label("", style));
    Glyph *next_appointment_title = kit->label("Next appointment : ", style);
    Glyph *appointment_box = new LRBox(next_appointment_title, next_appointment_label, new HGlue());

    Glyph *personal_box =  new TBBox(user_label, appointment_box);
    personal_patch = new Patch(personal_box);
    Glyph *hspace0 =  new HGlue();
    Glyph *spaced_personalbox = new LRBox(personal_patch);
    Glyph *personal_info_box = new BMargin(spaced_personalbox, 3.0);
    
    ButtonState *browser_state = new ButtonState(0);
    location_browser = new StringBrowser(browser_state, 8, 64, false);  // 8 lines x 64 chars
    Glyph *margined_location = new Margin(location_browser, 3.0);
    Glyph *bordered_location = new Border(margined_location, style->foreground(), 1.0);

#include <InterViews/2.6/_enter.h>
    Scroller *v_scroller = new VScroller(location_browser);
#include <InterViews/2.6/_leave.h>

    Glyph *margined_scroller = new Margin(v_scroller, 3.0);
    Glyph *bordered_scroller = new Border(margined_scroller, style->foreground(), 1.0);

    Glyph *location_box = new LRBox(bordered_location, bordered_scroller);

    Action *readappointment_action = new CalControl_ActionCallback(this, CalControl::ReadAppointments);
    Glyph *readappointment_label = kit->label("Read Appointments", style);
    Glyph *readappointment_button = kit->push_button(readappointment_label, style, readappointment_action);
    HGlue *hspace1 = new HGlue(2.0, 0.0, 0.0);

    Action *makeappointment_action = new CalControl_ActionCallback(this, CalControl::AppointmentMaker);
    Glyph *makeappointment_label = kit->label("Make Appointment", style);
    Glyph *makeappointment_button = kit->push_button(makeappointment_label, style, makeappointment_action);
    HGlue *hspace2 = new HGlue(2.0, 0.0, 0.0);
    
    Action *rescan_action = new CalControl_ActionCallback(this, CalControl::Rescan);
    Glyph *rescan_label = kit->label("Rescan", style);
    Glyph *rescan_button = kit->push_button(rescan_label, style, rescan_action);
    HGlue *hspace5 = new HGlue(8.0);
    
    Action *quit_action = new CalControl_ActionCallback(this, CalControl::Quit);
    Glyph *quit_label = kit->label("Quit", style);
    Glyph *quit_button = kit->push_button(quit_label, style, quit_action);
    
    Glyph *buttons_box = new LRBox(readappointment_button, hspace1,
				   makeappointment_button, hspace2,
				   rescan_button, hspace5,
				   quit_button);
    Glyph *margined_buttons = new TMargin(buttons_box, 3.0);

    Glyph *actives = new TBBox(personal_info_box, location_box, margined_buttons);
    Glyph *bordered = new Margin(actives, 4.0);
    Glyph *contents = new Background(bordered, style->background());

    window = new ApplicationWindow(contents);
    window->map();

    servers = new ServerList;

    UpdateBrowser();
    UpdatePersonal();

    time_of_last_rescan = time(NULL);    // initialize time of last rescan to now

    Run();
}


CalControl::~CalControl()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::~CalControl()\n" << flush;
#endif

    delete servers;
    delete window;
}


void CalControl::Run()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::Run()\n" << flush;
#endif

    while (!session->done())
    {
	Event event;
	time_t timenow;
	
	if (event.read(MINUTE, 0))       // guarantee that after 1 minute, the call returns
	    event.handle();

	timenow = time(NULL);
	if ((timenow - time_of_last_rescan) >= RESCAN_TIMEOUT)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
	    debug_stream << "CalControl::Run -> time to do another rescan\n" << flush;
#endif
	    Rescan();
	}
    }
}


void CalControl::Rescan()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::Rescan()\n" << flush;
#endif

    // update last rescan time
    time_of_last_rescan = time(NULL);

    // first update selected/all servers as requested
    int no_selected, no_servers;
    int i;

    no_selected = location_browser->Selections();
    if (no_selected == 0)
    {
	// no selections made -> complete update & rescan
	servers->ScanNameServer();
	UpdateBrowser();
    }
    else
    {
	// some selections made -> update only selected entries
	no_servers = servers->Count();

	for(i = 0; i < no_servers; i++)
	{
	    if (location_browser->Selected(i))
	    {
		char* entry = NULL;

		entry = MakeEntry(servers->Get(i));
		if (entry != NULL)
		{
		    location_browser->Remove(i);
		    location_browser->Insert(entry, i);
		    location_browser->Sync();
		    delete entry;
		}
		else
		{
		    // server cannot be accessed -> remove entry
		    location_browser->Remove(i);
		    servers->Delete(i);
		}
	    }
	}
    }

    // now update personal information
    UpdatePersonal();
}


void CalControl::ReadAppointments()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::ReadAppointments()\n" << flush;
#endif

    if (location_browser->Selections() > 1)
    {
	session->default_display()->ring_bell(100);
	Kit *kit = MonoKit::instance();
	Style *style = session->style();
	Glyph *t1 = kit->label("ERROR :", style);
	Glyph *t2 = kit->label("  Can only view one person's", style);
	Glyph *t3 = kit->label("  appointments at a time.", style);
	Glyph *msg = new TBBox(t1, t2, t3);
	Glyph *btn = kit->label(" OK ", style);
	Alert alert(session, window, msg, btn);
	alert.run();
    }
    else
    {
	ArjunaName *user = NULL;
	Boolean is_owner;

	// if no selections -> view user's own appointments
	// else view appointments for selected user
	if (location_browser->Selections() == 0)
	{
	    is_owner = TRUE;
	    user = servers->GetMyServer();
	}
	else
	{
	    is_owner = FALSE;
	    user = servers->Get(location_browser->Selection(0));
	}

	if (user == NULL)
	{
	    session->default_display()->ring_bell(100);
	    Kit *kit = MonoKit::instance();
	    Style *style = session->style();
	    Glyph *t1 = kit->label("ERROR :", style);
	    Glyph *t2 = kit->label("  Cannot locate requested server.", style);
	    Glyph *msg = new TBBox(t1, t2);
	    Glyph *btn = kit->label(" OK ", style);
	    Alert alert(session, window, msg, btn);
	    alert.run();
	}
	else
	{
	    enable_quit++;
	    AppointmentViewer viewer(session, window, user, is_owner);
	    viewer.run();
	    enable_quit--;
	}
    }
}


void CalControl::AppointmentMaker()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::AppointmentMaker()\n" << flush;
#endif

    AppointmentEntry *entry;

    MakeAppointment maker(session, window);
    entry = maker.run();
    if (entry != NULL)
    {
	int number_of_servers = location_browser->Selections();
	int i;
	int res;
	Boolean success;

	success = TRUE;					    // so far no problems

	// create array of servers, one per selection
	typedef Calendar *Calendar_Ptr;

	Calendar_Ptr *server_array;
	server_array = new Calendar_Ptr[number_of_servers + 1];
	for(i = 0; i < (number_of_servers+1); i++)
	    server_array[i] = NULL;

	// 1st array element -> user's own server
	ArjunaName *serverAN = servers->GetMyServer();
	if (serverAN != NULL)
	{
	    res = 0;
	    ClientRpc *RPCClient = new ClientRpc(serverAN);
	    RpcControl *RPCCtl   = new RpcControl(RPCClient);
	    server_array[0]      = new Calendar(res, serverAN, RPCCtl);
	    if (res < 1)
		success = FALSE;			    // could not create own server
	}

	if (success)
	{
	    for(i = 0; i < number_of_servers; i++)	    // create servers for all other selected entries
	    {
		serverAN = servers->Get(location_browser->Selection(i));
		res = 0;
		ClientRpc *RPCClient = new ClientRpc(serverAN);
		RpcControl *RPCCtl   = new RpcControl(RPCClient);
		server_array[i+1]    = new Calendar(res, serverAN, RPCCtl);
		if (res < 1)
		{
		    success = FALSE;			    // could not create requested server
		    break;
		}
	    }
	}

	if (success)
	{
	    AtomicAction A;

	    success = TRUE;
	    A.Begin();

	    for(i = 0; i < (number_of_servers+1); i++)
	    {
		if (i == 0)
		    entry->confirmed = TRUE;
		else
		    entry->confirmed = FALSE;

		res = 0;
		server_array[i]->AddAppointment(res, *entry);
		if (res < 1)
		{
		    success = FALSE;
		    break;
		}
	    }


	    if (success)				    // if all operations successfull, commit results
		A.End();
	    else
		A.Abort();
	}

	for(i = 0; i < (number_of_servers+1); i++)
	{
	    if (server_array[i] != NULL)
		delete server_array[i];
	}
	delete server_array;
    }
}


void CalControl::Quit()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void CalControl::Quit()\n" << flush;
#endif

    if (enable_quit == 0)
	session->quit();
    else
	session->default_display()->ring_bell(100);
}


void CalControl::UpdatePersonal()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "CalControl::UpdatePersonal()\n" << flush;
#endif

    char* user_entry = NULL;
    char* next_app   = NULL;
    char* appointment_entry = NULL;
    ArjunaName *my_server = servers->GetMyServer();

    if (my_server != NULL)
    {
	int res = 0;
	ClientRpc  *RPCClient = new ClientRpc(my_server);
	RpcControl *RPCCtl    = new RpcControl(RPCClient);
	Calendar   *calendar  = new Calendar(res, my_server, RPCCtl);
	if (res > 0)
	{
	    AtomicAction A;
	    char* user_name;
	    char* location;

	    A.Begin();

	    user_name = NULL;
	    location  = NULL;
	    res = 0;
	    calendar->GetNameLocation(res, time(NULL), user_name, location);
	    if (res > 0)
	    {
		user_entry = new char[ strlen(user_name) + PADDING_FIELD_SIZE + strlen(location) + 1];
		strcpy(user_entry, user_name);
		strcat(user_entry, PADDING);
		strcat(user_entry, location);
	    }

	    AppointmentEntry entry;
	    entry.description = NULL;

	    res = 0;
	    calendar->GetNextAppointment(res, time(NULL), entry);
	    if (res > 0)
	    {
		if (entry.start == 0)
		    next_app = "";
		else
		{
		    next_app = entry.description;
		}
	    }

	    A.End();
	}
	delete calendar;
    }

    if (user_entry == NULL)
	user_entry = "Could not locate/contact your calendar server";
    if (next_app == NULL)
	next_app = "";

    Kit *kit = MonoKit::instance();
    Glyph *user_info = kit->label(user_entry, session->style());
    Glyph *app_info  = kit->label(next_app, session->style());
    user_label->replace(0, user_info);
    next_appointment_label->replace(0, app_info);

    personal_patch->reallocate();
    personal_patch->redraw();
}


void CalControl::UpdateBrowser()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "CalControl::UpdateBrowser()\n" << flush;
#endif

    int i, no_servers;

    location_browser->Clear();
    no_servers = servers->Count();
    i = 0;
    while (i < no_servers)
    {
	char* entry = NULL;

	entry = MakeEntry(servers->Get(i));

	if (entry != NULL)
	{
	    location_browser->Append(entry);
	    delete entry;
	}
	else
	{
	    // RPC invocation failed (denoted by NULL entry)
	    servers->Delete(i);
	    no_servers--;
	}
	i++;
    }

#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::UpdateBrowser -> memory map after UpdateBrowser completed\n" << flush;
#endif

}


char* CalControl::MakeEntry(ArjunaName *server)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "CalControl::MakeEntry(ArjunaName *server)\n" << flush;
#endif

    char* entry = NULL;
    int res;

    if (server != NULL)
    {
	res = 0;
	ClientRpc  *RPCClient = new ClientRpc(server);
	RpcControl *RPCCtl    = new RpcControl(RPCClient);
	Calendar   *calendar  = new Calendar(res, server, RPCCtl);

	if (res != 0)
	{
	    AtomicAction A;
	    char* user_name;
	    char* location;
	    
	    A.Begin();

	    user_name = NULL;
	    location = NULL;
	    res = 0;
	    calendar->GetNameLocation(res, time(NULL), user_name, location);
	    if (res > 0)
	    {
		char* formatted_user_name = new char[ NAME_FIELD_SIZE + 1 ];
		
		if (strlen(user_name) < NAME_FIELD_SIZE)
		{
		    int extra;
		    extra = NAME_FIELD_SIZE - strlen(user_name);
		    strcpy(formatted_user_name, user_name);
		    strncat(formatted_user_name, SPACES, extra);
		    formatted_user_name[NAME_FIELD_SIZE] = '\0';
		}
		else
		{
		    strncpy(formatted_user_name, user_name, NAME_FIELD_SIZE);
		    formatted_user_name[NAME_FIELD_SIZE] = '\0';
		}
		
		entry = new char[ NAME_FIELD_SIZE + PADDING_FIELD_SIZE + strlen(location) + 1];
		strcpy(entry, formatted_user_name);
		strcat(entry, PADDING);
		strcat(entry, location);
	    }
	    
	    A.End();
	}
	
	delete calendar;
    }
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
    debug_stream << "CalControl::MakeEntry -> entry = " << entry << "\n" << flush;
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "CalControl::MakeEntry -> memory map after MakeEntry completed\n" << flush;
#endif

    return entry;
}
