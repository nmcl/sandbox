/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecType.cc,v 1.3 1995/02/10 11:42:09 ngdp Exp $
 */

#ifndef CRRECTYPE_H_
#  include <CRRecType.h>
#endif

/*
 * Enum: Crash Recovery Record Type (CRRecordType)
 * Types of crash recovery records
 */

ostream& operator<<  ( ostream& strm, CRRecordType t )
{
    switch (t)
    {
        case AARECORD:
	    strm << "local AA record";
	    break;
        case SAARECORD:
	    strm << "server AA record";
	    break;
        case SHADOWRECORD:
	    strm << "shadow state record";
	    break;
        case REPRECORD:
	    strm << "replication";
	    break;

	default:
	    strm << "** Unknown **";
	    break;
    }

    return strm;
}
