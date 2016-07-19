/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclListI.h,v 1.4 1993/03/22 09:27:02 ngdp Exp $
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
    DeclarationList *current_list;	/* list we are curently walking */
					/* down */

};

#endif
