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
 * $Id: HyperCard.cc,v 1.1 1991/08/28 15:42:59 nsmw Exp $
 */

#include <InterViews/box.h>

#include "HyperCard.h"

HyperCard::HyperCard(char* n, Glyph** b_l, int num) : TBBox(num)
{
    name = n;

    for (int i = 0; i < num; i++)
	append(b_l[i]);
}

HyperCard::~HyperCard()
{
}

char* HyperCard::getname()
{
    return name;
}
