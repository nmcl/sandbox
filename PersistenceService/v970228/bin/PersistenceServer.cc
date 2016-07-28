#include <os/string.h>
#include <os/stdlib.h>
#include <os/sys/types.h>
#include <os/iostream.h>

#include <Common/Boolean.h>

#include <Interface/PDSFactory.h>

#include <Implementation/Arjuna_PDSFactory.h>

int main (int, char**)
{
    Arjuna_PDSFactory theFactory("foo");

    try
    {
	CORBA::Orbix.impl_is_ready("PersistenceServer");
    }
    catch (const CORBA::SystemException& se)
    {
	cerr << "Unexpected Exception: " << &se << endl;
	return -1;
    }

    cout << "PersistenceServer terminating." << endl;

    return 0;
}
