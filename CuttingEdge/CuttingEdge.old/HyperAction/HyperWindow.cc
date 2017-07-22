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
 * $Id: HyperWindow.cc,v 1.1 1991/08/28 15:43:07 nsmw Exp $
 */

#include <string.h>

#include <InterViews/world.h>
#include <InterViews/background.h>
#include <InterViews/glue.h>
#include <InterViews/box.h>
#include <InterViews/rule.h>

#include "HyperWord.h"
#include "HyperWindow.h"

HyperWindow* HyperWindow::head = nil;

HyperWindow::HyperWindow(char* n, HyperDeck* dck)
{
    name   = n;
    deck   = dck;
    next   = head;
    head   = this;
    
    LRBox* cbox = new LRBox(new HyperWord(" ", nil, nil),
			    new HyperWord(deck->getname(), nil, nil),
			    new HyperWord(" ", nil, nil),
			    new VRule(World::current()->foreground(), 1),
                            new HGlue,
			    new HyperWord("Dismiss", name, "Dismiss"),
			    new HyperWord(" ", nil, nil));

    TBBox* box  = new TBBox(cbox, new HRule(World::current()->foreground(), 1), deck);

    Background* bg = new Background(box, World::current()->background());

    win    = new TransientWindow(bg);

    mapped = 0;
}

HyperWindow::~HyperWindow()
{
}

int HyperWindow::SearchForDeck(char* dck, char* crd)
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

int HyperWindow::Search(char* dck, char* crd)
{
    int res = 0;

    HyperWindow* temp = head;

    while ((res == 0) && (temp != nil))
    {
	res = temp->SearchForDeck(dck, crd);
	temp = temp->next;
    }

    return res;
}
