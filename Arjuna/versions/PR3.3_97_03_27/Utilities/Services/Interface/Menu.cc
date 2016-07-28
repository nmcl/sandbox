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

#include <InterViews/glue.h>
#include <InterViews/background.h>
#include <InterViews/box.h>
#include <InterViews/border.h>
#include <InterViews/layout.h>
#include <InterViews/style.h>
#include <IV-look/kit.h>
#include <IV-look/button.h>
#include <IV-look/dialogs.h>
#include <OS/string.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#include "Interface_stub.h"

#define SET_
#include "Menu.h"
#include "Message.h"
#include "Window.h"

extern State *head, *ptr;
extern void free_list();
extern ScrollingText* stx;


class Interface *Scanner::itf = 0;
class State *Scanner::a = 0;
class ApplicationWindow *Scanner::error = 0;
class Commit *Scanner::fault = 0;

class TopLevelWindow *Entry::pw = 0;
class FieldEditor *Entry::desc = 0;
class FieldEditor *Entry::path_l = 0;
class FieldEditor *Entry::mac_n = 0;
class FieldEditor *Entry::servers = 0;
class ApplicationWindow *Entry::error = 0;
class Commit *Entry::fault = 0;
class State *Entry::a = 0;
class Interface *Entry::itf = 0;

class Interface *Delete::itf = 0;
class TopLevelWindow *Delete::pw = 0;
class ApplicationWindow *Delete::error = 0;
class FieldEditor *Delete::number = 0;
class Commit *Delete::fault = 0;

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
	desc = DialogKit::instance()->field_editor("Interface description", WidgetKit::instance()->style());
	path_l = DialogKit::instance()->field_editor("header location", WidgetKit::instance()->style());
	mac_n = DialogKit::instance()->field_editor("machine name", WidgetKit::instance()->style());
	servers = DialogKit::instance()->field_editor("server locations", WidgetKit::instance()->style());
	Button *b1 = WidgetKit::instance()->push_button(" OK ",  cmt);
	Button *b2 = WidgetKit::instance()->push_button(" Cancel ", cnl);
	Menu *m = WidgetKit::instance()->menubar();
	m->append_item(new MenuItem(b1, b1->state(), b1->action()));
	m->append_item(new MenuItem(b2, b2->state(), b2->action()));
	Glyph *hspace = LayoutKit::instance()->hglue(15.0);
	Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
	Glyph *bts = LayoutKit::instance()->vbox(new Border(m, WidgetKit::instance()->foreground(), 1.0));
	Glyph *bx = LayoutKit::instance()->vbox(desc, path_l, mac_n, servers, bts);
	Glyph *by = LayoutKit::instance()->vbox(new Border(bx, WidgetKit::instance()->foreground(), 1.0));
	Background *bg1 = new Background(by, WidgetKit::instance()->background());
	pw = new TopLevelWindow(bg1);
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
    char* what = new char[strlen((char*) (desc->text())->string())+1];
    char* where = new char[strlen((char*) (mac_n->text())->string())+1];
    char* pth = new char[strlen((char*) (path_l->text())->string())+1];
    char* srv = new char[strlen((char*) (servers->text())->string())+1];

    ::strcpy(what, (desc->text())->string());
    ::strcpy(where, (mac_n->text())->string());
    ::strcpy(pth, (path_l->text())->string());
    ::strcpy(srv, (servers->text())->string());
    Entry::a = new State();
    Entry::a->Set_State(what, where, pth, srv);

    AtomicAction Z;
    Buffer buff;
    Boolean result;

    Z.Begin();

    buff.packMappedString(what);
    buff.packMappedString(where);
    buff.packMappedString(pth);
    buff.packMappedString(srv);

    res = -1;
    
    result = itf->Add(buff, res);

    if (res != 0)
    {
	Z.Abort();

	if (error == 0)
	{
	    Button *b1 = WidgetKit::instance()->push_button(" OK ", Entry::fault);
	    Menu *m = WidgetKit::instance()->menubar();
	    m->append_item(new MenuItem(b1, b1->state(), b1->action()));
	    Glyph *hspace = LayoutKit::instance()->hglue(15.0);
	    Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
	    Glyph *bts = LayoutKit::instance()->vbox(new Border(bt,
								WidgetKit::instance()->foreground(), 1.0));
	    Patch *p1 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  "Error! Could not add item."));
	    Patch *p2 = 0;
	    if (res == -1)
		p2 = new Patch(Message(WidgetKit::instance()->font(),
				       WidgetKit::instance()->foreground(),
				       "Possible Lock Conflict."));
	    Glyph *bx;
	    if (p2 == 0)
		bx = LayoutKit::instance()->vbox(p1, bts);
	    else
		bx = LayoutKit::instance()->vbox(p1, p2, bts);
	    Glyph *by = LayoutKit::instance()->vbox(new Border(bx,
							       WidgetKit::instance()->foreground(), 1.0));
	    Background *bg1 = new Background(by, WidgetKit::instance()->background());

	    error = new ApplicationWindow(bg1);
	    error->map();
	}
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

