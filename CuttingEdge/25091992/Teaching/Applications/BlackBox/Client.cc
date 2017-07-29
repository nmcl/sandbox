/*
 *  $Id: Client.cc,v 1.1 1992/02/26 22:31:47 nsmw Exp $
 */

#include <iostream.h>

#include <Port.h>
#include <PortAddress.h>

#include <Common/Buffer.h>

int main(int, char**)
{
    PortAddress portAddress;
    PortAddress daemonPortAddress("semillon", 4242);

    Port        port;
    
    int value1, value2, value3, value4;

    Buffer recBuff, sendBuff;

    cout << "Value 1: ";
    cin  >> value1;
    cout << "Value 2: ";
    cin  >> value2;
    cout << "Value 3: ";
    cin  >> value3;
    cout << "Value 4: ";
    cin  >> value4;

    sendBuff.pack(value1);
    sendBuff.pack(value2);
    sendBuff.pack(value3);
    sendBuff.pack(value4);

    port.SendMessage(daemonPortAddress, sendBuff);
    port.ReceiveMessage(portAddress, recBuff);

    recBuff.unpack(value1);
    recBuff.unpack(value2);
    recBuff.unpack(value3);
    recBuff.unpack(value4);

    cout << "\n";
    cout << "Value 1: " << value1 << "\n";
    cout << "Value 2: " << value2 << "\n";
    cout << "Value 3: " << value3 << "\n";
    cout << "Value 4: " << value4 << "\n";
}
