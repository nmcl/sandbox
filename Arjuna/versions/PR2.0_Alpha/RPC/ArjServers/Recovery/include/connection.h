/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: connection.h,v 1.2 1993/03/18 15:42:50 n048z Exp $
 *
 */

class str_list;

class connection {
public:
	connection(int xsd, char * callername, int callerport);
	~connection();

	char * cread(int * size);
	str_list * cread();

	int cwrite(char * bfr, int size);
	boolean cwrite(str_list *);

	char *	rname();
	int	rport();

private:
	int	sd;		/* socket descriptor of established circuit */

	char *	rem_name;	/* host name of remote party */
	int	rem_port;	/* port of remote party */

	char  * netbfr;
	int	netbfrsz;

};

