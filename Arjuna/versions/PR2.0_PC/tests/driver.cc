#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef ATOMICINT_H_
#  include "AtomInt.h"
#endif


int main (int, char**)
{
    AtomicAction A, B;
    int res = -1;
    AtomicInt X(res);

    if (res == 0)
    {
	cout << "Created object successfully" << endl;
	cout << "Object Uid is " << X.get_uid() << endl;
    }
    else
    {
	cout << "Creation error" << endl;
	return -1;
    }
    
    cout << "\nStarting top-level atomic action" << endl;

    A.Begin();

    if (X.get(res))
    {
	cout << "Current value is " << res << endl;
	if (X.set(10))
	{
	    cout << "Set value to 10" << endl;
	    if (X.get(res))
            {
		cout << "Value is now " << res << endl;
		cout << "\nStarting nested atomic action" << endl;

		B.Begin();
		
		if (X.set(20))
		{
		    cout << "Set value to 20" << endl;
		    if (X.get(res))
			cout << "Value is now " << res << endl;
		}
		else
		    cout << "Nested set error" << endl;

		cout << "Aborting nested action\n" << endl;

		B.Abort();

		if (X.get(res))
            	{
		    cout << "Value is now " << res << endl;
		    if (res != 10)
			cout << "This is incorrect" << endl;
		    else
			cout << "This is correct" << endl;
		}
		else
		    cout << "Nested get error" << endl;
            }
	    else
		cout << "Top-level get error" << endl;
	}
	else
	    cout << "Top-level set error" << endl;
    }
    else
	cout << "Top-level get error" << endl;

    cout << "starting to commit" << endl;

    if (A.End() == COMMITTED)
    {
	cout << "Top-level action committed\n" << endl;
	if (X.get(res))
	{
	    cout << "Final value is " << res << endl;
	    if (res == 10)
		cout << "This is correct" << endl;
	    else
		cout << "This is incorrect" << endl;
	}
	else
	    cout << "Final get error" << endl;
    }
    else
	cout << "Top-level commit error" << endl;

    return 0;
}
