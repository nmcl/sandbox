/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TheatreInter.cc,v 1.1 1993/11/03 12:24:31 nmcl Exp $
 */

#include <InterViews/2.6/InterViews/box.h>
#include <IV-look/2.6/InterViews/menu.h>
#include <InterViews/2.6/InterViews/glue.h>
#include <IV-look/2.6/InterViews/banner.h>
#include <IV-look/2.6/InterViews/border.h>
#include <InterViews/world.h>
#include <InterViews/2.6/InterViews/scene.h>

#include "SeatInter.h"

#include "TheatreInter.h"
#include "MenuItemDispatch.h"

const int AisleSize = 32;
const int FootRoom  = 8;

const int Selecting_Mode   = 0;
const int Unselecting_Mode = 1;

const int theatre_rows = 6;
const int theatre_seats[theatre_rows] = { 5, 6, 6, 6, 6, 6 };

TheatreInter::TheatreInter(int& numSeats, TheatreDispatch* ThDisp)
{
    num_seats = 0;
    TheatreDisp = ThDisp;

    MenuBar* menubar = new MenuBar;

    PulldownMenu* filemenu = new PulldownMenu("File ");
    filemenu->Include(new MenuItemDispatch("New", TheatreDisp, new_oper));
    filemenu->Include(new MenuItemDispatch("Open", TheatreDisp, open_oper));
    filemenu->Include(new MenuItemDispatch("Close", TheatreDisp, close_oper));
    menubar->Include(filemenu);

    PulldownMenu* modemenu = new PulldownMenu("Mode ");

    PullrightMenu* blockbookingmenu = new PullrightMenu("Block Booking >");
    blockbookingmenu->Include(new MenuItemDispatch("Start", TheatreDisp, blockbooking_begin_oper));
    blockbookingmenu->Include(new MenuItemDispatch("Commit", TheatreDisp, blockbooking_commit_oper));
    blockbookingmenu->Include(new MenuItemDispatch("Abort", TheatreDisp, blockbooking_abort_oper));
    modemenu->Include(blockbookingmenu);

    PullrightMenu* subblockbookingmenu = new PullrightMenu("Sub-Block Booking >");
    subblockbookingmenu->Include(new MenuItemDispatch("Start", TheatreDisp, subblockbooking_begin_oper));
    subblockbookingmenu->Include(new MenuItemDispatch("Commit", TheatreDisp, subblockbooking_commit_oper));
    subblockbookingmenu->Include(new MenuItemDispatch("Abort", TheatreDisp, subblockbooking_abort_oper));
    modemenu->Include(subblockbookingmenu);

    menubar->Include(modemenu);

    PulldownMenu* opermenu = new PulldownMenu("Operation ");
    opermenu->Include(new MenuItemDispatch("Reserve", TheatreDisp, reserve_oper));
    opermenu->Include(new MenuItemDispatch("Unreserve", TheatreDisp, unreserve_oper));
    opermenu->Include(new MenuItemDispatch("Redisplay", TheatreDisp, redisplay_oper));
    opermenu->Include(new MenuItemDispatch("Redisplay All", TheatreDisp, redisplay_all_oper));
    menubar->Include(opermenu);

    PulldownMenu* logmenu = new PulldownMenu("Log ");
    logmenu->Include(new MenuItemDispatch("Open", TheatreDisp, log_open_oper));
    logmenu->Include(new MenuItemDispatch("Close", TheatreDisp, log_close_oper));
    menubar->Include(logmenu);

    PulldownMenu* quitmenu = new PulldownMenu("Quit ");
    quitmenu->Include(new MenuItemDispatch("Cancel", TheatreDisp));
    quitmenu->Include(new MenuItemDispatch("Ok", TheatreDisp, quit_oper));
    menubar->Include(quitmenu);

    Insert(menubar);

    Insert(new HBorder());

    VBox* seat_rows = new VBox;
    
    for (int i = 0; i < theatre_rows; i++)
    {
	HBox* row = new HBox;

	row->Insert(new HGlue(0));
	for (int j = 0; j < theatre_seats[i]; j++)
	{
	    SeatInter* seat = new SeatInter(num_seats, TheatreDisp);
	    seats[num_seats] = seat;
	    num_seats++;
	    row->Insert(seat);
	}

	row->Insert(new HGlue(AisleSize, 0, 0));

	for (int k = 0; k < theatre_seats[i]; k++)
	{
	    SeatInter* seat = new SeatInter(num_seats, TheatreDisp);
	    seats[num_seats] = seat;
	    num_seats++;
	    row->Insert(seat);
	}
	row->Insert(new HGlue(0));

	seat_rows->Insert(new VGlue(FootRoom, 0, 0));
        seat_rows->Insert(row);
    }

    Insert(seat_rows);

    Insert(new VGlue(2*FootRoom, 0, 0));

    Insert(new HBorder());

    theatre_status = new Banner("The Newcastle Playhouse", "Log closed", "");

    Insert(theatre_status);

    operation_status = new Banner("", "", "");

    Insert(operation_status);

    numSeats = num_seats;
}

TheatreInter::~TheatreInter()
{
}

void TheatreInter::SetMode(char* text)
{
    operation_status->left = text;
    if (GetWorld() != 0)
	operation_status->Update();
}

void TheatreInter::SetLogStatus(char* text)
{
    theatre_status->middle  = text;
    if (GetWorld() != 0)
	theatre_status->Update();
}

void TheatreInter::SetOperation(char* text)
{
    operation_status->middle = text;
    if (GetWorld() != 0)
	operation_status->Update();
}

void TheatreInter::SetOperResult(char* text)
{
    operation_status->right  = text;
    if (GetWorld() != 0)
	operation_status->Update();
}

void TheatreInter::Select(int seat_num)
{
    seats[seat_num]->Select();
}

void TheatreInter::Unselect(int seat_num)
{
    seats[seat_num]->Unselect();
}

void TheatreInter::Reserve(int seat_num)
{
    seats[seat_num]->Reserve();
}

void TheatreInter::Unreserve(int seat_num)
{
    seats[seat_num]->Unreserve();
}

void TheatreInter::Unknown(int seat_num)
{
    seats[seat_num]->Unknown();
}
  
