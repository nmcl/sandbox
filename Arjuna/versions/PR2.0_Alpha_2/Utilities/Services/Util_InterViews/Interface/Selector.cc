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
#include <InterViews/margin.h>
#include <InterViews/glyph.h>
#include <InterViews/center.h>
#include <InterViews/patch.h>
#include <InterViews/glue.h>
#include <InterViews/world.h>
#include <InterViews/session.h>
#include <InterViews/background.h>
#include <IV-look/field.h>
#include <IV-look/kit.h>
#include <IV-look/button.h>
#include <IV-look/telltale.h>
#include <IV-look/menu.h>

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#include <System/fstream.h>
#endif

#ifndef ARJNAME_H_
#include <ArjServers/ArjName.h>
#endif

#ifndef CLIENTRPC_H_
#include <StubGen/ClientRpc.h>
#endif

#ifndef RPCCONTROL_H_
#include <StubGen/RpcControl.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#include "Interface_stub.h"
#include "Transporter_stub.h"

#define INIT_
#include "Selector.h"
#include "Common.h"
#include "Message.h"
#include "Window.h"
#include "Menu.h"

extern State *head, *ptr;

Selector::Selector (Patch* body, FieldEditor *name, char *called, ScrollingText *w1) : Listener(body, this)
{
    w = w1;
    desc = called;
    fld = name;
    button(true, Event::any);  // register event handler
    cmt = new Commit(Selector::retrieve, this);
    cnl = new Commit(Selector::cancel);
}

Selector::~Selector ()
{
    // do nothing
}

void Selector::event (Event &e)
{
    if (e.type() == Event::down)  // catch relevant event and process it
    {
	if (pw == 0)
	{
	    State *marker = my_state();
	    char* location = new char[strlen(fld->text())+1];
	    memset(location, '\0', strlen(fld->text()));
	    memcpy(location, fld->text(), strlen(fld->text()));
	    memset(location+strlen(fld->text()), '\0', 1);
	    Session *s = World::current()->session();
	    Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), cmt);
	    Button *b2 = Kit::instance()->simple_push_button(" Cancel ", s->style(), cnl);
	    Menu *m = Kit::instance()->menubar(s->style());
	    m->add_item(b1->telltale(), b1->action());
	    m->add_item(b2->telltale(), b2->action());
	    Glyph *hspace = new HGlue(15.0);
	    LRBox *bt = new LRBox(hspace, hspace, m, hspace);
	    TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
					  World::current()->foreground(), 1.0));
	    Patch *p1 = new Patch(new VCenter(Message(World::current()->font(),
						      World::current()->foreground(),
						      "Place selected header file in:"), 1.0));
	    Patch *p2 = new Patch(new VCenter(Message(World::current()->font(),
						      World::current()->foreground(),
						      location), 1.0));
	    Patch *p3 = new Patch(new VCenter(Message(World::current()->font(),
						      World::current()->foreground(),
						      "Servers on"), 1.0));
	    Patch *p4 = new Patch(new VCenter(Message(World::current()->font(),
						      World::current()->foreground(),
						      marker->servers), 1.0));
	    TBBox *cb = new TBBox(p1, p2, p3, p4, bts);
	    TBBox *mn = new TBBox(new Border(new Margin(cb, 2.0),
					     World::current()->foreground(), 1.0));
	    Background *bg2 = new Background(mn, World::current()->background());
	    pw = new PopupWindow(bg2);
	    pw->place(400, 400);
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
    ofstream outFile(me->fld->text(), ios::out);
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

	buff.pack(marker->pathname);
	
	ArjunaName AN;
	AN.SetServiceName("Transporter");
	AN.SetHostName(marker->hostname);

	ClientRpc *RPCClient = new ClientRpc(&AN);
	RpcControl *RPCCont = new RpcControl(RPCClient);

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
		    Session *s = World::current()->session();
		    Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), me->cnl);
		    Menu *m = Kit::instance()->menubar(s->style());
		    m->add_item(b1->telltale(), b1->action());
		    Glyph *hspace = new HGlue(15.0);
		    LRBox *bt = new LRBox(hspace, hspace, m, hspace);
		    TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
						      World::current()->foreground(), 1.0));
		    Patch *p1 = new Patch(new VCenter(Message(World::current()->font(),
							      World::current()->foreground(),
							      "Error! Could not find header."), 1.0));
		    TBBox *bx = new TBBox(p1, bts);
		    TBBox *by = new TBBox(new Border(new Margin(bx, 2.0),
						     World::current()->foreground(), 1.0));
		    Background *bg1 = new Background(by, World::current()->background());
		    error = new PopupWindow(bg1);
		    error->place(400, 600);
		    error->map();
		}
	    
		World::current()->RingBell(0);
	    }
	}
    }
    Selector::pw->unmap();
    Selector::pw = 0;
}





