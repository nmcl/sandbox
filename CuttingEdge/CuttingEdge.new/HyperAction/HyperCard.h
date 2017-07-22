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
 * $Id: HyperCard.h,v 1.1 1991/08/28 15:43:14 nsmw Exp $
 */

#ifndef _HYPERCARD_H_
#define _HYPERCARD_H_

#include <InterViews/box.h>

#include "HyperCard.h"

class HyperCard : public TBBox
{
    char* name;
public:
    HyperCard(char* name, Glyph** b_l, int num);
    virtual ~HyperCard();

    char* getname();
};
	
#endif  _HYPERCARD_H_
