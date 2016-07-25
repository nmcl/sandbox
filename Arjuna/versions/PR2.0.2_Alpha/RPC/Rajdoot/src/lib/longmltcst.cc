/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: longmltcst.cc,v 1.85 1993/10/22 13:23:27 nmcl Exp $
 */

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

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

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
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
#  include <Common/Error.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
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


static const char RCSid[] = "$Id: longmltcst.cc,v 1.85 1993/10/22 13:23:27 nmcl Exp $";


#ifdef SYSV
double clockTick = (1.0/sysconf(_SC_CLK_TCK));
#endif

struct msg_list *marker = 0;


extern SERVERSWORK latest_work;
extern int latest_indx;

unsigned long burstSize = 0;
unsigned long burstTime = 0;


void setBurstSize (unsigned long toSet)
{
    burstSize = toSet;
}

void getBurstSize (unsigned long& toSet)
{
    toSet = burstSize;
}

void setBurstTime (unsigned long toSet)
{
    burstTime = toSet;
}

void getBurstTime (unsigned long& toSet)
{
    toSet = burstTime;
}


/*
 * Meta filter called by long_receive whenever a new packet is received or a
 * timeout occurs. It assembles the packets into one complete message.
 */

int meta_filter (struct mc_sg_filter *temp_buf, Boolean timeout)
{
    struct info *header;

    header = (struct info*) temp_buf->mc_msg->mc_msg[0].iov_base;

    if (!timeout)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "meta_filter received packet\n" << flush;
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "expected <" << marker->seqnce << "," << ntohl(marker->source.lo_part) << ">\n" << flush;
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "got <" << ntohl(header->pack_numb) << "," << ntohl(header->source.lo_part) << ">\n" << flush;
#endif

	if ((ntohl(header->pack_numb) == 0) && (marker->seqnce == 0))
	{
	    marker->source.type = header->source.type;
	    marker->source.lo_part = header->source.lo_part;
	    marker->source.hi_part = header->source.hi_part;

	    if (ntohl(header->total) > 1)
	    {
		// check for retry at this level
	
		for (int i = 0; i <= latest_indx; i++)
		{
		    if ((latest_work[i].client_address.lo_part == header->source.lo_part)
			&& (latest_work[i].seq_no.sec == ntohl(header->sec))
			&& (latest_work[i].seq_no.msec == ntohl(header->msec)))
			return(ntohl(header->size));
		}
	    }
	
	    marker->seqnce = 1;
	    marker->source.type = header->source.type;
	    marker->source.lo_part = header->source.lo_part;
	    marker->source.hi_part = header->source.hi_part;
	    marker->sec = ntohl(header->sec);
	    marker->msec = ntohl(header->msec);
	    if (ntohl(header->total) == 1)
		return(ntohl(header->size));
	    else
		return(ENOTCOMPLETE);
	}
	else
	if ((ntohl(header->pack_numb) == marker->seqnce) && (header->source.lo_part == marker->source.lo_part)
	    && (ntohl(header->sec) == marker->sec) && (ntohl(header->msec) == marker->msec))
	{
	    marker->seqnce++;
	    if (marker->seqnce == ntohl(header->total))
		return(ntohl(header->size));
	    else
		return(ENOTCOMPLETE);
	}
	else
	    if ((ntohl(header->pack_numb) < marker->seqnce) && (header->source.lo_part == marker->source.lo_part)
		&& (ntohl(header->sec) == marker->sec) && (ntohl(header->msec) == marker->msec))
		return(ELOSTPACKETS);
	else
	    if ((ntohl(header->pack_numb) > marker->seqnce) && (header->source.lo_part == marker->source.lo_part)
		&& (ntohl(header->sec) == marker->sec) && (ntohl(header->msec) == marker->msec))
		return(ELOSTPACKETS);
	else
	    if ((header->source.lo_part == marker->source.lo_part) && (ntohl(header->sec) != marker->sec)
		&& (ntohl(header->msec) != marker->msec))
		return(ELOSTMESSAGE);
	else
	    return(ENOMESSAGE);
    }
    else
	    return(ETIMEOUT);
}

