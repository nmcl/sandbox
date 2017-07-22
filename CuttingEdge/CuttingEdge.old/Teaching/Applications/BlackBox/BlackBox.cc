/*
 *  $Id: BlackBox.cc,v 1.2 1992/02/27 10:29:47 nsmw Exp $
 */

#include <iostream.h>

#include <Port/Port.h>
#include <Port/PortAddress.h>

#include <Common/Buffer.h>

int main(int, char**)
{
    PortAddress sendersPortAddress;
    PortAddress myPortAddress(4242);

    Port        port(myPortAddress);

    Boolean res, finish;
    int value1, value2, value3, value4, valuetmp;

    cerr << "BlackBox :: BlackBox port is [" << port << "], address [" << myPortAddress << "]\n\n";
    cerr.flush();

    finish = ! port.Valid();

    while (! finish)
    {
	Buffer recBuff, sendBuff;

	if (port.ReceiveMessage(sendersPortAddress, recBuff) != Port::Done)
	{
	    cerr << "BlackBox :: Receive message failed.\n";
	    cerr.flush();

	    finish = TRUE;
	}
	else
	{
#ifdef DEBUG
	    cerr << "BlackBox :: Received message from [" << sendersPortAddress << "]\n";
	    cerr.flush();
#endif

	    res = recBuff.unpack(value1) && recBuff.unpack(value2) &&
		  recBuff.unpack(value3) && recBuff.unpack(value4);

	    if (res)
	    {
		valuetmp = value1;
		value1   = value2;
		value2   = valuetmp;

		value3 = value3 + 10;
		value4 = value4 - 10;

		res = sendBuff.pack(value1) && sendBuff.pack(value2) &&
		      sendBuff.pack(value3) && sendBuff.pack(value4);

		if (res)
		{
		    if (port.SendMessage(sendersPortAddress, sendBuff) != Port::Done)
		    {
			cerr << "BlackBox :: Send message failed.\n";
			cerr.flush();

			finish = TRUE;
		    }
#ifdef DEBUG
		    else
		    {
			cerr << "BlackBox :: Sent reply.\n";
			cerr.flush();
		    }
#endif
		}
		else
		{
		    cerr << "BlackBox :: Pack reply failed.\n";
		    cerr.flush();
		}
	    }
	    else
	    {
		cerr << "BlackBox :: Unpack reply failed.\n";
		cerr.flush();
	    }
	}
#ifdef DEBUG
	cerr << "\n";
	cerr.flush();
#endif
    }

    cerr << "BlackBox :: Terminated.\n";
    cerr.flush();
}
