struct header
{
	    int packet_number;
	        int buffer_len;
};

#define PACKET_SIZE (MAX_PACKET-sizeof(struct header))

