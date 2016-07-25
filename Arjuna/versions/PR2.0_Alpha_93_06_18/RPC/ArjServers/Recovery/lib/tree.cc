/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: tree.cc,v 1.3 1993/03/18 15:43:33 n048z Exp $
 *
 */

#ifndef STDIO_H_
#include <System/stdio.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif


#include "defs.h"
#include "strlist.h"
#include "tree.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif


/***
 * Initialize the empty tree node.
 */

tree::tree(char * k)
{
	key = strdup(k);
	leftson_p = (tree *)null;
	rightson_p = (tree *)null;
	contents_p = (str_list *)null;
	nodecount = 1;
}



tree::~tree()
{
	delete key;
	delete contents_p;
}



tree * tree::left()
{
	return (leftson_p);
}



tree * tree::right()
{
	return (rightson_p);
}



/***
 * add_entry
 *	Merge a new string into the list associated with the
 *	given key.  (may grow the tree)
 *
 *	Every node contains the count of the total number of
 *	nodes in the (sub)tree rooted at it.
 *
 * Inputs:
 *	(k,s)	- (key,value) pair to add to tree
 *
 * Returns true if a new node had to be created.
 */

boolean tree::add_entry(char * k, char * s)
{
	tree ** ppt;
	boolean n = false;

	int cr = strcmp(k, key);
	if (cr <  0) {
		ppt = &leftson_p;
	} else if (cr == 0) {
		if (contents_p == (str_list *)null)
			contents_p = (str_list *)new str_list();
		contents_p->merge_str(s);
		return (false);
	} else if (cr > 0) {
		ppt = &rightson_p;
	}


	if (*ppt == (tree *)null) { 
		*ppt = new tree(k);
		n = true;
	}

	n = (*ppt)->add_entry(k, s) || n;
	if (n)
		nodecount++;
	return (n);
}



/***
 * Return the string list associated with a key.
 *
 * Returns null if there is no such key.
 */

str_list * tree::find_entry(char * k)
{
	int i = strcmp(k, key);

	if (i < 0) {
		if (leftson_p == (tree *)null)
			return ((str_list *)null);
		else
			return (leftson_p->find_entry(k));
	} else  if (i == 0) {
		return (contents_p);
	} else {
		if (rightson_p == (tree *)null)
			return ((str_list *)null);
		else
			return (rightson_p->find_entry(k));
	}
}



/***
 * Count the nodes in a tree.
 */

int tree::count()
{
	return (nodecount);
}



/***
 * Do a pre-order traversal, executing a caller-supplied procedure
 * at each node.
 */

void tree::traverse(void (*visit)(char *, int, str_list *, void **),
		void ** arglist)
{
	(*visit)(key, nodecount, contents_p, arglist);

	if (leftson_p != (tree *)null)
		leftson_p->traverse(visit, arglist);
	if (rightson_p != (tree *)null)
		rightson_p->traverse(visit, arglist);
}


