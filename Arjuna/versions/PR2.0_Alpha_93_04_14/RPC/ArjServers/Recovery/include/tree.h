/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: tree.h,v 1.2 1993/03/18 15:43:01 n048z Exp $
 *
 */

class tree {
public:
	tree(char * key);
	~tree();

	tree *		left();			/* return the left son */
	tree *		right();		/* return the right son */

	boolean		add_entry(char * key, char * value);
	str_list *	find_entry(char * key);
	int		count();		/* count nodes */

	void		traverse(void (*visit)(char *,int,str_list *,void **),
				void ** args);

protected:
	tree *		leftson_p;
	tree *		rightson_p;
	char *		key;
	str_list *	contents_p;
	int		nodecount;
};

