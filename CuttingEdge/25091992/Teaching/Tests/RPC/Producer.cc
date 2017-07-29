/*
 * Copyright 1990, 1991, 1992,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

/*
 * $Id$
 */

#include <sysent.h>
#include <iostream.h>

#include <RPC/Client.h>

int main(int, char**)
{
    PortAddress portAddress(4500);

    Client client(portAddress);

    int value = 100;
    while (value >= -1)
    {
	Buffer workBuff, resultBuff;

	workBuff.pack(value);

	client.Call(workBuff, resultBuff);

	resultBuff.unpack(value);

	cout << "value = " << value << "\n";
	cout.flush();
    }

    exit(0);
}













