/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Theatre.cc,v 1.1 1994/07/01 10:53:49 ngdp Exp $
 */


#ifndef THEATRE_H_
#  include "Theatre.h"
#endif


Theatre::Theatre ()
{
    for (int i = 0; i < THEATRE_SIZE; i++)
	Seats[i] = UNRESERVED;
}

Theatre::~Theatre ()
{
}

Boolean Theatre::reserveSeat (int index)
{
    Boolean result = FALSE;
    
    if ((index < 0) || (index >= THEATRE_SIZE))
	return FALSE;

    if (Seats[index] == UNRESERVED)
    {
	Seats[index] = RESERVED;
	result = TRUE;
    }

    return result;
}

Boolean Theatre::unreserveSeat (int index)
{
    Boolean result = FALSE;
    
    if ((index < 0) || (index >= THEATRE_SIZE))
	return FALSE;

    if (Seats[index] == RESERVED)
    {
	Seats[index] = UNRESERVED;
	result = TRUE;
    }

    return result;
}    
    
Boolean Theatre::inspectSeat (int index, int& status)
{
    if ((index < 0) || (index >= THEATRE_SIZE))
	return FALSE;

    status = Seats[index];

    return TRUE;
}    
