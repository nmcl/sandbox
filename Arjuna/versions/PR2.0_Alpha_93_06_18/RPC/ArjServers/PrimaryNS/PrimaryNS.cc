/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: PrimaryNS.cc,v
 */

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef PRIMARYNS_H_
#  include <RPC/ArjServers/PrimaryNS.h>
#endif


PrimaryNS::PrimaryNS (int& res)
                              : index(0),
				first(TRUE),
				got(FALSE),
				AN(0),
				IsPrimary(FALSE)
{
    int opcode = -1;
    NS = new NSInterface(opcode);

    if (opcode == 0)
	res = 0;
}

PrimaryNS::~PrimaryNS ()
{
    if (Z)
    {
	if (Z->status() == RUNNING)
	{
	    error_stream << WARNING << "No call to PrimaryNS End or Abort operation\n" << flush;
	    error_stream << WARNING << "Will abort operation\n" << flush;
	    Z->Abort();
	}

	delete Z;
    }

    if (NS)
	delete NS;
}

ActionStatus PrimaryNS::Begin ()
{
    if (Z)
    {
	error_stream << WARNING << "Attempt to begin already running PrimaryNS\n" << flush;
	return Z->status();
    }

    Z = new AtomicAction;
    return Z->Begin();
}

ActionStatus PrimaryNS::End ()
{
    if (!Z)
    {
	error_stream << WARNING << "Attempt to end non-running PrimaryNS\n" << flush;
	return INVALID;
    }

    return Z->End();
}

ActionStatus PrimaryNS::Abort ()
{
    if (!Z)
    {
	error_stream << WARNING << "Attempt to abort non-running PrimaryNS\n" << flush;
	return INVALID;
    }

    return Z->Abort();
}

Boolean PrimaryNS::fetchNameList (const ArjunaName& scanFor)
{
    Boolean done = FALSE;
    Buffer key_buff;
    
    if (!Z)
    {
	error_stream << WARNING << "Must call PrimaryNS::Start before obtaining NameList\n" << flush;
	return done;
    }

    scanFor.pack(key_buff);
    NS->fetchName(done, key_buff, NameList);
    key = key_buff;
    return done;
}

ArjunaName* PrimaryNS::findPrimary ()
{
    Boolean found = FALSE;
    ArjunaName* AN1 = 0;

    AN1 = new ArjunaName;
    AN1->unpack(NameList);
    NameList.unpack(IsPrimary);
    NameList.unpack(number);

    if (IsPrimary)
	found = TRUE;

    for (int i = 0; ((i < number) && (!found)); i++)
    {
	if (AN1)
	    delete AN1;

	AN1 = new ArjunaName;
	AN1->unpack(NameList);
	NameList.unpack(IsPrimary);
	first = FALSE;
	if (IsPrimary)
	    found = TRUE;
    }

    if ((AN1) && (!found))
    {
	delete AN1;
	AN1 = 0;
    }
    
    return AN1;
}

void PrimaryNS::scanPast (int inx, Buffer& buff, Boolean saveInto)
{
    ArjunaName ANtemp;
    Boolean tempFlag;

    NameList.reread();
    ANtemp.unpack(NameList);
    NameList.unpack(tempFlag);
    NameList.unpack(number);

    if (saveInto)
    {
	ANtemp.pack(buff);
	buff.pack(tempFlag);
	buff.pack(number);
    }

    for (int i = 0; i < inx; i++)
    {
	if (AN)
	    delete AN;

	AN = new ArjunaName;
	AN->unpack(NameList);
	NameList.unpack(IsPrimary);

	if (saveInto)
	{
	    AN->pack(buff);
	    buff.pack(IsPrimary);
	}
    }

    if (AN)
    {
	delete AN;
	AN = 0;
    }
}

void PrimaryNS::scanAfter (int inx, Buffer& buff)
{
    for (int i = inx+1; i < number; i++)
    {
	if (AN)
	    delete AN;
	AN = new ArjunaName;
	AN->unpack(NameList);
	NameList.unpack(IsPrimary);

	AN->pack(buff);
	buff.pack(FALSE);
    }
}

ArjunaName* PrimaryNS::getPrimary ()
{
    Buffer buff;

    if (NameList.length() == 0)
	return 0;

    if (index > number)
	return 0;

    got = TRUE;

    if (first)
    {
	AN = findPrimary();
	if (AN)
	    return AN;
    }

    AN = new ArjunaName;
    if (index != 0)
    {
	scanPast(index, buff);

	AN->unpack(NameList);
	NameList.unpack(IsPrimary);
    }
    else
    {
	int tempNumb;

	AN->unpack(NameList);
	NameList.unpack(IsPrimary);
	NameList.unpack(tempNumb);
    }

    index++;

    return AN;
}

Boolean PrimaryNS::registerPrimary ()
{
    Buffer newbuff;
    ArjunaName AN, ANtemp;
    Boolean IsPrimary = FALSE, done = FALSE;
    int number;

    if (!got)
	return FALSE;

    NameList.reread();

    ANtemp.unpack(NameList);
    NameList.unpack(IsPrimary);
    NameList.unpack(number);

    if (index != 0)
    {
	scanPast(index, newbuff, TRUE);
	AN.unpack(NameList);
	NameList.unpack(IsPrimary);
	AN.pack(newbuff);
	newbuff.pack(IsPrimary);
    }
    else
    {
	ANtemp.pack(newbuff);
	newbuff.pack(IsPrimary);
    }

    scanAfter(index, newbuff);

    NS->replaceName(done, key, newbuff);
    return done;
}
