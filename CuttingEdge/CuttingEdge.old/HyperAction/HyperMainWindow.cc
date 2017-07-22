/*
 * Copyright 1990, 1991,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

/*
 * $Id: HyperMainWindow.cc,v 1.1 1991/08/28 15:43:04 nsmw Exp $
 */

#include <iostream.h>
#include <string.h>

#include <InterViews/world.h>
#include <InterViews/background.h
#include <InterViews/glue.h>
#include <InterViews/box.h>
#include <InterViews/rule.h>

#include "HyperWord.h"
#include "HyperWindow.h"
#include "HyperMainWindow.h"

HyperMainWindow* HyperMainWindow::mainWin = nil;

HyperMainWindow::HyperMainWindow(char* n, HyperDeck* dck)
{
    name    = n;
    deck    = dck;
    mainWin = this;

    LRBox* cbox = new LRBox(new HyperWord(" ", nil, nil),
			    new HyperWord(deck->getname(), nil, nil),
			    new HyperWord(" ", nil, nil),
			    new VRule(World::current()->foreground(), 1),
                            new HGlue,
			    new HyperWord("Quit", nil, "Quit"),
			    new HyperWord(" ", nil, nil));

    TBBox* box  = new TBBox(cbox, new HRule(World::current()->foreground(), 1), deck, new VGlue);

    Background* bg = new Background(box, World::current()->background());

    win = new ApplicationWindow(bg);

    mapped = 1;
    win->map();
    win->raise();
}

HyperMainWindow::~HyperMainWindow()
{
}

int HyperMainWindow::SearchForDeck(char* dck, char* crd)
{
    int res = 0;

    if (strcmp(dck, name) == 0)
    {
	if (strcmp(crd, "Dismiss") == 0)
	{
	    if (mapped == 1)
	    {
		win->unmap();
		mapped = 0;
	    }
	    res = 1;
	}
    }
    else
    {
	if (strcmp(deck->getname(), dck) == 0)
	    res = deck->SearchForCard(crd);

	if ((res == 1) && (mapped == 0))
	{
	    win->map();
	    win->raise();
	    mapped = 1;
	}
    }

    return res;
}

void HyperMainWindow::Search(char* dck, char* crd)
{
    int res = 0;

    if (dck == nil)
    {
	if (strcmp(crd, "Quit") == 0)
	    World::current()->quit();
	else
	    cerr << "No Target for (<nil>, " << crd << ")\n";
	res = 1;
    }
    
    if (res == 0)
	res = mainWin->SearchForDeck(dck, crd);

    if (res == 0)
	res = HyperWindow::Search(dck, crd);

    if (res == 0)
	cerr << "No Target for (" << dck << ", " << crd << ")\n";
}
