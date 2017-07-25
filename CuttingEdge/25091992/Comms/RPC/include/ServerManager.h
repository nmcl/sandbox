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

#ifndef _SERVERMANAGER_H_
#define _SERVERMANAGER_H_

/*
 *  $Id: ServerManager.h,v 1.3 1992/01/28 11:47:56 nmcl Exp $
 */

#include <Port.h>
#include <PortAddress.h>

class ServerManager
{
private:
    PortAddress *ManagerAddr;
public:
    ServerManager();
    ~ServerManager();

    PortOutcome Initial_Server(int argc, char* argv[], PortAddress& Client);
    PortOutcome Quit();
};

#endif  _SERVERMANAGER_H_
