/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileInfo.h,v 1.5 1993/09/20 14:09:16 ngdp Exp $
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

    void addStubModifier (StubModifier);

    String getDname () const;
    String getFname () const;
    StubModifier getGeneric () const;
    Boolean warningsEnabled () const;

private:
    String fileDname;
    String fileFname;
    StubModifier fileGeneric;
    Boolean warningsOn;
};

#include "FileInfo.n"

#endif
