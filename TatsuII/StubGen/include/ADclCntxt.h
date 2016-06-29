/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ADclCntxt.h,v 1.1 1997/06/09 19:52:07 nmcl Exp $
 */

#ifndef ADCLCNTXT_H_
#define ADCLCNTXT_H_

/*
 *
 * This class implements a flat scoped symbol table suitable for the 
 * tables needed in argument declaration lists. Most functionality is
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

class FctType;

class ArgDeclContext : public DeclContext
{
public:
    /* Constructors and destructors */

    ArgDeclContext (FctType *, DeclContext * = 0, int = 17);
    ~ArgDeclContext ();

    /* virtual interface*/

    virtual String getSimpleContextName (DeclStyle) const;
    virtual Boolean isApplicable (StubModifier) const;

private:

    FctType *funcArgsAreFor;
};

#include "ADclCntxt.n"

#endif
