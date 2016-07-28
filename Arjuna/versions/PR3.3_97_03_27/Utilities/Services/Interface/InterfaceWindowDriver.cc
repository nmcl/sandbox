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

#include <Config/Configure.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
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

#ifndef INTERFACEARGS_H_
#  include "InterfaceArgs.h"
#endif

#include "Interface_stub.h"
#include "Transporter_stub.h"

#include <InterViews/layout.h>
#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/box.h>
#include <InterViews/border.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>
#include <InterViews/color.h>
#include <InterViews/label.h>
#include <InterViews/handler.h>
#include <InterViews/patch.h>
#include <InterViews/style.h>
#include <InterViews/session.h>
#include <InterViews/scrbox.h>
#include <IV-look/button.h>
#include <IV-look/menu.h>
#include <IV-look/kit.h>
#include <IV-look/mono_kit.h>
#include <IV-look/field.h>
#include <IV-look/dialogs.h>

#include "Message.h"
#include "Selector.h"
#include "Window.h"
#include "Menu.h"


Patch* patch = (Patch*) 0;
Glyph* bar = (Glyph*) 0;
TBScrollBox* scrBox = (TBScrollBox*) 0;
ScrollingText* stx = (ScrollingText*) 0;
State *head = 0, *ptr = 0;


void free_list ()
{
    if (head != 0)
    {
	ptr = head;
	while (ptr->next != 0)
	{
	    ptr = ptr->next;
	    delete ptr->prev;
	}
    }

    head = 0;
    ptr = head;
}

int main(int argc, char** argv)
{
    if (!InterfaceArgs::CheckArgs(argc, argv, FALSE))
    {
	cerr << "Usage: InterfaceWindowDriver [-uid <database UID>]\n" << flush;
	cerr << "                             [-name <database hostname]\n" << flush;
	exit(0);
    }

    Session s("Interface Viewer", argc, argv, 0, 0);    
    char what[1024], name[20], where[100], placement[100];
    char *buffer;
    int i = 0, res = 0, size = 0;
    Interface *itf = 0;

    ArjunaName AN;
    AN.setServiceName("Interface");
    if (Definitions::DatabaseLocation)
	AN.setHostName(Definitions::DatabaseLocation);

    ClientRpc *RPCClient = new ClientRpc(&AN);
    ClientRpcManager *RPCCont = new ClientRpcManager(RPCClient);

    if (Definitions::DatabaseUID)
    {
	Uid x(Definitions::DatabaseUID);
	itf = new Interface(x, res, RPCCont);
    }
    else
    {
	itf = new Interface(res, RPCCont);
    }

    if (res != 1)
    {
	cerr << "Constructor error\n";
	exit(0);
    }

    memset(what, '\0', 1024);
    memset(name, '\0', 20);
    memset(where, '\0', 100);
    memset(placement, '\0', 100);

    FieldEditor* fe = DialogKit::instance()->field_editor("/tmp/header.h", WidgetKit::instance()->style());
    Scanner *scn = new Scanner(itf);
    buffer = scn->contents();

    scrBox = new TBScrollBox;
    stx = new ScrollingText(buffer, fe);
    bar = WidgetKit::instance()->vscroll_bar(scrBox);
    Glyph* _scr = LayoutKit::instance()->hbox(scrBox, bar);
    
    Entry *et = new Entry(itf);
    Delete *del = new Delete(itf);
    
    Button *b1 = WidgetKit::instance()->push_button(" Quit ", WidgetKit::instance()->quit());
    Button *b2 = WidgetKit::instance()->push_button(" Add ", et);
    Button *b3 = WidgetKit::instance()->push_button(" Rescan ",  scn);
    Button *b4 = WidgetKit::instance()->push_button(" Delete ", del);
    Menu *m = WidgetKit::instance()->menubar();

    m->append_item(new MenuItem(b1, b1->state(), b1->action()));
    m->append_item(new MenuItem(b2, b2->state(), b2->action()));
    m->append_item(new MenuItem(b3, b3->state(), b3->action()));
    m->append_item(new MenuItem(b4, b4->state(), b4->action()));

    Glyph *bts = LayoutKit::instance()->hbox(new Border(m, WidgetKit::instance()->foreground(), 1.0));
    Glyph *mn = LayoutKit::instance()->vbox(_scr, bts, fe);
    patch = new Patch(mn);
    ApplicationWindow* w3 = new ApplicationWindow(new Background(patch, WidgetKit::instance()->background()));

    s.run_window(w3);

    delete itf;
    delete RPCCont;

    return 0;
    
}
