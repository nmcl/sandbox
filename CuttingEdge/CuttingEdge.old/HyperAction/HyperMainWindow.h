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
 * $Id: HyperMainWindow.h,v 1.1 1991/08/28 15:43:19 nsmw Exp $
 */

#ifndef _HYPERMAINWINDOW_H_
#define _HYPERMAINWINDOW_H_

#include <InterViews/window.h>

#include "HyperDeck.h"

class HyperMainWindow
{
    char*                 name;
    HyperDeck*            deck;
    ApplicationWindow*    win;
    int                   mapped;

    static HyperMainWindow* mainWin;
    int SearchForDeck(char* dck, char* crd);
public:
    HyperMainWindow(char* n, HyperDeck* dck);
    virtual ~HyperMainWindow();

    static void Search(char* dck, char* crd);
};

#endif  _HYPERMAINWINDOW_H_
