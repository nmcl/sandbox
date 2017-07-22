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
 * $Id: HyperDeck.h,v 1.1 1991/08/28 15:43:16 nsmw Exp $
 */

#ifndef _HYPERDECK_H_
#define _HYPERDECK_H_

#include <InterViews/deck.h>
#include <InterViews/patch.h>

#include "HyperCard.h"

struct CardIndex
{
    HyperCard* card;
    int        index;
};

class HyperDeck : public Patch
{
    char*             name;
    CardIndex*        cardIndex;
    int               number;

    Deck* d;
public:
    HyperDeck(char* n, HyperCard** list, int num);
    virtual ~HyperDeck();

    char* getname();

    int SearchForCard(char* card);
};

#endif  _HYPERDECK_H_