char *Scanner::contents ()
{
    int res, position = 0, size = 0;
    AtomicAction X;
    Boolean result = TRUE;

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
	char *interfaces = 0, *hostname = 0, *pathname = 0, *servers = 0;
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
	    
	    ::delete [] interfaces;
	    ::delete [] hostname;
	    ::delete [] pathname;
	    ::delete [] servers;

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
	    Button *b1 = WidgetKit::instance()->push_button(" OK ", Scanner::fault);
	    Menu *m = WidgetKit::instance()->menubar();
	    m->append_item(new MenuItem(b1, b1->state(), b1->action()));
	    Glyph *hspace = LayoutKit::instance()->hglue(15.0);
	    Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
	    Glyph *bts = LayoutKit::instance()->vbox(new Border(bt,
								WidgetKit::instance()->foreground(), 1.0));
	    Patch *p1 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  "Error! Cannot inspect database."));
	    Patch *p2 = new Patch(Message(WidgetKit::instance()->font(),
					  WidgetKit::instance()->foreground(),
					  "Lock Conflict."));
	    Glyph *bx = LayoutKit::instance()->vbox(p1, p2, bts);
	    Glyph *by = LayoutKit::instance()->vbox(new Border(bx,
							       WidgetKit::instance()->foreground(), 1.0));
	    Background *bg1 = new Background(by, WidgetKit::instance()->background());
	    error = new ApplicationWindow(bg1);
	    error->map();
	}
    }
    else
    {
	if (X.End() != COMMITTED)
	    cerr << "X.End() : Not Done\n";
    }

    if (size > 0)
	buffer = (char*) realloc(buffer, size);
    else
    {
	::delete [] buffer;
	buffer = (char*) 0;
    }
    
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
	number = DialogKit::instance()->field_editor("Interface number", WidgetKit::instance()->style());
	Button *b1 = WidgetKit::instance()->push_button(" OK ", cmt);
	Button *b2 = WidgetKit::instance()->push_button(" Cancel ", cnl);
	Menu *m = WidgetKit::instance()->menubar();
	m->append_item(new MenuItem(b1, b1->state(), b1->action()));
        m->append_item(new MenuItem(b2, b2->state(), b2->action()));
	Glyph *hspace = LayoutKit::instance()->hglue(15.0);
        Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
	Glyph *bts = LayoutKit::instance()->vbox(new Border(bt,
							    WidgetKit::instance()->foreground(), 1.0));
        Glyph *bx = LayoutKit::instance()->vbox(number, bts);
        Glyph *by = LayoutKit::instance()->vbox(new Border(bx,
							   WidgetKit::instance()->foreground(), 1.0));
        Background *bg1 = new Background(by, WidgetKit::instance()->background());
        pw = new TopLevelWindow(bg1);
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

    position = atoi((char*) (number->text())->string());

    if (position > 0)
    {
	AtomicAction X;
	State *marker = head;
	Buffer buff;

	for (int k = 0; (k < position -1) && (marker != 0); k++, marker = marker->next);

	if (marker != 0)
	{
	    X.Begin();

	    buff.packMappedString(marker->interfaces);
	    buff.packMappedString(marker->hostname);
	    buff.packMappedString(marker->pathname);
	    buff.packMappedString(marker->servers);
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
		    Button *b1 = WidgetKit::instance()->push_button(" OK ", Delete::fault);
		    Menu *m = WidgetKit::instance()->menubar();
		    m->append_item(new MenuItem(b1, b1->state(), b1->action()));
		    Glyph *hspace = LayoutKit::instance()->hglue(15.0);
		    Glyph *bt = LayoutKit::instance()->hbox(hspace, hspace, m, hspace);
		    Glyph *bts = LayoutKit::instance()->vbox(new Border(bt,
									WidgetKit::instance()->foreground(), 1.0));
		    Patch *p1 = new Patch(Message(WidgetKit::instance()->font(),
						  WidgetKit::instance()->foreground(),
						  "Error! Cannot inspect database."));
		    Patch *p2 = 0;
		    if (res == -1)
			p2 = new Patch(Message(WidgetKit::instance()->font(),
					       WidgetKit::instance()->foreground(),
					       "Lock Conflict."));
		    
		    if (res == -2)
			p2 = new Patch(Message(WidgetKit::instance()->font(),
					       WidgetKit::instance()->foreground(),
					       "Missing interface. Try rescanning database."));
		    Glyph *bx;
		    if (p2 == 0)
			bx = LayoutKit::instance()->vbox(p1, bts);
		    else
			bx = LayoutKit::instance()->vbox(p1, p2, bts);
		    Glyph *by = LayoutKit::instance()->vbox(new Border(bx,
								       WidgetKit::instance()->foreground(), 1.0));
		    Background *bg1 = new Background(by, WidgetKit::instance()->background());
		    error = new ApplicationWindow(bg1);
		    error->map();
		}
	    }

	    if (Delete::pw != 0)
	    {
		Delete::pw->unmap();
		Delete::pw = 0;
	    }
	}
    }
}
