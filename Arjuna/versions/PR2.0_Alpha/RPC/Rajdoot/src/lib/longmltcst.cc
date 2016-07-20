/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: longmltcst.cc,v 1.67 1993/05/05 13:41:08 ngdp Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifdef SYSV
#ifndef SYS_TIMES_H_
#  include <System/sys/times.h>
#endif

#ifndef LIMITS_H_
#  include <System/limits.h>
#endif
#endif

#ifndef ERROR_H_
#include <Common/Error.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

#ifndef LONGMLTCST_H_
#  include "longmltcst.h"
#endif

/* New version of long_send and long_receive primitives. */

static const char RCSid[] = "$Id: longmltcst.cc,v 1.67 1993/05/05 13:41:08 ngdp Exp $";

#ifdef SYSV
double clockTick = (1.0/sysconf(_SC_CLK_TCK));
#endif

struct msg_list *marker = NULL;

/*****************************************************************************/
/* Calculate the number of packets of max. size transmissable in one go that */
/* make up actual message. Only MAX_MESSAGE will actually be transmitted in  */
/* one burst before waiting for WAITTIME milliseconds and sending the rest.  */
/* Because of this each message can be easily broken up into packets which   */
/* can be received in any order and assembled correctly to produce the       */
/* original message.							     */
/*****************************************************************************/

unsigned long numb_packets (struct mc_msghdr *message, unsigned long *msgsize)
{
	unsigned int i;
	unsigned long packet_size;
	long packets;
	unsigned long msize = 0;

	packet_size = MAX_SIZE - (sizeof(long) * message->msg_len) - sizeof(struct info);

	for (i = 0; i <= message->msg_len -1; i++)
	    msize += message->mc_msg[i].iov_len;
	*msgsize = msize;

	if (message->msg_len <= MAX_FIELDS)
	    packets = (msize/packet_size) + ((msize%packet_size) ? 1 : 0);
	else
	{
	    if (msize > packet_size)
		packets = (msize/packet_size) + ((msize%packet_size) ? 1 : 0);
	    else
	    {
		msize -= message->mc_msg[message->msg_len -1].iov_len;
		packets = (msize/packet_size) + ((msize%packet_size) ? 1 : 0) + 1;
	    }
	}

	return(packets);
}

/*****************************************************************************/
/* Long_send primitive. Takes same parameters as multicast_send but can cope */
/* with arbitrary length messages.                                           */
/* Sends MAX_MESSAGE packets before waiting for an ack from receiver.        */
/*****************************************************************************/

