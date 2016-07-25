/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclList.h,v 1.6 1993/06/17 15:07:21 ngdp Exp $
 */

#ifndef DECLLIST_H_
#define DECLLIST_H_

/*
 *
 * Stub generator declaration list class definition.
 *
 */

#ifndef DECLLISTE_H_
#  include "DeclListE.h"
#endif

class Declaration;

class DeclarationList
{
    friend class DeclarationListIterator;

public:
    /* Constructors */
    DeclarationList (Declaration *);
    ~DeclarationList ();

    /* public operations */

    void appendDecl (Declaration *);
    void appendList (DeclarationList *);

    void pushDecl (Declaration *);

private:
    /* private state */
    DeclarationListEntry *head;		/* pointer to head of list */
    DeclarationListEntry *tail;


};

#include "DeclList.n"

#endif
