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

#include <InterViews/world.h>
#include <InterViews/glue.h>
#include <InterViews/background.h>
#include <InterViews/box.h>
#include <InterViews/border.h>
#include <InterViews/margin.h>
#include <InterViews/session.h>
#include <IV-look/kit.h>
#include <IV-look/button.h>

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
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

#include "Interface_stub.h"

#define SET_
#include "Common.h"
#include "Menu.h"
#include "Message.h"
#include "Window.h"

extern State *head, *ptr;
extern void free_list();

foo::foo () {}

foo::~foo () {}

void foo::execute () { World::current()->quit(); }

Commit::Commit (void (*func)())
{
    x = func;
    y = 0;
    me = 0;
}

Commit::Commit (void (*func)(Selector*), Selector *who)
{
    y = func;
    x = 0;
    me = who;
}

Commit::~Commit() {}

void Commit::execute()
{
    if (me != 0)
	(y(me));
    else
	(x());
}

Entry::Entry (Interface *server)
{
    cmt = new Commit(Entry::work);
    cnl = new Commit(Entry::cancel);
    if (Entry::fault == 0)
	Entry::fault = new Commit(Entry::cancel);
    if (itf == 0)
	itf = server;
}

Entry::~Entry () {}

void Entry::Initialize ()
{
    memset(what, '\0', BUF_SIZE);
    memset(where, '\0', PATH);
    memset(machine, '\0', NAME);
}

void Entry::execute ()
{
    if (pw == 0)
    {
	Session *s = World::current()->session();
	desc = new FieldEditor("Interface description", s->style());
	path_l = new FieldEditor("header location", s->style());
	mac_n = new FieldEditor("machine name", s->style());
	servers = new FieldEditor("server locations", s->style());
	Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), cmt);
	Button *b2 = Kit::instance()->simple_push_button(" Cancel ", s->style(), cnl);
	Menu *m = Kit::instance()->menubar(s->style());
	m->add_item(b1->telltale(), b1->action());
	m->add_item(b2->telltale(), b2->action());
	Glyph *hspace = new HGlue(15.0);
	LRBox *bt = new LRBox(hspace, hspace, m, hspace);
	TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
					  World::current()->foreground(), 1.0));
	TBBox *bx = new TBBox(desc, path_l, mac_n, servers, bts);
	TBBox *by = new TBBox(new Border(new Margin(bx, 2.0),
					 World::current()->foreground(), 1.0));
	Background *bg1 = new Background(by, World::current()->background());
	pw = new TopLevelWindow(bg1);
	pw->place(400, 500);
	pw->name("Data Sheet");
	pw->map();
    }
}

void Entry::cancel ()
{
    if (Entry::pw != 0)
    {
	Entry::pw->unmap();
	Entry::pw = 0;
    }

    if (Entry::error != 0)
    {
	Entry::error->unmap();
	Entry::error = 0;
    }
}

void Entry::work ()
{
    int res;
    char* what = new char[strlen(Entry::desc->text())+1];
    char* where = new char[strlen(Entry::mac_n->text())+1];
    char* pth = new char[strlen(Entry::path_l->text())+1];
    char* srv = new char[strlen(Entry::servers->text())+1];

    memcpy(what, Entry::desc->text(), strlen(Entry::desc->text()));
    memcpy(where, Entry::mac_n->text(), strlen(Entry::mac_n->text()));
    memcpy(pth, Entry::path_l->text(),strlen(Entry::path_l->text()));
    memcpy(srv, Entry::servers->text(), strlen(Entry::servers->text()));
    memset(what+strlen(Entry::desc->text()), '\0', 1);
    memset(pth+strlen(Entry::path_l->text()), '\0', 1);
    memset(where+strlen(Entry::mac_n->text()), '\0', 1);
    memset(srv+strlen(Entry::servers->text()), '\0', 1);
    Entry::a = new State();
    Entry::a->Set_State(what, where, pth, srv);
    
    AtomicAction Z;
    Buffer buff;
    Boolean result;

    Z.Begin();

    buff.pack(what);
    buff.pack(where);
    buff.pack(pth);
    buff.pack(srv);

    res = -1;
    result = itf->Add(buff, res);

    if (res != 0)
    {
	Z.Abort();

	if (error == 0)
	{
	    Session *s = World::current()->session();
	    Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), Entry::fault);
	    Menu *m = Kit::instance()->menubar(s->style());
	    m->add_item(b1->telltale(), b1->action());
	    Glyph *hspace = new HGlue(15.0);
	    LRBox *bt = new LRBox(hspace, hspace, m, hspace);
	    TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
					      World::current()->foreground(), 1.0));
	    Patch *p1 = new Patch(new VCenter(Message(World::current()->font(),
                                                      World::current()->foreground(),
						      "Error! Could not add item."), 1.0));
	    Patch *p2 = 0;
	    if (res == -1)
		p2 = new Patch(new VCenter(Message(World::current()->font(),
						   World::current()->foreground(),
						   "Possible Lock Conflict."), 1.0));
	    TBBox *bx;
	    if (p2 == 0)
		bx = new TBBox(p1, bts);
	    else
		bx = new TBBox(p1, p2, bts);
	    TBBox *by = new TBBox(new Border(new Margin(bx, 2.0),
					     World::current()->foreground(), 1.0));
	    Background *bg1 = new Background(by, World::current()->background());
	    error = new PopupWindow(bg1);
	    error->place(400, 600);
	    error->map();
	}

	World::current()->RingBell(0);
    }
    else
	if (Z.End() != COMMITTED)
	    cerr << "Z.End() : Not Done\n";

    Entry::pw->unmap();
    Entry::pw = 0;
}

