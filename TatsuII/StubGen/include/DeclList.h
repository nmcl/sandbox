/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclList.h,v 1.1 1997/06/09 19:52:11 nmcl Exp $
 */

#ifndef DECLLIST_H_
#define DECLLIST_H_

#include <iostream>

using namespace std;

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

extern  ostream& operator<< (ostream&, DeclarationList *d);

#include "DeclList.n"

#endif
