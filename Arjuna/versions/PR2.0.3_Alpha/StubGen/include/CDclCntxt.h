/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CDclCntxt.h,v 1.6 1993/06/17 15:06:58 ngdp Exp $
 */

#ifndef CDCLCNTXT_H_
#define CDCLCNTXT_H_

/*
 *
 * This class implements a flat scoped symbol table suitable for the 
 * tables needed in classes and structs( ie they contain pointers
 * to base class contexts as well as to outer level contexts)
 *
 */

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

class BaseClass;
class ClassType;
class Declaration;
class DeclarationList;
class String;

class ClassDeclContext : public DeclContext
{
public:
    /* Constructors and destructors */

    ClassDeclContext (ClassType *, DeclContext * = 0, BaseClass * = 0, int = 211);
    ~ClassDeclContext ();

    /* virtual interface*/

    virtual Boolean isApplicable (StubModifier) const;
    virtual Declaration *lookFor (const String&, Type, Context) const;
    virtual Boolean putIn (Declaration *);

private:
    /* private state */

    BaseClass *inheritedContext;	/* Context of base classes*/
    ClassType *owner;			/* my owner class */

};

#endif
