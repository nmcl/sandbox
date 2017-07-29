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

#include <RPC/Server.h>

int main(int argc, char* argv[])
{
    Server server(4500);
//    Server server(argc, argv);

    int value = 0;
    while (value >= 0)
    {
	Buffer workBuff, resultBuff;

	server.GetWork(workBuff);

	workBuff.unpack(value);

	cout << "value = " << value << "\n";
	cout.flush();

	resultBuff.pack(value - 1);

	server.SendResult(resultBuff);
    }

    exit(0);
}







