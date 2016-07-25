/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileInfo.cc,v 1.6 1993/09/20 14:09:25 ngdp Exp $
 */

/*
 * Stub generator file information handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef FILEINFO_H_
#  include "FileInfo.h"
#endif

static const char RCSid[] = "$Id: FileInfo.cc,v 1.6 1993/09/20 14:09:25 ngdp Exp $";


FileInfo::FileInfo ( const String& dn, const String& fn )
                   : fileDname(dn),
		     fileFname(fn),
		     fileGeneric(STUB_DEFAULT),
		     warningsOn(TRUE)
{
}

void FileInfo::addStubModifier ( StubModifier newSm )
{
#ifdef DEBUG
    debugStream << "FileInfo::addStubModifier("  << (void *)this << "," 
		<< newSm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (newSm)
    {
    case STUB_DEFAULT:
    case STUB_NORENAME:
	if ((fileGeneric == STUB_DEFAULT)  || (fileGeneric == newSm))
	{
	    fileGeneric = newSm;
	    break;
	}
    case STUB_NOWARNINGS:
	warningsOn = FALSE;
	break;
    default:
	errorStream << newSm << " not applicable in #pragma\n";
	caution(errorStream);
    }
}

#ifdef NO_INLINES
#  define FILEINFO_CC_
#  include "FileInfo.n"
#  undef FILEINFO_CC_
#endif
