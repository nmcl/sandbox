/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: CalendarCreate.cc,v 1.2 1994/01/06 12:30:13 ngdp Exp $
 */

#include <Configure.h>

#include <System/iostream.h>
#include <System/stdlib.h>
#include <System/string.h>
#include <System/unistd.h>
#include <System/pwd.h>

#ifdef DEBUG
#include <Common/Debug.h>
#endif
#include "Calendar_stub.h"

#include <StubGen/ClientRpc.h>

#define LOCATION_SIZE_LIMIT 256
#define DEBUG_OVERHEAD 10
#define DEBUG_STR "debug_"

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "DEBUG -> Calendar server creator\n" << flush;
#endif

    uid_t user_id;
    struct passwd *user_pwentry;
    char* user_name = NULL;
    char* server_name = NULL;
    char* location = new char[LOCATION_SIZE_LIMIT];
    int res = 0;

    if (argc == 1)
    {
	user_id = getuid();
	user_pwentry = getpwuid(user_id);
	user_name = user_pwentry->pw_gecos;
	server_name = user_pwentry->pw_name;
	cout << "Your  id  is : " << server_name << "\n\n";
	cout << "Your name is : " << user_name << "\n" << flush;
    }
    else if (argc > 1)
    {
	if (strcmp(argv[1], "-d") == 0)
	{
	    char* tmp_str = new char[LOCATION_SIZE_LIMIT];

	    cout << "Server name (user's id) ? " << flush;
	    cin.getline(tmp_str, LOCATION_SIZE_LIMIT-DEBUG_OVERHEAD);
	    server_name = new char[LOCATION_SIZE_LIMIT];
	    strcat(server_name, DEBUG_STR);
	    strcat(server_name,tmp_str);

	    user_name = new char[LOCATION_SIZE_LIMIT];
	    cout << "User name ? " << flush;
	    cin.getline(user_name, LOCATION_SIZE_LIMIT);
	}
	else
	{
	    cout << "CalendarCreate: usage: CalendarCreate [-d]\n";
	    exit(1);
	}
    }

    cout << "Default location ? " << flush;
    cin.getline(location, LOCATION_SIZE_LIMIT);

    ClientRpc *RpcClient = new ClientRpc("Calendar");
#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
    RpcClient->SetTimeout(3000);
#endif
    RpcControl *RpcCon = new RpcControl(RpcClient);
    Calendar *my_calendar = new Calendar(res, server_name, user_name, location, RpcCon);

    if (res == 0)
	cout << "Calendar object creation failed !\n" << flush;
    else
	cout << "Calendar object successfully created !\n" << flush;

    delete my_calendar;
    
    delete RpcCon;
    delete RpcClient;
    
    return 0;
}




