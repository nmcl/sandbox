/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileInfo.cc,v 1.3 1993/03/22 09:31:04 ngdp Exp $
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

static const char RCSid[] = "$Id: FileInfo.cc,v 1.3 1993/03/22 09:31:04 ngdp Exp $";


FileInfo::FileInfo ( const String& dn, const String& fn )
                   : file_dname(dn),
		     file_fname(fn),
		     file_generic(STUB_DEFAULT)
{
}

void FileInfo::add_stub_modifier ( StubModifier new_sm )
{
#ifdef DEBUG
    debug_stream << "FileInfo::add_stub_modifier("  << (void *)this << "," 
	         << new_sm << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (new_sm)
    {
    case STUB_DEFAULT:
    case STUB_NORENAME:
	if ((file_generic == STUB_DEFAULT)  || (file_generic == new_sm))
	{
	    file_generic = new_sm;
	    break;
	}
    default:
	error_stream << new_sm << " not applicable in #pragma\n";
	warning(error_stream);
    }
}

#ifdef NO_INLINES
#  define FILEINFO_CC_
#  include "FileInfo.n"
#  undef FILEINFO_CC_
#endif
