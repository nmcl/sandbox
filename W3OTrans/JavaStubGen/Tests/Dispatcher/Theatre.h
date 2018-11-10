/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Theatre.h,v 1.1 1994/07/01 10:53:49 ngdp Exp $
 */

#ifndef THEATRE_H_
#define THEATRE_H_

#include <Config/Configure.h>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif


#define RESERVED 1
#define UNRESERVED 2
#define THEATRE_SIZE 20

enum TheatreOpcode { RESERVE, UNRESERVE, INSPECT };


class Theatre
{
public:
    Theatre ();
    ~Theatre ();

    Boolean reserveSeat (int);
    Boolean unreserveSeat (int);
    Boolean inspectSeat (int, int&);

private:
    int Seats[THEATRE_SIZE];
};

#endif


