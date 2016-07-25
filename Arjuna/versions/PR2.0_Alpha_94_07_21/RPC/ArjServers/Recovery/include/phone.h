/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: phone.h,v 1.2 1993/03/18 15:42:55 n048z Exp $
 *
 */

class connection;

class phone {
public:
	phone(int port = -1, int backlog = -1);
	~phone();

	connection *	answer();
	connection *	call(char * nodename, int port);

private:
	int	sd;		/* socket descriptor */
	int	portno;		/* port we've bound to it */
	int	backlog;	/* caller backlog set by listen() */
};

