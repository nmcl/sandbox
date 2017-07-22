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

#ifndef _PORT_H_
#define _PORT_H_

/*
 * $Id: Port.h,v 1.3 1992/02/06 17:35:02 nmcl Exp $
 */

#include <Std/boolean.h>
#include <Arjuna/Buffer.h>

#include "PortAddress.h"

class MultiBuffer
{
public:
    MultiBuffer();
    ~MultiBuffer();

    Buffer CurrBuff;
    MultiBuffer* next;
};

enum PortOutcome { Done, NotDone, Unknown };

class Port
{
public:
    Port ();
    Port (PortAddress&);
    ~Port ();

    PortOutcome SendMessage    (PortAddress,  Buffer&);
    PortOutcome ReceiveMessage (PortAddress&, Buffer&);
    PortOutcome SendLongMessage (PortAddress, MultiBuffer&);
    PortOutcome ReceiveLongMessage (PortAddress&, MultiBuffer&);
    PortOutcome PollForMessage (bool&);

    bool Valid();

    PortOutcome GetAddress     (PortAddress&) const;
private:
    bool ValidPort;
    int  sock;
};
    
#endif _PORT_H_
