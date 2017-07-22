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
 * $Id: HyperWindow.h,v 1.1 1991/08/28 15:43:23 nsmw Exp $
 */

#ifndef _HYPERWINDOW_H_
#define _HYPERWINDOW_H_

#include <InterViews/window.h>

#include "HyperDeck.h"

class HyperWindow
{
    char*               name;
    HyperDeck*          deck;
    TransientWindow*    win;
    int                 mapped;

    HyperWindow*        next;
    static HyperWindow* head;
    int SearchForDeck(char* dck, char* crd);
public:
    HyperWindow(char* n, HyperDeck* dck);
    virtual ~HyperWindow();

    static int Search(char* dck, char* crd);
};

#endif  _HYPERWINDOW_H_