/*****************************************************************************/
/* Calculate the number of packets of max. size transmissable in one go that */
/* make up actual message. Only MAX_MESSAGE will actually be transmitted in  */
/* one burst before waiting for ROUNDTRIP_TIME milliseconds and sending rest.*/
/* Because of this each message can be easily broken up into packets which   */
/* can be received in any order and assembled correctly to produce the       */
/* original message.							     */
/*****************************************************************************/

unsigned long numb_packets (struct mc_msghdr *message, unsigned long *msgsize)
{
    unsigned long packet_size;
    long packets;
    unsigned long msize = 0;

    packet_size = MAX_SIZE - (sizeof(long) * message->msg_len) - sizeof(struct info);
    
    for (int i = 0; i <= message->msg_len -1; i++)
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

    *msgsize = htonl(*msgsize);

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
    int sstat, counter = 0;
    long sent = 0, msgtosend = 0;
    char *ptr = 0, *new_buff = 0;
    struct mc_msghdr temp_buf;
    struct info header;
    struct MID me;
    struct RPC_SN* sNo = (struct RPC_SN*)message->mc_msg[0].iov_base;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "int multicast_long_send ()" << endl;
#endif

    temp_buf.mc_msg[0].iov_base = (char*) &header;
    temp_buf.mc_msg[0].iov_len = sizeof(header);

    multicast_get_mid(&me);

    header.source.type = me.type;
    header.source.hi_part = me.hi_part;
    header.source.lo_part = me.lo_part;
    header.total = htonl(numb_packets(message, &header.size));
    header.sec = htonl(sNo->sec);
    header.msec = htonl(sNo->msec);

    if (ntohl(header.total) <= 0)
    {
	error_stream << WARNING << "Rajdoot:multicast_long_send : invalid message length\n";
	return EMCASTSENDFAILED;
    }

    header.number_fields = htonl(message->msg_len);
    header.unknown.field1 = htonl(message->mc_msg[message->msg_len -1].iov_len);
    if (message->msg_len > 1)
	header.unknown.field2 = htonl(message->mc_msg[message->msg_len -2].iov_len);
    else
	header.unknown.field2 = htonl(0);
    if (message->msg_len > 2)
	header.unknown.field3 = htonl(message->mc_msg[message->msg_len -3].iov_len);
    else
	header.unknown.field3 = htonl(0);

    if (burstSize == 0)
    {
	msgtosend = ntohl(header.total)/MAGIC +1;
	if (msgtosend < MAX_MESSAGE/2)
	    msgtosend = MAX_MESSAGE/2;
	if (msgtosend > MAX_MESSAGE)
	    msgtosend = MAX_MESSAGE;
    }
    else
	msgtosend = burstSize;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "sending: header.unknown.field1 : " << ntohl(header.unknown.field1) << "\n" << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "sending: header.unknown.field2 : " << ntohl(header.unknown.field2) << "\n" << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "sending: header.unknown.field3 : " << ntohl(header.unknown.field3) << "\n" << flush;
#endif

    for (i = 0; i < message->msg_len; i++)
	psize += message->mc_msg[i].iov_len;

    new_buff = ::new char[psize+1];
    ::memset(new_buff, '\0', psize+1);
    ptr = new_buff;
    for (i = 0; i < message->msg_len; i++)
    {
	if (message->mc_msg[i].iov_len > 0)
	{
	    if (message->mc_msg[i].iov_base)
	    {
		::memcpy(ptr, message->mc_msg[i].iov_base, message->mc_msg[i].iov_len);
		ptr += message->mc_msg[i].iov_len;
	    }
	}
    }
    ptr = new_buff;

    do
    {
	temp_buf.mc_msg[1].iov_base = ptr;
	if (psize > MAX_SIZE)
	    temp_buf.mc_msg[1].iov_len = (int) MAX_SIZE;
	else
	    temp_buf.mc_msg[1].iov_len = psize;
	temp_buf.msg_len = 2;
	
	header.pack_numb = htonl(sent);
	sent++;
	counter++;

	sstat = multicast_send(midptr, &temp_buf);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Multicast_send returned with code: " << sstat << "\n" << flush;
#endif

	if ((counter >= msgtosend) && (header.total != htonl(sent)) && (sstat == MC_OK))
	{
	    counter = 0;
	    if (burstTime == 0)
		usleep(ROUNDTRIP_TIME*1000);
	    else
		usleep((unsigned int) burstTime);
	}
	
	ptr += MAX_SIZE;
	psize -= temp_buf.mc_msg[1].iov_len;

    } while ((sent != ntohl(header.total)) && (sstat == MC_OK));

    if (new_buff)
    {
#ifndef __GNUG__
	::delete [] new_buff;
#else
	::delete new_buff;
#endif
	new_buff = 0;
    }

    return(sstat);
}

