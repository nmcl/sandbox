/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileInfo.h,v 1.3 1993/03/22 09:27:16 ngdp Exp $
 */

#ifndef FILEINFO_H_
#define FILEINFO_H_

/*
 * File information class. Keeps stub generations options on a per file 
 * basis.
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

class FileInfo
{
public:
    /* constructors and destructors */

    FileInfo (const String&, const String&);

    /* Public functions */

    void add_stub_modifier (StubModifier);

    String get_dname () const;
    String get_fname () const;
    StubModifier get_generic () const;

private:
    String file_dname;
    String file_fname;
    StubModifier file_generic;
};

#include "FileInfo.n"

#endif
