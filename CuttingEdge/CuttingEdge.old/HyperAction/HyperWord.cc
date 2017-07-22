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
 * $Id: HyperWord.cc,v 1.1 1991/08/28 15:43:09 nsmw Exp $
 */

#include <InterViews/label.h>
#include <InterViews/world.h>

#include "Selector.h"
#include "HyperWord.h"

HyperWord::HyperWord(char* text, char* deck, char* card)
{
    Label* lbl = new Label(text, World::current()->font(), World::current()->foreground());

    if ((deck != nil) || (card != nil))
	body(new Selector(lbl, deck, card));
    else
	body(lbl);
}

HyperWord::~HyperWord()
{
}