int multicast_long_send (struct MID *midptr, struct mc_msghdr *message)
{
    unsigned int i, psize = 0;
    int sstat, counter = 0, rstat;
    long sent = 0, msgtosend = 0;
    char *ptr = 0, *new_buff = 0;
    struct mc_msghdr temp_buf, ack_buf;
    struct mc_sg_filter msg;
    struct info header;
    struct MID me;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Starting multicast_long_send.\n" << flush;
#endif

    temp_buf.mc_msg[0].iov_base = (char*) &header;
    temp_buf.mc_msg[0].iov_len = sizeof(header);

    multicast_get_mid(&me);

    header.source = me;
    header.total = numb_packets(message, &header.size);
    if (header.total <= 0)
    {
	error_stream << WARNING << "Rajdoot:multicast_long_send : invalid message length\n";
	return EMCASTSENDFAILED;
    }

    header.number_fields = message->msg_len;
    header.unknown.field1 = message->mc_msg[message->msg_len -1].iov_len;
    if (message->msg_len > 1)
	header.unknown.field2 = message->mc_msg[message->msg_len -2].iov_len;
    else
	header.unknown.field2 = 0;
    if (message->msg_len > 2)
	header.unknown.field3 = message->mc_msg[message->msg_len -3].iov_len;
    else
	header.unknown.field3 = 0;

    msgtosend = header.total/10;
    if (msgtosend < MAX_MESSAGE)
	msgtosend = MAX_MESSAGE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "sending: header.unknown.field1 : " << header.unknown.field1 << "\n" << flush;
    debug_stream << "sending: header.unknown.field2 : " << header.unknown.field2 << "\n" << flush;
    debug_stream << "sending: header.unknown.field3 : " << header.unknown.field3 << "\n" << flush;
#endif

    for (i = 0; i < message->msg_len; i++)
	psize += message->mc_msg[i].iov_len;

    new_buff = new char[psize+1];
    ptr = new_buff;
    for (i = 0; i < message->msg_len; i++)
    {
	if (message->mc_msg[i].iov_len > 0)
	    if (message->mc_msg[i].iov_base)
	    {
		memcpy(ptr, message->mc_msg[i].iov_base, message->mc_msg[i].iov_len);
		ptr += message->mc_msg[i].iov_len;
	    }
    }
    ptr = new_buff;

    msg.mc_msg = &ack_buf;
    msg.next_fs = NULL;
    msg.mc_msg->mc_msg[0].iov_base = (char*) &header;
    msg.mc_msg->mc_msg[0].iov_len = sizeof(struct info);
    msg.mc_msg->msg_len = 1;

    do
    {
	temp_buf.mc_msg[1].iov_base = ptr;
	if (psize > MAX_SIZE)
	    temp_buf.mc_msg[1].iov_len = (int) MAX_SIZE;
	else
	    temp_buf.mc_msg[1].iov_len = psize;
	temp_buf.msg_len = 2;
	
	header.pack_numb = sent;
	sent++;
	counter++;

	sstat = multicast_send(midptr, &temp_buf);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Multicast_send returned with code: " << sstat << "\n" << flush;
#endif

	ptr += MAX_SIZE;
	psize -= temp_buf.mc_msg[1].iov_len;

/* Wait for ack from receiver before sending next set of packets. */

	if ((counter == msgtosend) && (header.total != sent))
	{
	    rstat = multicast_receive(&me, &msg, NULL, WAITTIME);
	    counter = 0;
	}
    } while ((sent != header.total) && (sstat == MC_OK));

    if (new_buff)
    {
	delete [] new_buff;
	new_buff = NULL;
    }

    return(sstat);
}

/******************************************************************************************/
/* Meta filter called by long_receive whenever a new packet is received or a timeout      */
/* occurs. It assembles the packets into one complete message.                            */
/******************************************************************************************/

int meta_filter (struct mc_sg_filter *temp_buf, Boolean timeout)
{
    struct info *header;

    header = (struct info*) temp_buf->mc_msg->mc_msg[0].iov_base;

    if (!timeout)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "meta_filter received packet\n" << flush;
	debug_stream << "expected <" << marker->seqnce << "," << marker->source.lo_part << ">\n" << flush;
	debug_stream << "got <" << header->pack_numb << "," << header->source.lo_part << ">\n" << flush;
#endif

	if ((header->pack_numb == marker->seqnce) && (header->source.lo_part == marker->source.lo_part))
	{
	    marker->seqnce++;

	    if (marker->seqnce == header->total)
		return(header->size);
	    else
		return(ENOTCOMPLETE);
	}
	else
	    if ((header->pack_numb == 0) && (marker->seqnce == 0))
	    {
		marker->seqnce = 1;
		marker->source = header->source;

		if (header->total == 1)
		    return(header->size);
		else
		    return(ENOTCOMPLETE);
	    }
	else
	    if ((header->pack_numb != marker->seqnce) && (header->source.lo_part == marker->source.lo_part))
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "multicast_long_receive::packets lost in transmission.\n" << flush;
#endif
		return(ELOSTPACKETS);
	    }
	else
	    if (header->source.lo_part != marker->source.lo_part)
		return(ENOTCOMPLETE);
	else
	    return(ENOMESSAGE);
    }
    else
	    return(ETIMEOUT);
}

/******************************************************************************************/
/* multicast_long_receive. Takes additional parameters to multicast_receive which specify */
/* timeout period and also whether or not to allocate memory for last fields.             */
/******************************************************************************************/

