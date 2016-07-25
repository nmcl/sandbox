/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Alert.cc,v 1.2 1993/03/18 13:50:57 nmcl Exp $
 */

#include <InterViews/background.h>
#include <InterViews/box.h>
#include <IV-look/button.h>
#include <InterViews/event.h>
#include <IV-X11/xevent.h>
#include <InterViews/glue.h>
#include <InterViews/glyph.h>
#include <InterViews/handler.h>
#include <InterViews/margin.h>
#include <IV-look/mono_kit.h>
#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/window.h>

#include "Alert.h"

ActionCallbackimplement(Alert);

Alert::Alert(Session *s, Window *p, Glyph *msg, Glyph *btn_msg)
{
    session = s;
    parent = p;
    message = msg;
    message->ref();
    button_msg = btn_msg;
    button_msg->ref();
}


Alert::~Alert()
{
    message->unref();
    button_msg->unref();
    delete alert_window;
}


void Alert::run()
{
    make_alert();
    alert_window->map();
    flag = 0;
    while (flag == 0)
    {
	Event event;
	EventRep *xevent;

	session->read(event);
	xevent = event.rep();
	if (xevent->window_ == alert_window)
	{
	    alert_window->receive(event);
	    button->event(event);
	}
    }
    alert_window->unmap();
}


void Alert::make_alert()
{
    Glyph *hspace1 = new HGlue(2.0);
    Glyph *information = new LRBox(hspace1, message, hspace1);

    Kit *kit = MonoKit::instance();
    Action *btn_action = new Alert_ActionCallback(this, Alert::exit_alert);
    button = kit->push_button(button_msg, session->style(), btn_action);
    Glyph *hspace2 = new HGlue();
    Glyph *button_area = new LRBox(hspace2, button, hspace2);

    Glyph *vspace = new VGlue(8.0);
    Glyph *alertbox = new TBBox(information, vspace, button_area);
    Glyph *margined_alert = new Margin(alertbox, 4.0);
    Glyph *alert_contents = new Background(margined_alert, session->style()->background());

    alert_window = new TransientWindow(alert_contents);
    alert_window->transient_for(parent);
}

void Alert::exit_alert()
{
    flag = 1;
}
