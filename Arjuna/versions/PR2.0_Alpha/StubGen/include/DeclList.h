/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclList.h,v 1.5 1993/03/22 09:27:00 ngdp Exp $
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

    void append_decl (Declaration *);
    void append_list (DeclarationList *);

    void push_decl (Declaration *);
//    void push_list (DeclarationList *);

private:
    /* private state */
    DeclarationListEntry *head;		/* pointer to head of list */
    DeclarationListEntry *tail;


};

#include "DeclList.n"

#endif
