/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: longmltcst.h,v 1.4 1995/09/30 10:18:04 nmcl Exp $
 */

#ifndef LONGMLTCST_H_
#define LONGMLTCST_H_

/****************************************************************************/
/* Header file for new long_send and long_receive primitives. This contains */
/* various definitions used throughout the program.                         */
/****************************************************************************/

#include <RPC/Rajdoot/multicast.h>
#include <Common/Boolean.h>

#define MAX_SIZE ((Uint32) (1448-sizeof(struct info)))
#define CARRYON 1		/* Carry on receiving */
#define ENOTCOMPLETE -1		/* Message not complete */
#define ETIMEOUT -2		/* Timeout occurred before (entire) message received */
#define EPRINTSIZE 4		/* Return code from user's filter to get size of message as return value from LRECVSG */
#define ENOMESSAGE -3
#define ELOSTPACKETS -4
#define ELOSTMESSAGE -5
#define MAX_FIELDS 15
#define MAX_MESSAGE 4           /*Max number of packets sent in one burst*/
#define MAGIC 20

/* This structure contains the sizes of the (possibly) unknown fields at the receiver's side.*/
struct sizes {
    Uint32 field1;
    Uint32 field2;
    Uint32 field3;
};

struct info {
	Uint32 total;	        /*Total number of packets making up message*/
	Uint32 pack_numb;   	/*Number of this packet*/
	struct MID source;	        /*Sender's id*/
	Uint32 size;		/*Size of complete message in bytes*/
	struct sizes unknown;           /*Sizes of unknown fields*/
	Uint32 number_fields;    /*Number of fields to be sent*/
	Int32 sec;
	Int32 msec;
};

/* This data structure contains the information on the various messages that */
/* are being assembled. They form a linked list of character buffers.        */

struct msg_list
{
    struct MID source;                              /*Source of message*/
    Uint32 seqnce;
    Int32 sec, msec;
};

struct MID;
struct OBJECT_DESC;

extern int multicast_long_send (struct MID *, struct mc_msghdr *);
extern int multicast_long_receive (struct MID *, struct mc_sg_filter *, 
			    int (*filter) (struct mc_sg_filter *,Boolean),
			    Int32);
extern int multicast_send_message (struct OBJECT_DESC *, struct mc_msghdr *); 
 
#endif

