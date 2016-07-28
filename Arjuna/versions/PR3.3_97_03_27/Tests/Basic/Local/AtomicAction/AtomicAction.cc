/*
 * Copyright 1990, 1991, 1992
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
 *
 * $Id: AtomicAction.cc,v 1.2 1994/09/14 08:56:14 ngdp Exp $
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

main(int argc, char** argv)
{
    AtomicAction A, B;

    if (argc != 3)
	cerr << "Error : Insufficient Parameters\n";
    else
    {
	A.Begin();
	cout << "A.Begin : Started\n";

	B.Begin();
	cout << "B.Begin : Started\n";

	if (strcmp(argv[2], "COMMIT") == 0)
	{
	    if (B.End() != COMMITTED)
		cerr << "B.End() : Not Done\n";
	    else
		cout << "B.End() : Done\n";
	}
	else
	{
	    B.Abort();
	    cout << "B.Abort : Done\n";
	}

	if (strcmp(argv[1], "COMMIT") == 0)
	{
	    if (A.End() != COMMITTED)
		cerr << "A.End() : Not Done\n";
	    else
		cout << "A.End() : Done\n";
	}
	else
	{
	    A.Abort();
	    cout << "A.Abort : Done\n";
	}
    }
    return 0;
}
