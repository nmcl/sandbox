#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif

#ifndef SEQUENCENUMBER_H_
#  include "SequenceNumber.h"
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef GROUPID_H_
#  include "GroupID.h"
#endif


int main (int argc, char** argv)
{
    Server myPort;
    SequenceNumber sn;
    Buffer work, result;
    Client hisAddr, myAddr;
    
    if (!myPort.Initialise(argc, argv))
    {
	cout << "Initialise error" << endl;
	return -1;
    }
    
    for (;;)
    {
	myPort.GetWork(sn, work);

	if (!hisAddr.unpack(work))
	    cout << "Unpack error" << endl;
	else
	{
	    GroupID X;
	    
	    X.addMember(&hisAddr);
	    
	    if (!myAddr.SendMessage(&X, result, sn))
		cout << "Send error" << endl;
	    else
		cout << "Sent ok" << endl;
	}
    }
}
