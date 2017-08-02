/*
 * Calendar Queue for Process List
 *     - See Randy Brown, CACM 30(10): 1220-1227 (10/88).
 *     - See also SIMPACK++ code, Paul Fishwick, cis.ufl.edu.
 *
 * Ian.Mathieson@mel.dit.CSIRO.AU, 11 March 1994.
 */

/*
 * Modified slightly for new C++SIM format (release >= 1.5)
 * by M.C.Little@uk.ac.newcastle.
 * 22/3/94
 */

#include <iostream.h>

#ifndef CALENDAR_H_
#  include "Calendar.h"
#endif

#ifndef PROCESSITERATOR_H_
#  include "ProcessIterator.h"
#endif


Calendar::Calendar ()
{
    Init(0, 2, 1.0, 0.0);
    resizeEnable = TRUE;
}

const Process* Calendar::getNext (const Process* current) const
{
    if (current == (Process*) 0)
	return (Process*) 0;
    
    for (int j = 0; j < nBuckets; j++)
    {
	ProcessIterator iter(*calendar[j]);
	for (Process* p = iter(); p; p = iter())
	    if (p == current)
		return calendar[j]->getNext(current);
    }

    return (Process*) 0;
}

// Initialise calendar array within the array calQ[].
// Set bucket width to bWidth. Set calendar[0] to calQ[qBase].
// Number of buckets is nBuck. StartPrio is initial priority.
// Set up all data members apart from resizeEnable.

void
Calendar::Init(int qBase,int nBuck, double bWidth, double startPrio)
{
	// set position and size of new queue
    firstSub = qBase;
    calendar = calQ+qBase;
    width = bWidth;
    nBuckets = nBuck;

	// initially empty
    qSize = 0;
    for (int i = 0; i < nBuckets; i++) {
	if ( calendar[i] ) delete calendar[i];
	calendar[i] = new ProcessList();
    }

	// set up initial position in queue
    lastPrio = startPrio;
    long n = long(startPrio/width);	// virtual bucket
    lastBucket = int(n % nBuckets);
    bucketTop = BucketTop(lastPrio);

	// set up queue size change thresholds
    botThreshold = nBuckets/2 - 2;
    topThreshold = 2 * nBuckets;
}

// Insert process into calendar based on its scheduled event time.
// Insert before matching times if prior is TRUE, otherwise append.

void
Calendar::Insert (Process &p, Boolean prior)
{
	// calculate the bucket number for the entry
    int i = WhichBucket(p.evtime());

	// insert into calendar[i]
    calendar[i]->Insert(p,prior);

	// update size of queue
    qSize++;

	// double calendar size if needed
    if ( qSize > topThreshold ) Resize( 2*nBuckets );
}

// Insert process TI immediately before B in calendar.

Boolean
Calendar::InsertBefore (Process &ToInsert, Process &Before)
{
	// calculate the bucket number for the entry
    int i = WhichBucket(Before.evtime());

	// insert into calendar[i]
    Boolean b = calendar[i]->InsertBefore(ToInsert,Before);

	// update size of queue
    qSize++;

	// double calendar size if needed
    if ( qSize > topThreshold ) Resize( 2*nBuckets );

    return b;
}

// Insert process TI immediately after A in calendar.

Boolean
Calendar::InsertAfter (Process &ToInsert, Process &After)
{
	// calculate the bucket number for the entry
    int i = WhichBucket(After.evtime());

	// insert into calendar[i]
    Boolean b = calendar[i]->InsertAfter(ToInsert,After);

	// update size of queue
    qSize++;

	// double calendar size if needed
    if ( qSize > topThreshold ) Resize( 2*nBuckets );

    return b;
}

// Remove and return the specified or lowest priority process from calendar.

