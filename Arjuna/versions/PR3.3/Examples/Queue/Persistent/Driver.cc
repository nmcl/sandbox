/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Driver.cc,v 1.3 1994/07/08 15:27:51 nmcl Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif


int main (int, char**)
{
    Boolean res = FALSE;
    Queue q(res);
    QueueStatus status = DONE;
    int value = 0;
    
    if (res == FALSE)
    {
	cerr << "Constructor error." << endl;
	return -1;
    }
    
    for (int i = 0; (i < 5) && (status == DONE); i++)
    {
	cout << "Adding number " << i << " to queue" << endl;
	status = q.enqueue(i);

	if (status == DONE)
	    cout << "Added successfully\n" << endl;
	else
	    cout << "An error has occurred" << endl;
    }

    if (status != DONE)
	return -1;
    
    for (i = 0; status == DONE; i++)  // deliberately force an underflow!
    {
	cout << "Removing value" << endl;
	
	status = q.dequeue(value);
	
	if (status == DONE)
	    cout << "Have removed " << value << "\n" << endl;
	else
	    cout << "An error has occurred" << endl;
    }
    
    if (status == UNDER_FLOW)
	cout << "End of queue." << endl;
    
    return 0;
}

