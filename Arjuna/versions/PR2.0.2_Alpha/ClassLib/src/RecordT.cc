/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordT.cc,v 1.2 1993/12/14 11:31:11 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

/*
 *
 * RecordType Class
 *
 */

static const char RCSid[] = "$Id";

ostream& operator<<  ( ostream& strm, RecordType rt )
{
    switch (rt)
    {
        case USER_DEF_FIRST0:
	    strm << "USER_DEF_FIRST0";
	    break;
        case USER_DEF_FIRST1:
	    strm << "USER_DEF_FIRST1";
	    break;
        case USER_DEF_FIRST2:
	    strm << "USER_DEF_FIRST2";
	    break;
        case USER_DEF_FIRST3:
	    strm << "USER_DEF_FIRST3";
	    break;
        case USER_DEF_FIRST4:
	    strm << "USER_DEF_FIRST4";
	    break;
        case USER_DEF_FIRST5:
	    strm << "USER_DEF_FIRST5";
	    break;
        case USER_DEF_FIRST6:
	    strm << "USER_DEF_FIRST6";
	    break;
        case USER_DEF_FIRST7:
	    strm << "USER_DEF_FIRST7";
	    break;
        case USER_DEF_FIRST8:
	    strm << "USER_DEF_FIRST8";
	    break;
        case USER_DEF_FIRST9:
	    strm << "USER_DEF_FIRST9";
	    break;

        case LOCK:
	    strm << "LOCK";
	    break;
        case RECOVERY:
	    strm << "RECOVERY";
	    break;
        case PERSISTENCE:
	    strm << "PERSISTENCE";
	    break;
        case CADAVER:
	    strm << "CADAVER";
	    break;
	case ACTIVATION:
	    strm << "ACTIVATION";
	    break;
	case REPLICATION:
	    strm << "REPLICATION";
	    break;
        case NAMING:
	    strm << "NAMING";
	    break;
        case RPCCALL:
	    strm << "RPCCALL";
	    break;
        case RPCINITIATE:
	    strm << "RPCINITIATE";
	    break;
        case RPCTERMINATE:
	    strm << "RPCTERMINATE";
	    break;
        case RPCCADAVER:
	    strm << "RPCCADAVER";
	    break;

        case USER_DEF_LAST0:
	    strm << "USER_DEF_LAST0";
	    break;
        case USER_DEF_LAST1:
	    strm << "USER_DEF_LAST1";
	    break;
        case USER_DEF_LAST2:
	    strm << "USER_DEF_LAST2";
	    break;
        case USER_DEF_LAST3:
	    strm << "USER_DEF_LAST3";
	    break;
        case USER_DEF_LAST4:
	    strm << "USER_DEF_LAST4";
	    break;
        case USER_DEF_LAST5:
	    strm << "USER_DEF_LAST5";
	    break;
        case USER_DEF_LAST6:
	    strm << "USER_DEF_LAST6";
	    break;
        case USER_DEF_LAST7:
	    strm << "USER_DEF_LAST7";
	    break;
        case USER_DEF_LAST8:
	    strm << "USER_DEF_LAST8";
	    break;
        case USER_DEF_LAST9:
	    strm << "USER_DEF_LAST9";
	    break;

        case UNTYPED:
	    strm << "UNTYPED";
	    break;

        case NONE_RECORD:
	    strm << "NONE_RECORD";
	    break;

	default:
	    strm << "** Unknown **";
	    break;
    }

    return strm;
};
