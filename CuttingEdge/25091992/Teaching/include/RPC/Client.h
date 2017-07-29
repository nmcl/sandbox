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

#ifndef CLIENT_H_
#define CLIENT_H_

/*
 * $Id$
 */

#include <Port/Port.h>
#include <Port/PortAddress.h>

class Client
{
public:
    enum OutcomeC { Done, NotDone, Unknown };

    Client(char* service);
    Client(PortAddress& portAddress);
    ~Client();

    OutcomeC Call(Buffer& parameter, Buffer& result);
private:
    Port        port;
    PortAddress serverAddr;
};

#endif CLIENT_H_
