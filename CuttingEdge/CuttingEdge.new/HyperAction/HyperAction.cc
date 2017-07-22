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
 * $Id: HyperAction.cc,v 1.1 1991/08/28 15:42:56 nsmw Exp $
 */

#include <InterViews/world.h>

#include "HyperWindow.h"
#include "HyperMainWindow.h"

#include "read.h"

static PropertyData properties[] = {
    { nil }
};

static OptionDesc options[] = {
    { nil }
};

int main(int argc, char* argv[])
{
    World world("HyperAction", argc, argv, options, properties);
    
    HyperMainWindow* mainWin;
    read_mainwindow(mainWin, 0);

    int           num;
    HyperWindow** win_l;
    read_window_list(win_l, num, 0);

    world.run();
}
