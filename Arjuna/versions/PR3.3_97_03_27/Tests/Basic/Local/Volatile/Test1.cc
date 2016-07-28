/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test1.cc,v 1.3 1995/07/03 14:30:22 ngdp Exp $
 */

#include "Object1.h"

#include <System/iostream.h>
#include <System/stdlib.h>
#include <System/string.h>
#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

int ParseArg(int argc, char *argv[], ObjectType& ot, int& nobj)
{
    int err = 0;
    int i = 0;

    for (i = 1; (i < argc) && (err == 0); i++)
    {
	if ((i < argc) && (strcmp(argv[i], "-per") == 0))
	    ot = ANDPERSISTENT;
	else if ((i < argc) && (strcmp(argv[i], "-rec") == 0))
	    ot = RECOVERABLE;
        else if ((i < argc) && (strcmp(argv[i], "-num") == 0)) {
	    i++;
	    if (i < argc) {
		nobj = atoi(argv[i]);
		if (nobj<1) {
		    err = 1;
		    cout << "Number of object instances"
			 << " must be greater than 0.\n";
		}
	    }
	    else {
		err = 1;
		cout<<"No number given.\n";
	    }
	}
        else {
	    err = 1;
	    cout<<"Invalid parameter ["<<argv[i]<<"].\n";
	}
    }
    return err;
}



main(int argc, char** argv) {
    ObjectType obj_type = RECOVERABLE;
    int nobjects = 10;
    int err = 0;
    int i = 0;
    
    err = ParseArg(argc, argv, obj_type, nobjects);
    if (err) exit(-1);

    cout << "\nTest creation, modification and access of multiple "
	 << "object instances\n";
    cout << "requested object type= " << obj_type << endl;
    cout << "number of objects requested= " << nobjects << endl;

    Boolean lock;
    int res = 0;
    int val;
    
    Object1** copyX = new Object1*[nobjects];
    Object1** x = new Object1*[nobjects];
    Uid* uid = new Uid[nobjects];
    if (!x || !copyX || !uid)
	exit(-1);
    for (i=0; i<nobjects; i++) {
	x[i] = copyX[i] = 0;
	x[i] = new Object1(obj_type, uid[i], res);
	if (!res) {
	    cerr << "Unable to create object[" << i << "]" << endl;
	    x[i] = 0;
	    continue;
	}
	AtomicAction A;
	A.Begin();
	cout << "A.Begin : Started\n";
	cout << "Setting value to " << i << "\n";
	lock = x[i]->change(i, val);
	if (!lock) {
	    cout << "Could not acquire lock\n";
	    A.Abort();
	    lock = 0;
	}
	else {
	    lock = x[i]->get(val);
	}
	if (!lock) {
	    cerr << "An error has occurred while attempting"
		 << "to read object state\n" << flush;
	    A.Abort();
	}
	else {
	    if (val == i)
		cout << "Value returned is " << i 
		     << ". This is correct.\n" << flush;
	    else
		cerr << "An error has occurred. The state returned is " 
		     << val << " and not " << i << "\n" << flush;

	    if (A.End() != COMMITTED)
		cout << "\nA.End() : Not Done\n";
	    else {
		lock = x[i]->get(val);
		cout << "A.End() : Done\n";

		if (!lock)
		    cerr << "An error has occurred while attempting "
			 << "to read object state\n" << flush;
		else {
		    if (val == i)
			cout << "Final value returned is " << i 
			     << ". This is correct.\n" << flush;
		    else
			cerr << "An error has occurred. "
			     << "The state returned is " << val 
			     << " and not " << i << "\n" << flush;
		}
	    }
	}
    }

    cout << "\nLoading object from ObjectStore\n";

    res = 0;
    for (i=0; i<nobjects; i++) {
	copyX[i] = new Object1(uid[i], obj_type, res);

	if (res == 0) {
	    cerr << "Constructor error for object[" << i << "]\n";
	    copyX[i] = 0;
	}
	else {
	    AtomicAction C;
	    C.Begin();
	    cout << "C.Begin : Started\n";
	    lock = copyX[i]->get(val);
	    if (!lock)
		cerr << "An error has occurred while attempting" 
		     << " to read object state\n" << flush;
	    else {
		if (val == i)
		    cout << "Value returned is " << i 
			 << ". This is correct.\n" << flush;
		else
		    cerr << "An error has occurred. " 
			 << "The state returned is " << val << " and not " 
			 << i << "\n" << flush;
	    }

	    if (C.End() != COMMITTED)
		cout << "C.End() : Not Done\n";
	    else
		cout << "C.End() : Done\n";
	}
    }
    
    for (i=0; i<nobjects; i++)
    {
	delete x[i];
	delete copyX[i];
    }
    delete x;
    delete copyX;
    delete uid;

    return 0;
}
