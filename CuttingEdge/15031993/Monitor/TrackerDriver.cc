#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#include <System/fstream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef TRACKER_H_
#include "Tracker.h"
#endif

#ifndef DEPENDENCY_H_
#include "Dependency.h"
#endif

char* defaultName = "DependencyData";
char* fileName = 0;

int main (int argc, char** argv)
{
    int opt, res = -1;
    Boolean finished = FALSE, found = FALSE;
    Tracker* TK = 0;
    
    for (int i = 0; i < argc; i++)
    {
	if (strcmp(argv[i], "-uid") == 0)
	{
	    Uid u_id(argv[i+1]);
	    TK = new Tracker(u_id, res);
	    found = TRUE;
	}

	if (strcmp(argv[i], "-file") == 0)
	    fileName = argv[i+1];

	if (strcmp(argv[i], "-help") == 0)
	{
	    cout << "TrackerDriver [-uid <UID>] [-file <file name>]\n" << flush;
	    return 2;
	}
    }

    if (!fileName)
	fileName = defaultName;
    
    if (!found)
    {
	TK = new Tracker(res);
    }

    if (res != 0)
    {
	cout << "An error occurred while attempting to create Tracker\n" << flush;
	return 1;
    }

    while (!finished)
    {
	cout << "\n1: Register Node\n" << flush;
	cout << "2: Unregister Node\n" << flush;
	cout << "3: Modify Node\n" << flush;
	cout << "4: Update Node\n" << flush;
	cout << "5: Calculate Dependencies\n" << flush;
	cout << "6: List Nodes\n" << flush;
	cout << "7: Show Dependencies\n" << flush;
	cout << "8: Output Data To File\n" << flush;
	cout << "9: Quit\n" << flush;

	cin >> opt;

	switch (opt)
	{
	case 1:
	{
	    char hostName[MAXHOSTNAMELEN];
	    memset(hostName, '\0', MAXHOSTNAMELEN);
	    struct timeval TTF, TTR;
	    AtomicAction A;

	    cout << "What is the name of the node to register?\n" << flush;
	    cin >> hostName;
    
	    cout << "What is the time of failure? <sec, usec>\n" << flush;
	    cin >> TTF.tv_sec >> TTF.tv_usec;

	    cout << "What is the time of recovery? <sec, usec>\n" << flush;
	    cin >> TTR.tv_sec >> TTR.tv_usec;

	    A.Begin();

	    if (TK->registerNode(hostName, TTF, TTR))
	    {
		if (A.End() == COMMITTED)
		    cout << "Registered ok\n" << flush;
		else
		    cout << "Action failure\n" << flush;
	    }
	    else
	    {
		A.Abort();
		cout << "A problem occurred while attempting to register node\n" << flush;
	    }
	}
	break;
        case 2:
        {
	    char hostName[MAXHOSTNAMELEN];
	    memset(hostName, '\0', MAXHOSTNAMELEN);
	    AtomicAction A;

	    cout << "What is the name of the node?\n" << flush;
	    cin >> hostName;

	    A.Begin();

	    if (TK->unregisterNode(hostName))
	    {
		if (A.End() == COMMITTED)
		    cout << "Node successfully removed\n" << flush;
		else
		    cout << "Action failure\n" << flush;
	    }
	    else
	    {
		A.Abort();
		cout << "A problem occurred while attempting to remove node\n" << flush;
	    }
	}
	break;
        case 3:
        {
	    char hostName[MAXHOSTNAMELEN];
	    memset(hostName, '\0', MAXHOSTNAMELEN);
	    struct timeval TTF, TTR;
	    AtomicAction A;

	    cout << "What is the name of the node to modify?\n" << flush;
	    cin >> hostName;

	    cout << "What is the time of failure? <sec, usec>\n" << flush;
	    cin >> TTF.tv_sec >> TTF.tv_usec;

	    cout << "What is the time of recovery? <sec, usec>\n" << flush;
	    cin >> TTR.tv_sec >> TTR.tv_usec;

	    A.Begin();

	    if (TK->modifyNode(hostName, TTF, TTR))
	    {
		if (A.End() == COMMITTED)
		    cout << "Node modified successfully\n" << flush;
		else
		    cout << "Action failure\n" << flush;
	    }
	    else
	    {
		A.Abort();
		cout << "A problem occurred while attempting to modify node\n" << flush;
	    }
	}
	break;
	case 4:
        {
	    AtomicAction A;
	    char hostName[MAXHOSTNAMELEN];
	    ::memset(hostName, '\0', MAXHOSTNAMELEN);
	    struct timeval TTF, TTR;
	    
	    cout << "What is the name of the node to update?" << endl;
	    cin >> hostName;
	    
	    cout << "What is the new time of failure? <sec, usec>" << endl;
	    cin >> TTF.tv_sec >> TTF.tv_usec;
	    
	    cout << "What is the new time of recovery? <sec, usec>" << endl;
	    cin >> TTR.tv_sec >> TTR.tv_usec;
	    
	    A.Begin();
	    
	    if (TK->updateNode(hostName, TTF, TTR))
	    {
		if (A.End() == COMMITTED)
		    cout << "Updated node successfully" << endl;
		else
		    cout << "Action error" << endl;
	    }
	    else
	    {
		A.Abort();
		cout << "Could not update node" << endl;
	    }
	}
	break;
        case 5:
        {
	    AtomicAction A;

	    A.Begin();

	    if (TK->doTrack())
	    {
		if (A.End() == COMMITTED)
		    cout << "Calculated dependencies\n" << flush;
		else
		    cout << "Action failure\n" << flush;
	    }
	    else
	    {
		A.Abort();
		cout << "A problem occurred while attempting to calculate dependencies\n" << flush;
	    }
	}
	break;
        case 6:
        {
	    AtomicAction A;
	    NodeData* ptr = 0;

	    A.Begin();

	    if (TK->listNodes(ptr))
	    {
		if (A.End() != COMMITTED)
		{
		    cout << "Action failure\n" << flush;
		    break;
		}
	    }
	    else
	    {
		A.Abort();
		cout << "A problem occurred while attempting to get list of nodes\n" << flush;
		break;
	    }

	    cout << "****\n" << flush;

	    if (ptr)
		cout << *ptr;
	    else
		cout << "No data" << endl;
	}
	break;
       case 7:
       {
	   AtomicAction A;

	   A.Begin();
	   
	   cout << "Number of nodes " << TK->getNumberOfNodes() << endl;
	   cout << *TK;

	   if (A.End() != COMMITTED)
	       cout << "Action error" << endl;
       }
	break;
       case 8:
       {
	   AtomicAction A;
	   int numberOfNodes = 0;

	   ofstream outFile(fileName, ios::out);
	   if (!outFile)
	   {
	       cout << "Could not open " << fileName << " for output\n" << flush;
	       break;
	   }
	   
	   A.Begin();

	   numberOfNodes = TK->getNumberOfNodes();
	   
	   if (numberOfNodes == -1)
	   {
	       cout << "Error while getting number of nodes" << endl;
	       A.Abort();
	       break;
	   }
	   
	   outFile << numberOfNodes << "\n";

	   outFile << *TK;

	   if (A.End() != COMMITTED)
	       cout << "Action error" << endl;
       }
	break;
       case 9:
	 finished = TRUE;
	 break; }
    }

    if (TK)
	delete TK;
}