Scanner::Scanner (Interface *server)
{
    itf = server;
    buffer = 0;
    Scanner::a = new State();
    if (Scanner::fault == 0)
	Scanner::fault = new Commit(Scanner::cancel);
}

Scanner::~Scanner () {}

void Scanner::cancel ()
{
    if (Scanner::error != 0)
    {
	Scanner::error->unmap();
	Scanner::error = 0;
    }
}

void Scanner::add_body (ScrollingText *body) { stx = body; }

char *Scanner::contents ()
{
    int res, position = 0, size = 0;
    AtomicAction X;
    Boolean result = TRUE;

    if (buffer)
	delete [] buffer;

    buffer = new char[10240];
    memset(buffer, '\0', 10240);
    char *marker = buffer;
    char pos[8];
    free_list();
    
    X.Begin();

    do
    {
	AtomicAction Y;
	Buffer buff;
	char *interfaces, *hostname, *pathname, *servers;
	int u_id;

	a->Initialize();

	Y.Begin();

	res = -1;
	result = itf->Inspect(buff, position, res);

	if ((res == 0) && (result))
	{
	    buff.unpack(interfaces);
	    buff.unpack(hostname);
	    buff.unpack(pathname);
	    buff.unpack(servers);
	    buff.unpack(u_id);

	    a->Set_State(interfaces, hostname, pathname, servers, u_id);
	    
	    delete interfaces;
	    delete hostname;
	    delete pathname;
	    delete servers;

	    if (head == 0)
	    {
		head = new State(*a);
		ptr = head;
	    }
	    else
	    {
		ptr->Add(new State(*a));
		ptr = ptr->next;
	    }

	    memset(pos, '\0', 8);
	    sprintf(pos, "%d: ", a->u_id+1);
	    memcpy(marker, pos, strlen(pos));
	    marker+=strlen(pos);
	    memcpy(marker, a->interfaces, strlen(a->interfaces));
	    size += strlen(a->interfaces)+strlen(pos)+1;
	    marker+=strlen(a->interfaces);
	    *marker = '\n';
	    marker++;

	    if (Y.End() != COMMITTED)
		cerr << "Y.End() : Not Done\n";
	}
	else
	    Y.Abort();

    } while (result);

    if (res == -1)
    {
	if (error == 0)
	{
	    Session *s = World::current()->session();
	    Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), Scanner::fault);
	    Menu *m = Kit::instance()->menubar(s->style());
	    m->add_item(b1->telltale(), b1->action());
	    Glyph *hspace = new HGlue(15.0);
	    LRBox *bt = new LRBox(hspace, hspace, m, hspace);
	    TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
					      World::current()->foreground(), 1.0));
	    Patch *p1 = new Patch(new VCenter(Message(World::current()->font(),
                                                      World::current()->foreground(),
						      "Error! Cannot inspect database."), 1.0));
	    Patch *p2 = new Patch(new VCenter(Message(World::current()->font(),
						      World::current()->foreground(),
						      "Lock Conflict."), 1.0));
	    TBBox *bx = new TBBox(p1, p2, bts);
	    TBBox *by = new TBBox(new Border(new Margin(bx, 2.0),
					     World::current()->foreground(), 1.0));
	    Background *bg1 = new Background(by, World::current()->background());
	    error = new PopupWindow(bg1);
	    error->place(400, 600);
	    error->map();
	}

	World::current()->RingBell(0);
    }
    else
    {
	if (X.End() != COMMITTED)
	    cerr << "X.End() : Not Done\n";
    }

    buffer = (char*) realloc(buffer, size);
    return buffer;
}

