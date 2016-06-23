/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TmplCntxt.h,v 1.1 1997/09/25 15:30:29 nmcl Exp $
 */

#ifndef TMPLCNTXT_H_
#define TMPLCNTXT_H_

/*
 *
 * This class implements a flat scoped symbol table suitable for the 
 * tables needed in templates for classes and structs( ie they contain pointers
 * to other class contexts as well as to outer level contexts)
 *
 */

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

class Declaration;
class DeclarationList;
class String;

class TemplateContext : public DeclContext
{
public:
    /* Constructors and destructors */

    TemplateContext (DeclContext *, int = 17);
    ~TemplateContext ();

    /* Specifics to this class */

    Declaration *getTemplateEntry (int) const;
    int getEntryCount () const;

    void setContext (DeclContext *);

    /* inherited virtual interface*/

    virtual void freeze ();
    virtual Boolean isApplicable (StubModifier) const;
    virtual Declaration *lookFor (const String&, Type, Context) const;
    virtual Boolean putIn (Declaration *);
    virtual ostream& printOn (ostream&, DeclStyle = NORMAL_DECL, Boolean = TRUE,
			      Boolean = TRUE,  Boolean = FALSE, const char* = ", ") const;
    

private:
    /* private state */

    Boolean argsFrozen;
    DeclContext *mainContext;
};

#endif
