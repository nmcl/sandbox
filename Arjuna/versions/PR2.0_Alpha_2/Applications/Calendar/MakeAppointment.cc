/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: MakeAppointment.cc,v 1.1 1993/11/03 12:22:44 nmcl Exp $
 */

// C++
#include <System/time.h>

#ifdef sun
extern "C" {
    // Sun version of time.h defines "timelocal" in place of "mktime".
    // -- note use of ifdef's arround the actual calls.
    time_t timelocal(const struct tm *);

    // Sun version of time.h fails to define "strftime"
    int strftime(char *, int, const char *, const struct tm *);
}
#endif

#include <System/string.h>
#include <System/iostream.h>
#include <System/strstream.h>

// InterViews
#include <InterViews/background.h>
#include <InterViews/border.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/display.h>
#include <InterViews/event.h>
#include <IV-X11/xevent.h>
#include <InterViews/font.h>
#include <InterViews/glue.h>
#include <InterViews/glyph.h>
#include <InterViews/label.h>
#include <InterViews/margin.h>
#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/window.h>
#include <IV-look/button.h>
#include <IV-look/field.h>
#include <IV-look/mono_kit.h>
#include <OS/leave-scope.h>


// Arjuna
#ifdef DEBUG
#include <Common/Debug.h>
#endif

// application
#include "Calendar_stub.h"
#include "MakeAppointment.h"

ActionCallbackimplement(MakeAppointment);


MakeAppointment::MakeAppointment(Session *s, Window *p)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "MakeAppointment::MakeAppointment(Session *s, Window *p)\n";
#endif

    session = s;
    parent  = p;
    entry = new AppointmentEntry;
    entry->start = 0;
    entry->end = 0;
    entry->description = NULL;
}


MakeAppointment::~MakeAppointment()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "MakeAppointment::~MakeAppointment()\n";
#endif

    delete dialog;
    delete entry;
}


AppointmentEntry *MakeAppointment::run()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "AppointmentEntry *MakeAppointment::run()\n";
#endif

    make_dialog();

    date_editor->field("DD/MM/YY");
    start_editor->field("HH:MM");
    end_editor->field("HH:MM");

    dialog->map();
    flag = 0;
    while (flag == 0)
    {
	Event event;

	event.read();
	event.handle();
    }
    dialog->unmap();
    return entry;
}


void MakeAppointment::make_dialog()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::make_dialog()\n";
#endif

    Style *style = session->style();
    Kit *kit = MonoKit::instance();

    Glyph *window_title = new HCenter(new Label("APPOINTMENT", new Font("6x13bold"), style->foreground()));
    Glyph *centered_title = new LRBox(new HGlue, window_title, new HGlue);

    Glyph *hspace = new HGlue(1.0, 0.0, 0.0);

    Glyph *date_label  = kit->label("Date : ", style);
    Action *date_action = new MakeAppointment_ActionCallback(this, MakeAppointment::date_done);
    date_editor = new FieldEditor(" DD/MM/YY ", style, date_action);
    Glyph *margined_date_editor = new Margin(date_editor, 2.0);
    Glyph *bordered_date_editor = new Border(margined_date_editor, style->foreground());
    Glyph *date_field = new LRBox(new VCenter(date_label), hspace, new VCenter(date_editor), new HGlue());
    
    Glyph *start_label = kit->label("Start : ", style);
    Action *start_action = new MakeAppointment_ActionCallback(this, MakeAppointment::start_done);
    start_editor = new FieldEditor(" HH:MM ", style, start_action);
    Glyph *margined_start_editor = new Margin(start_editor, 2.0);
    Glyph *bordered_start_editor = new Border(margined_start_editor, style->foreground());
    Glyph *start_field = new LRBox(new VCenter(start_label), hspace, new VCenter(start_editor), new HGlue());

    Glyph *end_label   = kit->label("End   : ", style);
    Action *end_action = new MakeAppointment_ActionCallback(this, MakeAppointment::end_done);
    end_editor = new FieldEditor(" HH:MM ", style, end_action);
    Glyph *margined_end_editor = new Margin(end_editor, 2.0);
    Glyph *bordered_end_editor = new Border(margined_end_editor, style->foreground());
    Glyph *end_field = new LRBox(new VCenter(end_label), hspace, new VCenter(end_editor), new HGlue());

    Glyph *description_label = kit->label("Description : ", style);
    Action *description_action = new MakeAppointment_ActionCallback(this, MakeAppointment::description_done);
    description_editor = new FieldEditor(" Description of appointment ", style, description_action);
    Glyph *margined_description_editor = new Margin(description_editor, 2.0);
    Glyph *bordered_description_editor = new Border(margined_description_editor, style->foreground());
    Glyph *description_field = new LRBox(new VCenter(description_label), hspace, new VCenter(description_editor), new HGlue());

    Action *done_action = new MakeAppointment_ActionCallback(this, MakeAppointment::exit_dialog);
    Glyph *done_label = kit->label("OK", style);
    done_button = kit->push_button(done_label, style, done_action);

    Action *cancel_action = new MakeAppointment_ActionCallback(this, MakeAppointment::cancel_dialog);
    Glyph *cancel_label = kit->label("Cancel", style);
    cancel_button = kit->push_button(cancel_label, style, cancel_action);

    Glyph *buttons = new LRBox(cancel_button, new HGlue(), done_button);

    Glyph *fields = new TBBox(date_field, new VGlue(2.0), start_field, new VGlue(1.0), end_field, new VGlue(2.0), description_field);
    Glyph *all = new TBBox(centered_title, new VGlue(5.0), fields, new VGlue(7.0), buttons);
    Glyph *contents = new Margin(all, 4.0);
    Glyph *dialog_contents = new Background(contents, style->background());

    dialog = new TransientWindow(dialog_contents);
    dialog->transient_for(parent);
}


