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
 * $Id: HyperDeck.cc,v 1.1 1991/08/28 15:43:00 nsmw Exp $
 */

#include <string.h>

#include <InterViews/world.h>
#include <InterViews/canvas.h>
#include <InterViews/deck.h>

#include "HyperCard.h"
#include "HyperDeck.h"

HyperDeck::HyperDeck(char* n, HyperCard** list, int num) : Patch(nil)
{
    name   = n;
    number = num;

    cardIndex = new CardIndex[num];

    d = new Deck();

    for (int i = 0; i < number; i++)
    {
	d->insert(i, list[i]);
	cardIndex[i].card   = list[i];
	cardIndex[i].index = i;
    }
    d->flip_to(cardIndex[0].index);

    body(d);
}

HyperDeck::~HyperDeck()
{
}

char* HyperDeck::getname()
{
    return name;
}

int HyperDeck::SearchForCard(char* crd)
{
    int res = 0;

    for (int i = 0; (i < number) && (res == 0); i++)
    {
	if (strcmp(cardIndex[i].card->getname(), crd) == 0)
	{
	    d->flip_to(cardIndex[i].index);

	    redraw();
	    World::current()->sync();
	    res = 1;
	}
    }

    return res;
}
