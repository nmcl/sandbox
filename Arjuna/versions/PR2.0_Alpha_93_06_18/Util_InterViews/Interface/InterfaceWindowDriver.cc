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

#include <Configure.h>

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#include <System/fstream.h>
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

#define _INIT_
#ifndef INTERFACEARGS_H_
#include "InterfaceArgs.h"
#endif

#include "Interface_stub.h"
#include "Transporter_stub.h"

#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/border.h>
#include <InterViews/margin.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>
#include <InterViews/border.h>
#include <InterViews/world.h>
#include <InterViews/margin.h>
#include <InterViews/color.h>
#include <InterViews/label.h>
#include <InterViews/handler.h>
#include <InterViews/patch.h>
#include <InterViews/style.h>
#include <InterViews/session.h>
#include <IV-look/button.h>
#include <IV-look/menu.h>
#include <IV-look/telltale.h>
#include <IV-look/kit.h>
#include <IV-look/mono_kit.h>
#include <IV-look/field.h>

#include "Message.h"
#include "Selector.h"
#include "Window.h"
#include "Common.h"
#include "Menu.h"

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

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    if (!InterfaceArgs::CheckArgs(argc, argv, FALSE))
    {
	cerr << "Usage: InterfaceDriver [-uid <database UID>]\n" << flush;
	cerr << "                       [-name <database hostname]\n" << flush;
	exit(0);
    }

    World world("Interface", argc, argv, 0, 0);
    const Font *f = world.font();
    const Color *fg = world.foreground();
    const Color *c = world.background();
    char what[1024], name[20], where[100], placement[100];
    char *buffer;
    int i = 0, res = 0, size = 0;
    Interface *itf = 0;

    ArjunaName AN;
    AN.SetServiceName("Interface");
    if (Definitions::DatabaseLocation)
	AN.SetHostName(Definitions::DatabaseLocation);

    ClientRpc *RPCClient = new ClientRpc(&AN);
    RpcControl *RPCCont = new RpcControl(RPCClient);

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

    TBBox *bx = new TBBox;
    for (int l = 0; l < LINES; l++)
    {
        Patch *p = new Blank_Line;
        bx->insert(l, p);
    }

    Scanner *scn = new Scanner(itf);
    buffer = scn->contents();
    Session *s = world.session();
    FieldEditor *fe = new FieldEditor("/tmp/header.h", s->style());
    Background *gb = new Background(bx, c);
    ScrollingText *stx = new ScrollingText(buffer, gb, fe);
    Glyph *scrollBarText = Kit::instance()->vscroll_bar(stx, s->style());
    LRBox *box = new LRBox(stx, scrollBarText);
    Entry *et = new Entry(itf);

    scn->add_body(stx);

    Delete *del = new Delete(itf);
    Button *b1 = Kit::instance()->simple_push_button(" Quit ", s->style(), new foo());
    Button *b2 = Kit::instance()->simple_push_button(" Add ", s->style(), et);
    Button *b3 = Kit::instance()->simple_push_button(" Rescan ", s->style(), scn);
    Button *b4 = Kit::instance()->simple_push_button(" Delete ", s->style(), del);
    Menu *m = Kit::instance()->menubar(s->style());

    m->add_item(b1->telltale(), b1->action());
    m->add_item(b2->telltale(), b2->action());
    m->add_item(b3->telltale(), b3->action());
    m->add_item(b4->telltale(), b4->action());

    LRBox *bts = new LRBox(new Border(new Margin(m, 2.0), fg, 1.0));
    TBBox *mn = new TBBox(bts, fe);
    Background *bg1 = new Background(mn, c);
    TBBox *n = new TBBox(box, bg1);
    TopLevelWindow *w3 = new TopLevelWindow(n);
    
    w3->geometry("500x233");
    w3->place(400, 200);
    w3->name("Interface Viewer");
    w3->map();

    world.RingBell(0);
    world.run();

    if (itf)
	delete itf;
}