void Scanner::execute ()
{
    char *txt = contents();
    stx->change_text(txt);
}

Delete::Delete (Interface *server)
{
    cmt = new Commit(Delete::work);
    cnl = new Commit(Delete::cancel);
    if (Delete::fault == 0)
	Delete::fault = new Commit(Delete::cancel);
    if (itf == 0)
	itf = server;
}

Delete::~Delete () {}

void Delete::execute ()
{
    if (pw == 0)
    {
	Session *s = World::current()->session();
	number = new FieldEditor("Interface number", s->style());
	Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), cmt);
	Button *b2 = Kit::instance()->simple_push_button(" Cancel ", s->style(), cnl);
	Menu *m = Kit::instance()->menubar(s->style());
	m->add_item(b1->telltale(), b1->action());
        m->add_item(b2->telltale(), b2->action());
        Glyph *hspace = new HGlue(15.0);
        LRBox *bt = new LRBox(hspace, hspace, m, hspace);
	TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
					  World::current()->foreground(), 1.0));
        TBBox *bx = new TBBox(number, bts);
        TBBox *by = new TBBox(new Border(new Margin(bx, 2.0),
                                         World::current()->foreground(), 1.0));
        Background *bg1 = new Background(by, World::current()->background());
        pw = new TopLevelWindow(bg1);
        pw->place(400, 500);
	pw->name("Interface To Delete");
	pw->map();
    }
}

void Delete::cancel ()
{
    if (Delete::pw != 0)
    {
	Delete::pw->unmap();
	Delete::pw = 0;
    }

    if (Delete::error != 0)
    {
	Delete::error->unmap();
	Delete::error = 0;
    }
}

void Delete::work ()
{
    int res, position;
    Boolean result;

    position = atoi(number->text());

    if (position > 0)
    {
	AtomicAction X;
	State *marker = head;
	Buffer buff;

	for (int k = 0; (k < position -1) && (marker != 0); k++, marker = marker->next);

	if (marker != 0)
	{
	    X.Begin();

	    buff.pack(marker->interfaces);
	    buff.pack(marker->hostname);
	    buff.pack(marker->pathname);
	    buff.pack(marker->servers);
	    buff.pack(marker->u_id);

	    res = -1;
	    result = Delete::itf->Delete(position, buff, res);

	    if (res == 0)
	    {
		if (X.End() != COMMITTED)
		    cerr << "X.End() : Not Done\n";
	    }
	    else
	    {
		X.Abort();

		if (error == 0)
		{
		    Session *s = World::current()->session();
		    Button *b1 = Kit::instance()->simple_push_button(" OK ", s->style(), Delete::fault);
		    Menu *m = Kit::instance()->menubar(s->style());
		    m->add_item(b1->telltale(), b1->action());
		    Glyph *hspace = new HGlue(15.0);
		    LRBox *bt = new LRBox(hspace, hspace, m, hspace);
		    TBBox *bts = new TBBox(new Border(new Margin(bt, 2.0),
						      World::current()->foreground(), 1.0));
		    Patch *p1 = new Patch(new VCenter(Message(World::current()->font(),
							      World::current()->foreground(),
							      "Error! Cannot inspect database."), 1.0));
		    Patch *p2 = 0;
		    if (res == -1)
			p2 = new Patch(new VCenter(Message(World::current()->font(),
							   World::current()->foreground(),
							   "Lock Conflict."), 1.0));
		    if (res == -2)
			p2 = new Patch(new VCenter(Message(World::current()->font(),
							   World::current()->foreground(),
							   "Missing interface. Try rescanning database."), 1.0));
		    TBBox *bx;
		    if (p2 == 0)
			bx = new TBBox(p1, bts);
		    else
			bx = new TBBox(p1, p2, bts);
		    TBBox *by = new TBBox(new Border(new Margin(bx, 2.0),
						     World::current()->foreground(), 1.0));
		    Background *bg1 = new Background(by, World::current()->background());
		    error = new PopupWindow(bg1);
		    error->place(400, 600);
		    error->map();
		}

		World::current()->RingBell(0);
	    }

	    if (Delete::pw != 0)
	    {
		Delete::pw->unmap();
		Delete::pw = 0;
	    }
	}
    }
}
