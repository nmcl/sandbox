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

#ifndef SERVER_H
#define SERVER_H

/*
 * $Id$
 */

#include <Port/Port.h>
#include <Port/PortAddress.h>

class Server
{
public:
    enum OutcomeS { Done, NotDone, Unknown };

    Server(PortAddress& portAddress);
    Server(int argc, char *argv[]);
    ~Server();

    OutcomeS GetWork(Buffer& parameter);
    OutcomeS SendResult(Buffer& result);
private:
    Port         port;
    PortAddress  clientAddr;
};

#endif SERVER_H
