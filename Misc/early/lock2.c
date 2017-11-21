#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>

main()
{
	int filedesc;
	struct flock lockdesc;

	filedesc = open("/tmp/lock.c", O_RDWR, 0666);

	if (filedesc < 0)
	{
		printf("error opening file\n");
		exit(0);
	}

	lockdesc.l_type = F_WRLCK;
	lockdesc.l_whence = SEEK_SET;
	lockdesc.l_start = 0;
	lockdesc.l_len = 0;

	if (fcntl(filedesc, F_SETLK, lockdesc) < 0)
	{
		perror("Error");
		close(filedesc);
		exit(1);
	}
	else
	{
		printf("ok\n");
		close(filedesc);
	}
}
