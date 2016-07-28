/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: SeatInter.cc,v 1.2 1994/10/31 12:07:30 nsmw Exp $
 */

#include <InterViews/canvas.h>
#include <IV-2_6/InterViews/shape.h>
#include <IV-2_6/InterViews/painter.h>
#include <InterViews/event.h>
#include <IV-2_6/InterViews/sensor.h>
#include <InterViews/bitmap.h>

#include "TheatreDispatch.h"
#include "LogBaseEvent.h"
#include "SeatInter.h"

#include "Empty_Seat.xbm"
#include "Full_Seat.xbm"
#include "Unknown_Seat.xbm"

#include "Selected_Empty_Seat.xbm"
#include "Selected_Full_Seat.xbm"
#include "Selected_Unknown_Seat.xbm"

static Bitmap* empty_seat_bmap          = 0;
static Bitmap* full_seat_bmap           = 0;
static Bitmap* unknown_seat_bmap        = 0;
static Bitmap* empty_select_seat_bmap   = 0;
static Bitmap* full_select_seat_bmap    = 0;
static Bitmap* unknown_select_seat_bmap = 0;

SeatInter::SeatInter(int seat_num, TheatreDispatch* ThDisp)
{
    status      = Seat_Unknown;
    seat_number = seat_num;
    TheatreDisp = ThDisp;

    if (empty_seat_bmap == 0)
	empty_seat_bmap = new Bitmap(Empty_Seat_bits, Empty_Seat_width, Empty_Seat_height);

    if (full_seat_bmap == 0)
	full_seat_bmap = new Bitmap(Full_Seat_bits, Full_Seat_width, Full_Seat_height);

    if (unknown_seat_bmap == 0)
	unknown_seat_bmap = new Bitmap(Unknown_Seat_bits, Unknown_Seat_width, Unknown_Seat_height);

    if (empty_select_seat_bmap == 0)
	empty_select_seat_bmap = new Bitmap(Selected_Empty_Seat_bits, Selected_Empty_Seat_width, Selected_Empty_Seat_height);

    if (full_select_seat_bmap == 0)
	full_select_seat_bmap = new Bitmap(Selected_Full_Seat_bits, Selected_Full_Seat_width, Selected_Full_Seat_height);

    if (unknown_select_seat_bmap == 0)
	unknown_select_seat_bmap = new Bitmap(Selected_Unknown_Seat_bits, Selected_Unknown_Seat_width, Selected_Unknown_Seat_height);

    shape->Rect(Unknown_Seat_width, Unknown_Seat_height);
    shape->Rigid();
    output = new Painter;
    input = new Sensor;
    input->Catch(DownEvent);
}

SeatInter::~SeatInter()
{
}

void SeatInter::Select()
{
    switch (status)
    {
    case Seat_Empty:
        status = Seat_Select_Empty;
	break;
    case Seat_Full:
	status = Seat_Select_Full;
	break;
    case Seat_Unknown:
	status = Seat_Select_Unknown;
	break;
    case Seat_Select_Empty:
	status = Seat_Select_Empty;
	break;
    case Seat_Select_Full:
	status = Seat_Select_Full;
	break;
    case Seat_Select_Unknown:
	status = Seat_Select_Unknown;
	break;
    default:
	break;
    };
    Draw();
}

void SeatInter::Unselect()
{
    switch (status)
    {
    case Seat_Empty:
        status = Seat_Empty;
	break;
    case Seat_Full:
	status = Seat_Full;
	break;
    case Seat_Unknown:
	status = Seat_Unknown;
	break;
    case Seat_Select_Empty:
	status = Seat_Empty;
	break;
    case Seat_Select_Full:
	status = Seat_Full;
	break;
    case Seat_Select_Unknown:
	status = Seat_Unknown;
	break;
    default:
	break;
    };
    Draw();
}

void SeatInter::Reserve()
{
    switch (status)
    {
    case Seat_Empty:
        status = Seat_Full;
	break;
    case Seat_Full:
	status = Seat_Full;
	break;
    case Seat_Unknown:
	status = Seat_Full;
	break;
    case Seat_Select_Empty:
	status = Seat_Select_Full;
	break;
    case Seat_Select_Full:
	status = Seat_Select_Full;
	break;
    case Seat_Select_Unknown:
	status = Seat_Select_Full;
	break;
    default:
	break;
    };
    Draw();
}

void SeatInter::Unreserve()
{
    switch (status)
    {
    case Seat_Empty:
        status = Seat_Empty;
	break;
    case Seat_Full:
	status = Seat_Empty;
	break;
    case Seat_Unknown:
	status = Seat_Empty;
	break;
    case Seat_Select_Empty:
	status = Seat_Select_Empty;
	break;
    case Seat_Select_Full:
	status = Seat_Select_Empty;
	break;
    case Seat_Select_Unknown:
	status = Seat_Select_Empty;
	break;
    default:
	break;
    };
    Draw();
}

void SeatInter::Unknown()
{
    switch (status)
    {
    case Seat_Empty:
        status = Seat_Unknown;
	break;
    case Seat_Full:
	status = Seat_Unknown;
	break;
    case Seat_Unknown:
	status = Seat_Unknown;
	break;
    case Seat_Select_Empty:
	status = Seat_Select_Unknown;
	break;
    case Seat_Select_Full:
	status = Seat_Select_Unknown;
	break;
    case Seat_Select_Unknown:
	status = Seat_Select_Unknown;
	break;
    default:
	break;
    };
    Draw();
}

void SeatInter::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    switch (status)
    {
    case Seat_Empty:
	output->Stencil(canvas, 0, 0, empty_seat_bmap, nil);
	break;
    case Seat_Full:
	output->Stencil(canvas, 0, 0, full_seat_bmap, nil);
	break;
    case Seat_Unknown:
	output->Stencil(canvas, 0, 0, unknown_seat_bmap, nil);
	break;
    case Seat_Select_Empty:
	output->Stencil(canvas, 0, 0, empty_select_seat_bmap, nil);
	break;
    case Seat_Select_Full:
	output->Stencil(canvas, 0, 0, full_select_seat_bmap, nil);
	break;
    case Seat_Select_Unknown:
	output->Stencil(canvas, 0, 0, unknown_select_seat_bmap, nil);
	break;
    default:
	break;
    };
}

void SeatInter::Handle(Event& e)
{
    switch (e.eventType)
    {
    case DownEvent :
	TheatreDisp->SeatDo(seat_number);
	break;
    default:
	break;
    }
}
	
