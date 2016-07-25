/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ADclCntxt.h,v 1.4 1993/06/17 15:06:51 ngdp Exp $
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

class ArgDeclContext : public DeclContext
{
public:
    /* Constructors and destructors */

    ArgDeclContext (DeclContext * = 0, int = 17);
    ~ArgDeclContext ();

    /* virtual interface*/

    virtual Boolean isApplicable (StubModifier) const;

private:
};

#include "ADclCntxt.n"

#endif
