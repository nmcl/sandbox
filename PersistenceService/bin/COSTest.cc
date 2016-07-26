#include <os/string.h>
#include <os/stdlib.h>
#include <os/sys/types.h>
#include <os/iostream.h>

#include <Common/Boolean.h>

#include <Interface/CosPersistencePID.h>

#include <Implementation/Arjuna_PIDFactory.h>

#include "PObject.h"

void usage (char* name)
{
    cout << "Usage: " << name << " [-value <number>] [-pid <pid>] [-del] [-help]" << endl;
}

int main (int argc, char** argv)
{
    CORBA::Environment env;
    PersistentObject* theObject = (PersistentObject*) 0;
    long value = -1;
    long nValue = 10;
    char* pid = (char*) 0;
    CORBA::Boolean doDelete = FALSE;
    CosPersistencePID::PID_ptr thePID = CosPersistencePID::PID::_nil();
    
    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-value") == 0)
	{
	    nValue = atoi(argv[i+1]);
	    i++;
	}
	if (::strcmp(argv[i], "-pid") == 0)
	{
	    pid = argv[i+1];
	    i++;
	}
	if (::strcmp(argv[i], "-del") == 0)
	    doDelete = TRUE;
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    return 0;
	}
    }

    if (pid)
    {
	thePID = Arjuna_PIDFactory::createFromString(pid);
	theObject = new PersistentObject(thePID);
    }
    else
	theObject = new PersistentObject();
    
    theObject->get(value);

    cout << "Value is: " << value << endl;

    theObject->set(nValue+value);

    cout << "Set value to: " << nValue << endl;

    theObject->get(value);

    cout << "Value is now: " << value << endl;

    if (pid == (char*) 0)
    {
	thePID = CosPersistencePID::PID::_duplicate(theObject->p(env));
    
	pid = thePID->get_PIDString(env);

	cout << "pid is: " << pid << endl;

	::delete [] pid;
    }
    
    CORBA::release(thePID);

    if (doDelete)
    {
	cout << "Deleting object state." << endl;

	theObject->deleteState(CosPersistencePID::PID::_nil(), env);
    }
    
    delete theObject;

    return 0;
}
