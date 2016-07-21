/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: strlistiter.h,v 1.3 1993/03/18 15:42:59 n048z Exp $
 *
 */

#ifdef index
#  undef index
#endif

class str_list_iterator {
public:
	str_list_iterator(str_list *, int start_at = 0);
	~str_list_iterator();

	char * str_next();

private:
	str_list *	psl;
	int		index;

};

