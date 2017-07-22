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
 * $Id: HyperImage.cc,v 1.1 1991/08/28 15:43:02 nsmw Exp $
 */

#include <iostream.h>

#include <InterViews/bitmap.h>
#include <InterViews/world.h>

#include "Selector.h"
#include "HyperImage.h"

HyperImage::HyperImage(char* filename, char* deck, char* card)
{
    Bitmap* bm = Bitmap::open(filename);

    if (bm == nil)
    {
	cerr << "Invalid bitmap : " << filename << "\n";
	cont = nil;
    }
    else
	cont = new Stencil(bm, World::current()->foreground());

    if ((deck != nil) || (card != nil))
	body(new Selector(cont, deck, card));
    else
	body(cont);
}

HyperImage::~HyperImage()
{
}
