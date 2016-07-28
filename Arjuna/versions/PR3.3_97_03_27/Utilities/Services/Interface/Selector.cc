/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#include <InterViews/window.h>
#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/label.h>
#include <InterViews/border.h>
#include <InterViews/glyph.h>
#include <InterViews/patch.h>
#include <InterViews/glue.h>
#include <InterViews/session.h>
#include <InterViews/background.h>
#include <InterViews/event.h>
#include <InterViews/layout.h>
#include <InterViews/style.h>
#include <IV-look/field.h>
#include <IV-look/kit.h>
#include <IV-look/button.h>
#include <IV-look/telltale.h>
#include <IV-look/menu.h>
#include <OS/string.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#include "Interface_stub.h"
#include "Transporter_stub.h"

#include "Selector.h"
#include "Message.h"
#include "Window.h"
#include "Menu.h"

extern State *head, *ptr;

class ApplicationWindow *Selector::pw = 0;
class ApplicationWindow *Selector::error = 0;


Selector::Selector (FieldEditor *name, char *called) : InputHandler(0, Session::instance()->style())
{
    desc = called;
    fld = name;
    cmt = new Commit(Selector::retrieve, this);
    cnl = new Commit(Selector::cancel);
    body(new Label(called, WidgetKit::instance()->font(), WidgetKit::instance()->foreground()));
}

Selector::~Selector ()
{
    body(0);
}

void Selector::allocate (Canvas* c, const Allocation& a, Extension& e)
{
    body()->allocate(c, a, e);
}

void Selector::draw (Canvas* c, const Allocation& a) const
{
    body()->draw(c, a);
}

void Selector::press (const Event &e)
{
    focus(this);
    
    if (e.type() == Event::down)  // catch relevant event and process it
    {
	if (pw == 0)
	{
	    State *marker = my_state();
	    char* location = new char[strlen((char*) (fld->text())->string())+1];
	    ::strcpy(location, (fld->text())->string());
	    Button *b1 = WidgetKit::instance()->push_button(" OK ", cmt);
	    Button *b2 = WidgetKit::instance()->push_button(" Cancel ", cnl);
	    Menu *m = WidgetKit::instance()->menubar();
	    m->append_item(new MenuItem(b1, b1->state(), b1->action()));
	    m->append_item(new MenuItem(b2, b2->state(), b2->action()));
	    Glyph *hspace = LayoutKit::instance()->hglue(15.0);
	    Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
	    Glyph *bts = LayoutKit::instance()->vbox(new Border(bt,
								WidgetKit::instance()->foreground(), 1.0));
	    Patch *p1 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  "Place selected header file in:"));
	    Patch *p2 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  location));
	    Patch *p3 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  "Servers on"));
	    Patch *p4 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  marker->servers));
	    Glyph *cb = LayoutKit::instance()->vbox(p1, p2, p3, p4, bts);
	    Glyph *mn = LayoutKit::instance()->vbox(new Border(cb,
							       WidgetKit::instance()->foreground(), 1.0));
	    Background *bg2 = new Background(mn, WidgetKit::instance()->background());
	    pw = new ApplicationWindow(bg2);
	    pw->map();
	}
    }
}

void Selector::cancel ()
{
    if (Selector::pw != 0)
    {
	Selector::pw->unmap();
	Selector::pw = 0;
    }

    if (Selector::error != 0)
    {
	Selector::error->unmap();
	Selector::error = 0;
    }
}

State *Selector::my_state ()
{
    State *marker = head;
    boolean ok = FALSE;
    int id = 1;

    while ((marker != 0) && (!ok))
    {
	char pos[8];
	char* int_desc = new char[strlen(marker->interfaces)+8];

	memset(pos, '\0', 8);
	memset(int_desc, '\0', strlen(marker->interfaces)+8);
	sprintf(pos, "%d: ", id);
	memcpy(int_desc, pos, strlen(pos));
	memcpy(int_desc+strlen(pos), marker->interfaces, strlen(marker->interfaces));
	if (strcmp(int_desc, desc) != 0)
	{
	    id++;
	    marker = marker->next;
	}
	else
	    ok = TRUE;
    }

    return marker;
}

void Selector::retrieve (Selector *me)
{
    ofstream outFile((char*) (me->fld->text())->string(), ios::out);
    if (outFile)
    {
	State *marker = head;
	Buffer buff;
	int res = 0;
	boolean ok = FALSE;
	int id = 1;

	while ((marker != 0) && (!ok))
	{
	    char pos[8];
	    char* int_desc = new char[strlen(marker->interfaces)+8];

	    memset(pos, '\0', 8);
	    memset(int_desc, '\0', strlen(marker->interfaces)+8);
	    sprintf(pos, "%d: ", id);
	    memcpy(int_desc, pos, strlen(pos));
	    memcpy(int_desc+strlen(pos), marker->interfaces, strlen(marker->interfaces));
	    if (strcmp(int_desc, me->desc) != 0)
	    {
		id++;
		marker = marker->next;
	    }
	    else
		ok = TRUE;
	}

	buff.packMappedString(marker->pathname);
	
	ArjunaName AN;
	AN.setServiceName("Transporter");
	AN.setHostName(marker->hostname);

	ClientRpc *RPCClient = new ClientRpc(&AN);
	ClientRpcManager *RPCCont = new ClientRpcManager(RPCClient);

	Transporter T(res, RPCCont);
	if (res == 1)
	{
	    res = -1;
	    Buffer buff2;

	    buff2 = T.Get(buff, res);
	    if (res == 0)
	    {
		char* filedata = 0;

		buff2.unpack(filedata);
		outFile << filedata;

		delete filedata;
	    }
	    else
	    {
		if (error == 0)
		{
		    Button *b1 = WidgetKit::instance()->push_button(" OK ", me->cnl);
		    Menu *m = WidgetKit::instance()->menubar();
		    m->append_item(new MenuItem(b1, b1->state(), b1->action()));
		    Glyph *hspace = LayoutKit::instance()->hglue(15.0);
		    Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
		    Glyph *bts = LayoutKit::instance()->vbox(new Border(bt,
									WidgetKit::instance()->foreground(), 1.0));
		    Patch *p1 = new Patch(Message(WidgetKit::instance()->font(),
						  WidgetKit::instance()->foreground(),
						  "Error! Could not find header."));
		    Glyph *bx = LayoutKit::instance()->vbox(p1, bts);
		    Glyph *by = LayoutKit::instance()->vbox(new Border(bx,
								       WidgetKit::instance()->foreground(), 1.0));
		    Background *bg1 = new Background(by, WidgetKit::instance()->background());
		    error = new ApplicationWindow(bg1);
		    error->map();
		}
	    }
	}
    }
    Selector::pw->unmap();
    Selector::pw = 0;
}