/*****************************************************************************/
/* multicast_long_receive. Takes additional parameters to multicast_receive  */
/* which specify timeout period and also whether or not to allocate memory   */
/* for last fields.                                                          */
/*****************************************************************************/

int multicast_long_receive (struct MID *midptr, struct mc_sg_filter *msg_ptr,
			    int (*filter) (struct mc_sg_filter *, Boolean), long time_out)
{
    struct mc_sg_filter buf;
    struct mc_msghdr temp_buf;
    struct info header;
    long interval;
    int flag, fstat = 0, stop;
    unsigned int length, i;
    long rstat;
    caddr_t buffer = 0;
    caddr_t ptr = 0;
    caddr_t user_buffer1 = 0;
    caddr_t user_buffer2 = 0;
#ifdef SYSV
    clock_t currentTime = 0, startTime = 0;
    struct tms Clockbuffer;
#else    
    unsigned long start_ms, finish_ms;
    struct timeval stime,ftime;
#endif

    header.unknown.field1 = header.unknown.field2 = header.unknown.field3 = 0;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "int multicast_long_receive ()" << endl;
#endif

    buf.mc_msg = &temp_buf;
    buf.next_fs = 0;

    if (time_out == -1)
	interval = -1;
    else
	interval = time_out * 10000;

#ifdef SYSV
    startTime = times(&Clockbuffer);
    double convertTime = 0.0;
#else    
    gettimeofday(&stime, 0);
    start_ms = stime.tv_sec*1000000 + stime.tv_usec;
#endif

    marker = new msg_list;

    do
    {
	if (buffer)
	{
	    free(buffer);
	    buffer = 0;
	}

	marker->seqnce = 0;
	marker->source.type = htonl(0);
	marker->source.lo_part = htonl(0);
	marker->source.hi_part = htonl(0);
	marker->sec = 0;
	marker->msec = 0;

	length = (unsigned int) MAX_SIZE;

	buffer = (caddr_t) ::malloc(length);
	ptr = buffer;

	stop = FALSE;
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

	    if ((rstat != ETIMEOUT) && (rstat != ELOSTPACKETS))
	    {
		// since this can happen anywhere, need to exit to resync system.

		if (ntohl(header.number_fields) != msg_ptr->mc_msg->msg_len)
		    rstat = ELOSTMESSAGE;
	    }

	    if ((rstat >= ENOTCOMPLETE) && (!flag))
	    {
		if (ntohl(header.size) == 0)
		{
		    error_stream << WARNING << "Rajdoot:multicast_long_receive : invalid message length received\n";
		    rstat = ELOSTPACKETS;
		}
		else
		{
		    if (ntohl(header.total) > 1)
			buffer = (caddr_t) realloc((malloc_t) buffer, (unsigned int) (ntohl(header.total) * MAX_SIZE));
		    
		    ptr = buffer;
		    flag = 1;
		}
	    }

	    if (rstat == ENOTCOMPLETE)
		ptr += MAX_SIZE;

	    if ((time_out != -1) && (rstat != ETIMEOUT))
	    {
#ifdef SYSV
		currentTime = times(&Clockbuffer) - startTime;
		convertTime = currentTime * clockTick * 1000000;
		interval = (time_out * 10000) - convertTime;
#else		
		gettimeofday(&ftime, 0);
		finish_ms = ftime.tv_sec * 1000000 + ftime.tv_usec;
		interval = (time_out * 10000) - (finish_ms - start_ms);
#endif
	    }

	    if ((interval < 0) && (time_out != -1))
		rstat = ETIMEOUT;

	} while ((rstat != ETIMEOUT) && (rstat < 1) && (rstat != ELOSTMESSAGE));

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "receiving: header.unknown.field1 : " << ntohl(header.unknown.field1) << "\n" << flush;
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "receiving: header.unknown.field2 : " << ntohl(header.unknown.field2) << "\n" << flush;
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "receiving: header.unknown.field3 : " << ntohl(header.unknown.field3) << "\n" << flush;
#endif

	if (rstat > 0)
	{
	    long sizeToUse = ntohl(header.unknown.field1) + ntohl(header.unknown.field2);

	    if (ntohl(header.unknown.field3) >= 0)
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -3].iov_len = (int) ntohl(header.unknown.field3);

	    if (ntohl(header.unknown.field1) > 0)
		user_buffer1 = ::new char[(unsigned int) ntohl(header.unknown.field1)];
	    else
		user_buffer1 = 0;

	    if (ntohl(header.unknown.field2) > 0)
		user_buffer2 = ::new char[(unsigned int) ntohl(header.unknown.field2)];
	    else
		user_buffer2 = 0;

	    if ((user_buffer1 != 0) || (user_buffer2 != 0))
	    {
		ptr = buffer + ntohl(header.size) - sizeToUse;

		if (user_buffer2 != 0)
		{
		    ::memcpy(user_buffer2, ptr, (int) ntohl(header.unknown.field2));
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_base = user_buffer2;
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_len = (int) ntohl(header.unknown.field2);
		}

		ptr += ntohl(header.unknown.field2);

		if (user_buffer1 != 0)
		{
		    ::memcpy(user_buffer1, ptr, (int) ntohl(header.unknown.field1));
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_base = user_buffer1;
		    msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_len = (int) ntohl(header.unknown.field1);
		}
	    }

	    if (ntohl(header.unknown.field2) <= 0)
	    {
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_len = 0;
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -2].iov_base = 0;
	    }

	    if (ntohl(header.unknown.field1) <= 0)
	    {
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_len = 0;
		msg_ptr->mc_msg->mc_msg[msg_ptr->mc_msg->msg_len -1].iov_base = 0;
	    }

	    if (rstat - sizeToUse != 0)
	    {
		if (sizeToUse > 0)
		    buffer = (caddr_t) realloc((malloc_t) buffer, (unsigned int)(rstat - sizeToUse));
#ifdef DEBUG
		else
		{
		    if (sizeToUse < 0)
		    {
			error_stream << WARNING << "Illegal size received for unknown fields " << sizeToUse << endl;
			error_stream << WARNING << ntohl(header.unknown.field1) << " " << ntohl(header.unknown.field2) << endl;
		    }
							 
		}
#endif
	    }

	    ptr = buffer;
	    for (i = 0; i < msg_ptr->mc_msg->msg_len -2; i++)
	    {
		if (msg_ptr->mc_msg->mc_msg[i].iov_len == 0)
		    msg_ptr->mc_msg->mc_msg[i].iov_base = 0;
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

    } while((rstat != ETIMEOUT) && (fstat == CARRYON) && (!stop));

    if (marker)
    {
	delete marker;
	marker = 0;
    }

    if (((msg_ptr->mc_msg->msg_len <= 2) && (buffer)) || ((rstat < 0) && (buffer)))
        free(buffer);

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
    debug_stream << "int multicast_send_message ()" << endl;
#endif

    int status, i, t_status, failed;
    unsigned short sendRetry;

    i = 0;
    failed = 0;
    status = MC_OK;
    while (objects[i].destn_host != 0)        /* Use multiple unicast */
    {
      if (objects[i].grps_id.groups_type != SRVR_dead) /* Server did not respond to initiate */
      {
	  sendRetry = 5;
	  do
	  {
	    t_status = multicast_long_send(&objects[i].grps_id.grp_srvr.srvr.sr_id,
					   message);
	  } while ((t_status == ELOSTMESSAGE) && (sendRetry-- > 0));

          if (t_status != MC_OK)
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
