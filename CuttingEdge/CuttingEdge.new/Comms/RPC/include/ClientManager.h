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

#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

/*
 *  $Id: ClientManager.h,v 1.2 1992/01/28 11:47:52 nmcl Exp $
 */

#include <Port.h>
#include <PortAddress.h>

class ClientManager
{
private:
    PortAddress* ManagerAddr;
public:
    ClientManager(char* hostname);
    ~ClientManager();

    PortOutcome Create_Server(const char* service, PortAddress& ClientAddr, PortAddress& ServerAddr);
    PortOutcome Quit();
};

#endif  _CLIENTMANAGER_H_
