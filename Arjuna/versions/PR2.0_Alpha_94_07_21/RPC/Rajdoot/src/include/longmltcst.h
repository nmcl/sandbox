/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: longmltcst.h,v 1.20 1993/09/08 09:28:53 nmcl Exp $
 */

#ifndef LONGMLTCST_H_
#define LONGMLTCST_H_

/****************************************************************************/
/* Header file for new long_send and long_receive primitives. This contains */
/* various definitions used throughout the program.                         */
/****************************************************************************/

#include <RPC/Rajdoot/multicast.h>
#include <Common/Boolean.h>

#define MAX_SIZE ((unsigned long) (1448-sizeof(struct info)))
#define CARRYON 1		/* carry on receiving */
#define ENOTCOMPLETE -1		/*Message not complete*/
#define ETIMEOUT -2		/*Timeout occurred before entire message received*/
#define EPRINTSIZE 4		/*Return code from user's filter to get size of message as return value from LRECVSG*/
#define ENOMESSAGE -3
#define ELOSTPACKETS -4
#define ELOSTMESSAGE -5
#define MAX_FIELDS 15
#define MAX_MESSAGE 4           /*Max number of packets sent in one burst*/
#define MAGIC 20

/* This structure contains the sizes of the (possibly) unknown fields at the receiver's side.*/
struct sizes {
    unsigned long field1;
    unsigned long field2;
    unsigned long field3;
};

struct info {
	unsigned long total;	        /*Total number of packets making up message*/
	unsigned long pack_numb;   	/*Number of this packet*/
	struct MID source;	        /*Sender's id*/
	unsigned long size;		/*Size of complete message in bytes*/
	struct sizes unknown;           /*Sizes of unknown fields*/
	unsigned long number_fields;    /*Number of fields to be sent*/
	long sec;
	long msec;
};

/* This data structure contains the information on the various messages that */
/* are being assembled. They form a linked list of character buffers.        */

struct msg_list
{
    struct MID source;                              /*Source of message*/
    unsigned long seqnce;
    long sec, msec;
};

extern int multicast_long_send (struct MID *, struct mc_msghdr *);
extern int multicast_long_receive (struct MID *, struct mc_sg_filter *, 
			    int (*filter) (struct mc_sg_filter *,Boolean),
			    long);
extern int multicast_send_message (struct OBJECT_DESC *, struct mc_msghdr *); 
 
#endif

