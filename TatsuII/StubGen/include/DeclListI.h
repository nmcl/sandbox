/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclListI.h,v 1.1 1997/06/09 19:52:12 nmcl Exp $
 */

#ifndef DECLLISTI_H_
#define DECLLISTI_H_

/*
 *
 * Stub generator declaration list iterator class. 
 * Iterates over lists of declarations.
 *
 */

class Declaration;
class DeclarationList;
class DeclarationListEntry;

class DeclarationListIterator
{
public:
    DeclarationListIterator (DeclarationList *);

    Declaration *operator() ();

private:
    DeclarationListEntry *next;		/* return this one next time */
					/* called */
    DeclarationList *currentList;	/* list we are curently walking */
					/* down */

};

#endif