void MakeAppointment::exit_dialog()
{
#ifdef DEBUG
   debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED; 
   debug_stream << "void MakeAppointment::exit_dialog()\n";
#endif

    // parse contents. if successful, flag = 1
    time_t now = time(NULL);
    struct tm *tmp_tm = localtime(&now);
    struct tm *date_tm = new struct tm;
    *date_tm = *tmp_tm;  // setup default entries (timezone etc)

    if (parse_date(date_editor->text(), date_tm) != 0)
    {
	delete date_tm;
	session->default_display()->ring_bell(100);
	date_editor->edit();
	return;
    }

    struct tm *start_tm = new struct tm;
    struct tm *end_tm   = new struct tm;
    *start_tm = *date_tm; // initialize date entries
    *end_tm   = *date_tm;

    if (parse_time(start_editor->text(), start_tm) != 0)
    {
	delete date_tm;
	delete start_tm;
	delete end_tm;
	session->default_display()->ring_bell(100);
	start_editor->edit();
	return;
    }

    if (parse_time(end_editor->text(), end_tm) != 0)
    {
	delete date_tm;
	delete start_tm;
	delete end_tm;
	session->default_display()->ring_bell(100);
	end_editor->edit();
	return;
    }
#ifdef sun
    time_t start_time = timelocal(start_tm);
    time_t end_time   = timelocal(end_tm);
#else
    time_t start_time = mktime(start_tm);
    time_t end_time   = mktime(end_tm);
#endif

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
    debug_stream << "MakeAppointment::exit_dialog -> start_time, end_time = " << start_time << ", " << end_time << "\n";
    debug_stream << "MakeAppointment::exit_dialog -> start_time = " << ctime(&start_time) << "\n";
    debug_stream << "MakeAppointment::exit_dialog ->   end_time = " << ctime(&end_time) << "\n";
#endif

    if (start_time >= end_time)
    {
	delete date_tm;
	delete start_tm;
	delete end_tm;
	session->default_display()->ring_bell(100);
	start_editor->edit();
	return;
    }

    entry->start = start_time;
    entry->end   = end_time;
    entry->description = new char[strlen(description_editor->text())];
    strcpy(entry->description, description_editor->text());

    flag = 1;
}


void MakeAppointment::cancel_dialog()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::cancel_dialog()\n";
#endif

    delete entry;
    entry = NULL;
    flag = 1;
}


void MakeAppointment::date_done()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::date_done()\n";
#endif

    if (parse_date(date_editor->text(), NULL) == 0)
    {
	start_editor->edit();
    }
    else
    {
	session->default_display()->ring_bell(50);
	date_editor->edit();
    }
}


void MakeAppointment::start_done()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::start_done()\n";
#endif

    if (parse_time(start_editor->text(), NULL) == 0)
    {
	end_editor->edit();
    }
    else
    {
	session->default_display()->ring_bell(50);
	start_editor->edit();
    }
}


void MakeAppointment::end_done()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::end_done()\n";
#endif

    if (parse_time(end_editor->text(), NULL) == 0)
    {
	description_editor->edit();
    }
    else
    {
	session->default_display()->ring_bell(50);
	end_editor->edit();
    }
}


void MakeAppointment::description_done()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::description_done()\n";
#endif

    date_editor->edit();
}


int MakeAppointment::parse_date(const char* date_str, struct tm *date_tm)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PROTECTED;
    debug_stream << "int MakeAppointment::parse_date(const char* date_str, struct tm *date_tm)\n";
#endif

    int day, month, year;
    char dummy;

    char* tmp = strdup(date_str);
    istrstream iss(tmp, strlen(tmp));
    iss >> day >> dummy >> month >> dummy >> year;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
    debug_stream << "int MakeAppointment::parse_date -> day/month/year = " << day << "/" << month << "/" << year << "\n";
#endif

    if ((year > 99) || (year < 1))
	return 1;

    if ((month > 12) || (month < 1))
	return 1;

    if ((day > 31) || (day < 1))
	return 1;

    if (date_tm != NULL)
    {
	date_tm->tm_mday = day;
	date_tm->tm_mon  = month - 1;
	date_tm->tm_year = year;
    }

    return 0;
}


int MakeAppointment::parse_time(const char* time_str, struct tm *time_tm)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
    debug_stream << "void MakeAppointment::parse_time(const char* time_str, struct tm *time_tm)\n";
#endif

    int hour, minute;
    char dummy;

    char* tmp = strdup(time_str);
    istrstream iss(tmp, strlen(tmp));
    iss >> hour >> dummy >> minute;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PROTECTED;
    debug_stream << "int MakeAppointment::parse_time -> hour:minute = " << hour << ":" << minute << "\n";
#endif

    if ((hour > 23) || (hour < 0))
	return 1;

    if ((minute > 59) || (minute < 0))
	return 1;

    if (time_tm != NULL)
    {
	time_tm->tm_sec  = 0;
	time_tm->tm_min  = minute;
	time_tm->tm_hour = hour;
    }

    return 0;
}
