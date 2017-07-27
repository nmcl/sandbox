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

/*
 * $Id: Manager.cc,v 1.2 1991/08/19 20:02:29 nsmw Exp $
 */

#include <sysent.h>

#include <PortAddress.h>
#include <Port.h>

#ifdef DEBUG
#include <iostream.h>
#endif DEBUG

#include "Manager.h"

int main(int argc, char* argv[])
{
    PortAddress ManInterAddr;
    PortAddress ManagerAddr(MANAGER_PORT);
    
    Port    CommsPort(ManagerAddr);
    
    bool quit;
    
    if (CommsPort.Valid())
	quit = FALSE;
    else
    {
	perror("MANAGER: Valid CommsPort ");
	quit = TRUE;
    }
    
#ifdef DEBUG
    cerr << "MANAGER: Port is [" << ManagerAddr << "]\n"; cerr.flush();
#endif DEBUG
    
    while (! quit)
    {
	Buffer Buff;
	
	if (CommsPort.ReceiveMessage (ManInterAddr, Buff) != Done)
	    perror("MANAGER: ReceiveMessage ");
	else
	{
	    int  opcode;
	    Buff.unpack(opcode);
	    
	    switch (opcode)
	    {
	    case Do_Create_Server:
	    {
		char*          service  = NULL;
		PortAddress    ClientPortAddr;
		
		Buff.unpack(service);
		ClientPortAddr.unpack(Buff);

#ifdef DEBUG
		cerr << "MANAGER: Manager Interface is [" << ManInterAddr << "] : "
                     << service << " [" << ClientPortAddr << "]\n"; cerr.flush();
#endif

		unsigned short portnum1;
		char           portname1[32];
		char*          hostname1 = NULL;
		unsigned short portnum2;
		char           portname2[32];
		char*          hostname2 = NULL;

		if (! (ClientPortAddr.GetPort(portnum1) && ClientPortAddr.GetHost(hostname1)))
		{
		    perror("MANAGER: ClientPort Get Port & Host ");
		    exit(1);
		}

		if (! (ManInterAddr.GetPort(portnum2) && ManInterAddr.GetHost(hostname2)))
		{
		    perror("MANAGER: Client Get Port & Host ");
		    exit(1);
		}

		sprintf(portname1, "%d", portnum1);
		sprintf(portname2, "%d", portnum2);
		
		pid_t p_id;
		
		p_id = fork();
		
		if (p_id == 0)
		{
		    pid_t p_id;
		    
		    p_id = fork();
		    
		    if (p_id == 0)
		    {
			if (execl(service, service, hostname1, portname1, hostname2, portname2,NULL))
			    perror("MANAGER: exec ");
		    }
		    else if (p_id < 0)
			perror("MANAGER: sub-fork ");
		    else
			exit(0);
		    exit(1);
		}
		else if (p_id < 0)
		    perror("MANAGER: fork ");
		else
		{
		    union wait statusp;
		    int wait_res;
		    
		    wait_res = wait(&statusp);

#ifdef DEBUG
		    cerr << "MANAGER: wait " << wait_res << " "
                         << statusp.w_status << "\n"; cerr.flush();
#endif DEBUG
    
		    if ((wait_res <= 0) || (statusp.w_status != 0))
			perror("MANAGER: wait ");
		}
		
		if (service != NULL)
		    delete service;
		
		if (hostname1 != NULL)
		    delete hostname1;
		
		if (hostname2 != NULL)
		    delete hostname2;
	    }
	    break;
	case Do_Quit:
	    quit = TRUE;
	    break;
	default:
	    perror("MANAGER: opcode switch ");
	    break;
	}
	}
    }
    
    exit(0);
}













