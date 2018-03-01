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
 * $Id: BadActionCode.cc,v 1.4.6.1 1996/10/10 12:27:07 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

main(int, char**)
{
    AtomicAction A, B;

    A.Begin();
    cout << "A.Begin : Started\n";

    B.Begin();
    cout << "B.Begin : Started\n";

    if (AtomicAction::Current() == &B)
	cout << "Current action is B\n";
    else
	if (AtomicAction::Current() == &A)
	    cout << "Current action is A\n";

    cout << "Attempting to commit action A\n";
    if (A.End() != COMMITTED)
    {
	cerr << "A.End() : Not Done\n";

	ErrorList& eList = A.getErrorList();
	Error *error;

	while ((error = eList.pop()))
	{
	    cerr << *error;
	    delete error;
	    
	}
	
	if (AtomicAction::Current() == &B)
	    cout << "Current action is B\n";
	else
	    if (AtomicAction::Current() == &A)
		cout << "Current action is A\n";
	    else
		cout << "No actions running\n";
	
	if (B.End() != COMMITTED)
	    cerr << "B.End() : Not Done\n";
	else
	    cout << "B.End() : Done\n";

	if (A.End() != COMMITTED)
	    cerr << "A.End() : Not Done\n";
	else
	    cout << "A.End() : Done\n";
    }
    else
    {
	cerr << "A.End() : Done\n";

	if (B.End() != COMMITTED)
	    cerr << "B.End() : Not Done\n";
	else
	    cout << "B.End() : Done\n";
    }
    return 0;
}
