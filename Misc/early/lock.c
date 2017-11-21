#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <stdio.h>
#include <errno.h>

main()
{
	int filedesc;

	filedesc = open("/tmp/story", O_RDWR, 0666);

	if (filedesc < 0)
	{
		printf("error opening file\n");
		exit(0);
	}

	if (lockf(filedesc, F_LOCK, 0) < 0)
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