int multicast_long_receive (struct MID *midptr, struct mc_sg_filter *msg_ptr,
			    int (*filter) (struct mc_sg_filter *, Boolean), long time_out)
{
    struct mc_sg_filter buf;
    struct mc_msghdr temp_buf, msg;
    struct MID to;
    struct info header;
    long interval, msgtoreceive = 0;
    int t_out, flag, failed, fstat = 0, stop, counter = 0;
    unsigned int length, i;
    long rstat, sstat;
    caddr_t buffer = NULL;
    caddr_t ptr = NULL;
    caddr_t user_buffer1 = NULL;
    caddr_t user_buffer2 = NULL;
    Boolean HaveReceived = FALSE;
#ifdef SYSV
    clock_t currentTime = 0;
    struct tms Clockbuffer;
#else    
    unsigned long start_ms, finish_ms;
    struct timeval stime,ftime;
#endif

    header.unknown.field1 = header.unknown.field2 = header.unknown.field3 = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Within multicast_long_receive\n" << flush;
#endif

    buf.mc_msg = &temp_buf;
    buf.next_fs = NULL;

    if (time_out == -1)
	interval = -1;
    else
	interval = time_out * 10000;

#ifdef SYSV
    currentTime = times(&Clockbuffer);
    double convertTime = 0.0;
#else    
    gettimeofday(&stime, NULL);
    start_ms = stime.tv_sec*1000000 + stime.tv_usec;
#endif

    do
    {
	if (buffer)
	{
	    delete buffer;
	    buffer = NULL;
	}

	length = (unsigned int) MAX_SIZE;

	buffer = new char[length];
	memset(buffer, '\0', length);
	ptr = buffer;

	marker = new msg_list;
	marker->seqnce = 0;
	marker->source.lo_part = 0;
	marker->source.hi_part = 0;

	stop = FALSE;
	failed = FALSE;
	flag = 0;
	do
	{
	    buf.mc_msg->mc_msg[0].iov_base = (caddr_t) &header;
	    buf.mc_msg->mc_msg[0].iov_len  = sizeof(struct info);
	    buf.mc_msg->mc_msg[1].iov_base = ptr;
	    buf.mc_msg->mc_msg[1].iov_len  = length;
	    buf.mc_msg->msg_len = 2;

	    rstat = multicast_receive(midptr, &buf, meta_filter, interval);

#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "multicast_receive returned with: " << rstat << "\n" << flush;
#endif


/*
	    if ((rstat == ETIMEOUT) && (HaveReceived))
		rstat = ENOMESSAGE;
*/

	    if (rstat != ETIMEOUT)
	    {
		if (header.number_fields != msg_ptr->mc_msg->msg_len)
		{
#ifdef DEBUG
		    error_stream << WARNING << "Rajdoot:multicast_long_receive : received unexpected message - will ignore\n";
#endif
		    rstat = ENOMESSAGE;
		}
	    }

/* If MAX_MESSAGE packets received and more to come then send ack to sender. */

	    if (rstat == ENOTCOMPLETE)
	    {
		HaveReceived = TRUE;
		if (!msgtoreceive)
		{
		    msgtoreceive = header.total/10;
		    if (msgtoreceive < MAX_MESSAGE)
			msgtoreceive = MAX_MESSAGE;
		}
		counter++;
		if (counter == msgtoreceive)
		{
		    to = header.source;
		    msg.mc_msg[0].iov_base = (char*) &header;
		    msg.mc_msg[0].iov_len = sizeof(struct info);
		    msg.msg_len = 1;
		    sstat = multicast_send(&to, &msg);
		    counter = 0;
		}
	    }

	    if ((rstat >= ENOTCOMPLETE) && (!flag))
	    {
		HaveReceived = TRUE;
		if (header.size == 0)
		{
		    error_stream << WARNING << "Rajdoot:multicast_long_receive : invalid message length received\n";
		    rstat = ELOSTPACKETS;
		}
		else
		{
		    if (header.total > 1)
			buffer = (caddr_t) realloc((malloc_t) buffer, (unsigned int) (header.total * MAX_SIZE));
		    
		    ptr = buffer;
		    flag = 1;
		}
	    }

	    if (rstat < ENOTCOMPLETE)
		failed = TRUE;
	    else
		failed = FALSE;

	    if (rstat == ENOTCOMPLETE)
		ptr += MAX_SIZE;

	    if (time_out != -1)
	    {
#ifdef SYSV
		currentTime -= times(&Clockbuffer);
		convertTime = currentTime * clockTick * 1000000;
		interval = (time_out * 10000) - convertTime;
#else		
		gettimeofday(&ftime, NULL);
		finish_ms = ftime.tv_sec*1000000 + ftime.tv_usec;
		interval = (time_out * 10000) - (finish_ms - start_ms);
#endif
	    }

	    if ((interval < 0) && (time_out != -1))
	    {
		t_out = TRUE;
		if (rstat < 0)
		    rstat = ETIMEOUT;
	    }
	    else
		t_out = FALSE;

	} while ((rstat != ETIMEOUT) && (!t_out) && (rstat < 1) && (!failed));

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "receiving: header.unknown.field1 : " << header.unknown.field1 << "\n" << flush;
	debug_stream << "receiving: header.unknown.field2 : " << header.unknown.field2 << "\n" << flush;
	debug_stream << "receiving: header.unknown.field3 : " << header.unknown.field3 << "\n" << flush;
#endif

	if (rstat > 0)
	{
	    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -3].iov_len = (int) header.unknown.field3;

	    if (header.unknown.field1 > 0)
		user_buffer1 = new char[(unsigned int) header.unknown.field1];
	    else
		user_buffer1 = NULL;

	    if (header.unknown.field2 > 0)
		user_buffer2 = new char[(unsigned int) header.unknown.field2];
	    else
		user_buffer2 = NULL;

	    if ((user_buffer1 != NULL) || (user_buffer2 != NULL))
	    {
		ptr = buffer + header.size - header.unknown.field1 - header.unknown.field2;

		if (user_buffer2 != NULL)
		{
		    memcpy(user_buffer2, ptr, (int) header.unknown.field2);
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_base = user_buffer2;
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_len = (int) header.unknown.field2;
		}

		ptr += header.unknown.field2;

		if (user_buffer1 != NULL)
		{
		    memcpy(user_buffer1, ptr, (int) header.unknown.field1);
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_base = user_buffer1;
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_len = (int) header.unknown.field1;
		}
	    }

	    if (header.unknown.field2 == 0)
	    {
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_len = 0;
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_base = NULL;
	    }

	    if (header.unknown.field1 == 0)
	    {
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_len = 0;
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_base = NULL;
	    }

	    if (rstat - header.unknown.field1 - header.unknown.field2 != 0)
		if (header.unknown.field1 + header.unknown.field2 != 0)
		    buffer = (caddr_t) realloc((malloc_t) buffer,
					       (unsigned int)(rstat - header.unknown.field1 - header.unknown.field2));

	    ptr = buffer;
	    for (i = 0; i < msg_ptr->mc_msg->msg_len -2; i++)
	    {
		if (msg_ptr->mc_msg->mc_msg[i].iov_len == 0)
		    msg_ptr->mc_msg->mc_msg[i].iov_base = NULL;
		else
		{
		    msg_ptr->mc_msg->mc_msg[i].iov_base = ptr;
		    ptr += msg_ptr->mc_msg->mc_msg[i].iov_len;
		}
	    }

	    msg_ptr->source = marker->source;
	    
	    if ((int) filter !=0 )
	    {
		if (rstat > 0) fstat = (*filter)(msg_ptr, FALSE);
		else fstat = (*filter)(msg_ptr, TRUE);
	    }
	    else stop = TRUE;
	}

	if (rstat < ENOTCOMPLETE) stop = TRUE;

    } while((!t_out) && (fstat == CARRYON) && (!stop));

    if (marker)
    {
	delete marker;
	marker = 0;
    }

    if (((msg_ptr->mc_msg->msg_len <= 2) && (buffer)) ||
	((rstat < 0) && (buffer)))
	delete [] buffer;

    if ((int) filter != 0)
    {
	if (fstat == EPRINTSIZE) return(rstat);
	else return(fstat);
    }

    return(rstat);
}

int multicast_send_message (struct OBJECT_DESC *objects, struct mc_msghdr *message)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "int multicast_send_message (struct OBJECT_DESC *objects, struct mc_msghdr *message)" << endl;
#endif

    int status, i, t_status, failed;

    i = 0;
    failed = 0;
    status = MC_OK;
    while (objects[i].destn_host != 0)        /* Use multiple unicast */
    {
      if (objects[i].grps_id.groups_type != SRVR_dead) /* Server did not respond to initiate */
      {
          t_status = multicast_long_send(&objects[i].grps_id.grp_srvr.srvr.sr_id,
                                         message);
          if (t_status == EUCASTSENDFAILED)
              failed ++;
      }

      i++;
    }
    if (failed == i)
      status = EMCASTSENDFAILED;
    else
      if (failed == 0)
          status = MC_OK;
      else
          status = EPARTIALSEND;
    return(status);
}
