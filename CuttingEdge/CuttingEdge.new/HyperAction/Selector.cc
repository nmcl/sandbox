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
 * $Id: Selector.cc,v 1.2 1991/09/26 13:27:13 nsmw Exp $
 */

#include <InterViews/sensor.h>
#include <InterViews/event.h>

#include "HyperMainWindow.h"

#include "Selector.h"

Selector::Selector(Glyph* cont, char* dck, char* crd) : Listener(cont, this)
{
    button(true, Event::left);

    deck = dck;
    card = crd;
}

Selector::~Selector()
{
}

void Selector::event(Event& e)
{
    switch (e.type())
    {
    case Event::down:
	HyperMainWindow::Search(deck, card);
	break;
    default:
	break;
    }
}
