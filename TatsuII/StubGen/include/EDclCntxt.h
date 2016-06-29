/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EDclCntxt.h,v 1.1 1997/06/09 19:52:13 nmcl Exp $
 */

#ifndef EDCLCNTXT_H_
#define EDCLCNTXT_H_

/*
 *
 * This class implements a flat scoped symbol table suitable for the 
 * tables needed in exception declaration lists. Most functionality is
 * inherited from DeclContext.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

class ExceptionDeclContext : public DeclContext
{
public:
    /* Constructors and destructors */

    ExceptionDeclContext (DeclContext * = 0, int = 17);
    ~ExceptionDeclContext ();

    /* virtual interface*/

    virtual Boolean isApplicable (StubModifier) const;

private:
};

#include "EDclCntxt.n"

#endif
