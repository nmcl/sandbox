#ifndef CLIENT_H_
#  include "Client.h"
#endif

#ifndef SEQUENCENUMBER_H_
#  include "SequenceNumber.h"
#endif

#ifndef GROUPLIST_H_
#  include "GroupList.h"
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

const int groupSize = 1;


int main ()
{
    GroupList serverGroup(groupSize);
    Client myPort;
    Boolean res = TRUE;
    Buffer myAddr, result;
    SequenceNumber sn;
    
    for (int i = 0; ((i < groupSize) && res); i++)
	res = serverGroup.addName("ServerTest");

    if (!res)
    {
	cout << "Error in adding name" << endl;
	return -1;
    }
    
    res = myPort.Initiate(serverGroup, 1);
    
    if (!res)
    {
	cout << "Initiate failed" << endl;
	return 1;
    }
    
    myPort.pack(myAddr);
    
    if (!myPort.SendRequest(serverGroup, myAddr, sn))
	cout << "Send error" << endl;
    else
    {
	cout << "Sent msg ok" << endl;

	if (!myPort.ReceiveMessage(result, -1, sn))
	    cout << "Receive error" << endl;
	else
	    cout << "Received ok" << endl;

    }

    if (!myPort.Terminate(serverGroup))
	cout << "Terminate failed" << endl;

    return 0;
}


    
    
    
