/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: strlist.h,v 1.2 1993/03/18 15:42:57 n048z Exp $
 *
 */

const	Size_increment = 16;


class str_list {

friend class str_list_iterator;

public:
	str_list(int max_size = Size_increment);
	~str_list();

	void	merge_str(char *);
	void	add_str(char *);
	char *	get_str(int);
	struct	iovec * get_iovec(int * totalsize, int * count);

	int	count();
	void	print();

private:
	char **	edge;		/* pointer to vector of strings (like argv) */
	int	edge_max;	/* size of edge (max. # pointers) */
	int	edge_count;	/* current entry count */
};

