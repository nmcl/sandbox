#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

struct mc_msghdr
{
	    struct iovec mc_address;
	        struct iovec mc_msg[MSG_MAXIOVLEN-1];
	            int msg_len;
};

struct mc_sg_filter
{
	    struct mc_sg_filter *next_fs;
	        struct mc_msghdr *mc_msg;
};

