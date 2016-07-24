/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecType.h,v 1.3 1994/08/04 08:59:54 ndbi Exp $
 */

#ifndef CRRECTYPE_H_
#define CRRECTYPE_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

/*
 * Enum: Crash Recovery Record Type (CRecRecordType)
 * Types of crash recovery records
 */

enum CRRecordType
{
    UNKNOWN_CR_REC_TYPE, AARECORD, SAARECORD, SHADOWRECORD, REPRECORD
};

class ostream;
extern ostream& operator<<  ( ostream& strm, CRRecordType t );

#endif // CRRECTYPE_H_
