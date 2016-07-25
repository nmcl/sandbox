/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerList.h,v 1.1 1993/11/03 12:22:53 nmcl Exp $
 */

class ArjunaName;

class ServerList
{
public:
    ServerList();
    ~ServerList();

    void Clear();
    int  Count();
    void ScanNameServer();
    void Delete(int index);
    void Add(ArjunaName *ServerName);
    ArjunaName *Get(int index);
    ArjunaName *GetMyServer();

protected:
    struct ListElement
    {
	ArjunaName *server;
	ListElement *next;
    };

    ListElement *servers;
    ArjunaName *user_server;
    int number_of_servers;
};
