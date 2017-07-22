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
 * $Id: HyperText.cc,v 1.1 1991/08/28 15:43:05 nsmw Exp $
 */

#include <InterViews/label.h>
#include <InterViews/box.h>
#include <InterViews/glue.h>
#include <InterViews/discretion.h>
#include <InterViews/simplecomp.h>
#include <InterViews/world.h>
#include <InterViews/font.h>
#include <InterViews/color.h>

#include "HyperText.h"

HyperText::HyperText(HyperWord** words, int num, int width) : LRComposition(new TBBox, new SimpleCompositor, nil, width)
{
    Discretionary* seper = new Discretionary(0, new Label(" ", World::current()->font(), World::current()->foreground()), nil, nil, nil);

    for (int i = 0; i < num; i++)
    {
	append(words[i]);
	append(seper);
    }
    append(new HGlue);
    repair();
}

HyperText::~HyperText()
{
}