Process *
Calendar::Remove (const Process *element)
{
    ProcessList *pl;	// indicates the correct list to remove from
    int i;		// likewise for the correct bucket
    Process* p = (Process*) 0;

	// take care of boundary condition - empty list
    if ( qSize == 0 ) return 0;

	// remove specified element if it's there ...
    if (element) {

	i = WhichBucket(element->evtime());
	pl = calendar[i];

    } else {

	// ... otherwise remove lowest priority process
	    // search buckets once
	for ( i = lastBucket;; ) {
		// check calendar[i] 
	    pl = calendar[i];
	    ProcessIterator iter(*pl);
	    p = iter();
	    if ( p && p->evtime() < bucketTop ) {
		goto FOUND;
	    } else {
		    // do next bucket or direct search
		if ( ++i == nBuckets ) i = 0;
		bucketTop += width;
		if ( i == lastBucket ) break;	// direct search
	    }
	}

	    // direct search to find earliest
	double earliest = 0;
	i = -1;
	for ( int j = 0; j < nBuckets; j++ ) {
	    pl = calendar[j];
	    ProcessIterator iter(*pl);
	    p = iter();
	    if ( p && ( i == -1 || p->evtime() < earliest ) ){
		i = j;
		earliest = p->evtime();
	    }
	}
	pl = calendar[i];
    }

FOUND:

	// remove process from list and update/resize as needed
    p = pl->Remove(element);
    if ( p ) {
	lastBucket = i;
	lastPrio = p->evtime();
	bucketTop = BucketTop(lastPrio);
	qSize--;
	if ( qSize < botThreshold ) Resize( nBuckets/2 );
    }
    return p;
}

// Print out the event times of each process on the calendar,
// plus the calendar configuration.

ostream&
Calendar::print (ostream& strm) const
{
    for ( int i = 0; i < nBuckets; i++ ) {
	strm << "\nBucket " << i << "\n";
	strm << i << (i==lastBucket) ? ">" : " ";
	ProcessIterator iter(*calendar[i]);
	for ( Process *p = iter(); p; p = iter() ) {
	    strm << p->evtime() << "\n";
	}
	strm << "\n";
    }
    strm << "qSize == " << qSize << "\n";
    strm << "lastPrio == " << lastPrio << "\n";
    strm << "bucketTop == " << bucketTop << "\n";
    strm << "width == " << width << "\n";
    strm << "topThreshold == " << topThreshold << "\n";
    strm << "botThreshold == " << botThreshold << "\n";
    strm << "firstSub == " << firstSub << "\n";

    return strm;
}

// Move the current queue items into a bigger/smaller set of buckets
// at the opposite end of calQ from the original.

void
Calendar::Resize(int sz)
{
    if ( !resizeEnable ) return;
    if ( sz > MAXNBUCKETS || sz < 1 ) return;

	// preserve location and size of current calendar
    ProcessList **oldC = calendar;
    int oldN = nBuckets;

	// setup new calendar
    Init( (firstSub ? 0 : QSPACE-sz), sz, NewWidth(), lastPrio );

	// move list elements onto new calendar
    for ( int i = 0; i < oldN; i++ ) {
	ProcessList *pl = oldC[i];
	Process *p;
	while ( p = pl->Remove() ) {
	    Insert( *p );
	}
    }
}

// Randomly sample the queues and calculate a new width to
// accomodate roughly 3 items per bucket.

double
Calendar::NewWidth()
{
        // decide how many elements to sample
    if ( qSize < 2 ) return 1.0;
    int nsamples = ( qSize <= 5 ) ? qSize : 5 + qSize/10;
    const int maxSamples = 25;
    if ( nsamples > maxSamples ) nsamples = maxSamples;

	// record current configuration and disable resize
    int oldLB = lastBucket;
    double oldLP = lastPrio;
    double oldBT = bucketTop;
    resizeEnable = FALSE;

	// remove nsample events and calculate average event separation
    double ave = 0.0;
    Process *tmp[maxSamples];
    tmp[0] = Remove();
    for ( int i = 1; i < nsamples; i++ ) {
	tmp[i] = Remove();
	ave += tmp[i]->evtime() - tmp[i-1]->evtime();
    }
    ave /= nsamples-1;

	// restore samples and recalculate ignoring large separations
	// restore in reverse and insert before to maintain original ordering
    double ave2 = 0.0;
    int count = 0;
    for ( i = nsamples; --i > 0; ) {
	double delta = tmp[i]->evtime() - tmp[i-1]->evtime();
	if ( delta < ave * 2.0 ) {
	    ave2 += delta; count++;
	}
	Insert( *tmp[i], TRUE );
    }
    Insert( *tmp[0], TRUE );
    ave2 /= count;
    double newW = 3 * ave2;

	// restore configuration and enable resize again
    lastBucket = oldLB;
    lastPrio = oldLP;
    bucketTop = oldBT;
    resizeEnable = TRUE;

    return newW;
}


#ifdef NO_INLINES
#  define CALENDAR_CC_
#  include "Calendar.n"
#  undef CALENDAR_CC_
#endif
