/*
 * Copyright 1990, 1991, 1992,
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

#ifndef PORT_H_
#define PORT_H_

/*
 * $Id: Port.h,v 1.1 1992/02/21 10:25:45 nmcl Exp $
 */

#include <iostream.h>

#include <Common/Boolean.h>
#include <Common/Buffer.h>

#include "PortAddress.h"

class Port
{
public:
    enum Outcome { Done, NotDone, Unknown };

    Port ();                                 // Create new port
    Port (PortAddress& portAddress);         // Create port with given address
    ~Port ();                                // Delete port

    Boolean Valid() const;                   // Is Port valid?

    // Send message in buffer to receiver at designated destination
    Outcome SendMessage    (PortAddress  destinationPortAddress, Buffer& buffer) const;

    // Receive message into buffer; sourcePortAddress indicates sender of message
    Outcome ReceiveMessage (PortAddress& sourcePortAddress,      Buffer& buffer) const;

    // Is there a message waiting to be received?
    Outcome PollForMessage (Boolean& waitingMessage) const;

    // Return PortAddres for this Port
    Outcome GetAddress     (PortAddress& localPortAddress) const;

private:
    Boolean ValidPort;
    int     sock;

public:
    ostream& print (ostream&) const;
};

extern ostream& operator<< (ostream& strm, const Port& port);

#endif
