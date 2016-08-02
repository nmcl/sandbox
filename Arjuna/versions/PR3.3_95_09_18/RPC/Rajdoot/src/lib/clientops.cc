/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: clientops.cc,v 1.5 1995/07/03 14:29:41 ngdp Exp $
 */

#ifndef STDDEF_H_
#   include <System/stddef.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#   include <Common/Debug.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDLIB_H_
#   include <System/stdlib.h>
#endif

#include <RPC/Rajdoot/rpcuser.h>
#include "rpc.h"

int findserver_insg (struct GROUP_INFO *, struct GROUP_DESC *,
		     struct MID, struct MID *);
int insertsrvr_givengrp (struct GROUP_DESC *, struct MID);
int remove_super_group (struct GROUP_INFO *, struct GROUP_DESC *, Boolean);

static const char RCSid[] = "$Id: clientops.cc,v 1.5 1995/07/03 14:29:41 ngdp Exp $";

/*****************************************************************************/
/* Routines that manipulate linked list group structure maintained by client */
/*****************************************************************************/

Boolean empty_grp (struct GROUP_DESC *mygrp)
{
    return (((mygrp -> grps . head_gids == NULL) &&
	     (mygrp -> sids . head_sids == NULL)) ? TRUE : FALSE);
} /* end empty_grp() */

void initialise_supergroup(struct GROUP_INFO *grp)
{
    /* Initialise the entire top-level super group structure */
  
  grp -> head_gd = NULL;
  grp -> no_supergrps = 0;

} /* end initialise_supergroup() */

int insert_super_group (struct GROUP_INFO *grp, struct MID givenmid)
{ /* Link a super group to the existing super group structure, if one exists;
   * otherwise create a new super group structure.
   */
  
  /* Insert group at head, if empty list, otherwise at 2nd position.
   * The only order imposed is that the first group is the parent group
   * of all the following ones - which are linked in no particular order
   */

  struct GROUP_DESC *temp_grp, *insert_slot;

  if (grp -> head_gd == NULL)
  {
      grp -> head_gd = new GROUP_DESC;
      insert_slot = grp -> head_gd;
      grp -> head_gd -> next_group = NULL;
  }
  else
  { /* insert group at 2nd position and shift list along.
     * 1st position is reserved for parent -
     * No need to traverse till end of list, since the position of group
     * in list is arbitrary.
     */
     
     if (grp -> head_gd -> next_group == NULL)
     { /* 2nd position is free */
       insert_slot = grp -> head_gd -> next_group = new GROUP_DESC;
       insert_slot -> next_group = NULL;
     }
     else
     { /*  Insert at 2nd position - shift list along */
       temp_grp = grp -> head_gd -> next_group;
       insert_slot = new GROUP_DESC;
       grp -> head_gd -> next_group = insert_slot;
       insert_slot -> next_group = temp_grp;
       /* temp_grp = NULL;  */
     }
  }

  insert_slot -> mymid = givenmid;
  insert_slot -> sids . head_sids = NULL;
  insert_slot -> grps . head_gids = NULL;  
  insert_slot -> sids . no_srs = insert_slot -> grps . no_grps = 0;

  grp -> no_supergrps++;
  return(SUCCEEDED); /* No range checks - therefore no exception returns */
          
}/* end insert_super_group() */

int insert_head_supergrp (struct GROUP_INFO *grp, struct MID givenmid)
{ /* Insert a super group at the head of the list of super groups  -
   * this being the only super group, if the list was empty at start
   */

  struct GROUP_DESC *temp_grp, *insert_slot;

  if (grp -> head_gd == NULL)
  {
      insert_slot = grp -> head_gd = new GROUP_DESC;
      grp -> head_gd -> next_group = NULL;
  }
  else
  { /* insert group at head of list and shift list along.
     * 1st position is reserved for parent.
     */

    temp_grp = grp -> head_gd;
    insert_slot = new GROUP_DESC;
    grp -> head_gd = insert_slot;
    insert_slot -> next_group = temp_grp;
  }
    
  insert_slot -> mymid = givenmid;
  insert_slot -> sids . head_sids = NULL;
  insert_slot -> grps . head_gids = NULL;
  insert_slot -> sids . no_srs = insert_slot -> grps . no_grps = 0;

  grp -> no_supergrps++;
  return(SUCCEEDED); /* No range checks - therefore no exception returns */
          
}/* end insert_head_supergrp() */

int find_group (struct GROUP_INFO *supergrp, struct GROUP_DESC *mygrp, struct MID grpid)
{
  /* Find 'supergrp' with mid = 'grpid' and return it in 'mygrp' if found */
  
  struct GROUP_DESC *current_supergrp;
  Boolean found = FALSE;

  current_supergrp = supergrp -> head_gd;

  do
  {
    if (identical_group(&grpid, &(current_supergrp -> mymid)))
      found = TRUE;
    else
      current_supergrp = current_supergrp -> next_group;
  } while ((!found) && (current_supergrp != NULL));

  if (!found)
    return(FAILED);
  else
  {
    mygrp = current_supergrp;
    return(SUCCEEDED);
  }
    
}/* end find_group() */

int insert_groups (struct GROUP_INFO *supergrp, struct MID mysupergrpid, struct MID grp_to_insert)
{
  /* Insert 'grp_to_insert' in 'supergrp' whose mid is 'mysupergrpid'.
   * This routine adds a group to an existing super group:
   * groups are arranged in no particular order, insert at head of list.
   */

  struct GROUP_DESC *mygrp;
  struct GROUPIDS *temp_slot;
  
  mygrp = supergrp->head_gd;

  if (mygrp == NULL)
    return(EMPTY_SUPERGRP);

  if (find_group(supergrp, mygrp, mysupergrpid) != SUCCEEDED)
    return(GRP_NOT_FOUND);
  else
  {
    if (mygrp -> grps . head_gids == NULL)
    {
      /* Insert at head - empty list */
      mygrp -> grps . head_gids = new GROUPIDS;
      (mygrp -> grps) . head_gids -> next_groupid = NULL;
    }
    else
    {
      /* Insert at head - shift list along */
      temp_slot = (mygrp -> grps) . head_gids;
      mygrp -> grps . head_gids = new GROUPIDS;
      ((mygrp -> grps) . head_gids) -> next_groupid = temp_slot;
    }
    mygrp -> grps . no_grps++;
    /* No checks on fidlds of groups */  
    mygrp -> grps . head_gids -> g_id = grp_to_insert; 
    
    return(SUCCEEDED);
  } /* end else */
  
}/* end insert_groups() */

int insertgrp_givengrp (struct GROUP_DESC *mygrp, struct MID groups)
{ /* Add a 'group' to an existing super group */
  /* Groups arranged in no particular order, insert at head of list */

  struct GROUPIDS *temp_slot;
  
  if (mygrp == NULL)
    return(INVALID_GRP);
  if (mygrp -> grps . head_gids == NULL)
  { /* Insert at head - empty list */
    mygrp -> grps . head_gids = new GROUPIDS;
    (mygrp -> grps) . head_gids -> next_groupid = NULL;
  }
  else
  { /* Insert at head - shift list along */
    temp_slot = (mygrp -> grps) . head_gids;
    mygrp -> grps . head_gids = new GROUPIDS;
    ((mygrp -> grps) . head_gids) -> next_groupid = temp_slot;
  }
  mygrp -> grps . no_grps++;
  /* No checks on fidlds of groups */  
  mygrp -> grps . head_gids -> g_id = groups; 
  
  return(SUCCEEDED);
}/* end insertgrp_givengrp() */

int insert_servers (struct GROUP_INFO *supergrp, struct MID mysupergrpid, struct MID sr_to_insert, struct MID sender)
{
  /* Insert in 'supergrp' with id 'mysupergrpid' server 'sr_to_insert'.
   * This routine inserts a server within an existing super group 
   * servers are arranged in no particular order - insert at head of list
   */
  
  struct SERVERIDS  *temp_slot;
  struct GROUP_DESC *mygrp, temp_group;
  struct MID  groupmid;

  mygrp = supergrp->head_gd;

  if (mygrp == NULL)
    return(EMPTY_SUPERGRP);

  if (find_group(supergrp, mygrp, mysupergrpid) != SUCCEEDED)
    return(GRP_NOT_FOUND);

  if ((findserver_insg(supergrp, &temp_group, sender, &groupmid) != SUCCEEDED))
  {
    if (mygrp -> sids . head_sids == NULL)
    {
      /* Insert at head - empty list */
      mygrp -> sids . head_sids = new SERVERIDS;
      mygrp -> sids . head_sids -> next_serverid = NULL;
    }
    else
    {
      /* Insert at head - shift list along */
      temp_slot = mygrp -> sids . head_sids;
      mygrp -> sids . head_sids = new SERVERIDS;
      mygrp -> sids . head_sids -> next_serverid = temp_slot;
    }
    
    mygrp -> sids . no_srs++;
    /* No checks on fields of server */

    mygrp -> sids . head_sids -> s_id = sr_to_insert;
    mygrp -> sids . head_sids -> sender = sender;
  } /* end if */

  return(SUCCEEDED);
} /* end insert_servers() */


/***********************/
/* ALTERNATIVE ROUTINE */
/***********************/

int insert_srvrs (struct GROUP_INFO *supergrp, struct MID mysupergrpid, struct MID sr_to_insert)
{
  /* Insert in 'supergrp' with id 'mysupergrpid' server 'sr_to_insert'.
   * This routine inserts a server within an existing super group 
   * servers are arranged in no particular order - insert at head of list
   */
  
  struct GROUP_DESC *mygrp;

  mygrp = supergrp->head_gd;

  if (mygrp == NULL)
    return(EMPTY_SUPERGRP);

  if (find_group(supergrp, mygrp, mysupergrpid) != SUCCEEDED)
    return(GRP_NOT_FOUND);
  else
    insertsrvr_givengrp(mygrp, sr_to_insert);
  
  return(SUCCEEDED);
  
} /* end insert_srvrs() */
    
int insertsrvr_givengrp (struct GROUP_DESC *mygrp, struct MID server)
{ /* Insert a server within an existing super group */
  /* Servers arranged in no particular order - insert at head of list */

  struct SERVERIDS *temp_slot;
  
  if (mygrp == NULL)
    return(INVALID_GRP);
  if (mygrp -> sids . head_sids == NULL)
  { /* Insert at head - empty list */
    mygrp -> sids . head_sids = new SERVERIDS;
    mygrp -> sids . head_sids -> next_serverid = NULL;
  }
  else
  { /* Insert at head - shift list along */
    temp_slot = mygrp -> sids . head_sids;
    mygrp -> sids . head_sids = new SERVERIDS;
    mygrp -> sids . head_sids -> next_serverid = temp_slot;
  }
  
  mygrp -> sids . no_srs++;
  /* No checks on fields of server */
  mygrp -> sids . head_sids -> s_id = server;
  
  return(SUCCEEDED);
  
} /* end insertsrvr_givengrp() */
  
int remove_server (struct GROUP_INFO *super_grp, struct GROUP_DESC *mygrp, struct MID server, Boolean all, Boolean checksupergrp)
{ /* Remove a server from existing super group */
  
  /* Remove 'server' from 'mygrp' which is in 'super_grp', or remove ALL
   * servers from 'mygrp' if 'all' is set.  
   * 'Checksupergrp' set to FALSE if routine is called from 
   * removesupergroup(), in order to avoid unnecessary recursion.
   */

  struct SERVERIDS *current_server, *previous_server, *next_server;
  Boolean found, listisempty = TRUE;
  
  if (super_grp -> head_gd == NULL)
    return(EMPTY_SUPERGRP);
  if (!all)
  { /* Search through list for server id */

    current_server = mygrp -> sids . head_sids;
    previous_server = NULL;
    found = FALSE;
    while ((current_server != NULL) && (!found))
    {
      if (identical_group(&(current_server->s_id), &server))
      {
       	found = TRUE;
       	mygrp -> sids . no_srs--;

       	if (previous_server == NULL) /* Is server to be removed at head of list? */
       	  mygrp ->sids .head_sids = mygrp->sids . head_sids -> next_serverid;
       	else 
       	  previous_server -> next_serverid = current_server->next_serverid;
         /* Takes care of case where removed server is last in list, i.e.
          * new last element (or head - for empty list) now point to NULL
          */
       	  
       	delete current_server;
      }
      previous_server = current_server;
      current_server = current_server -> next_serverid;

    } /* end while */
      
      if (!found)
        return(SRVR_NOT_FOUND);
    } /* end if (!all) */
    else
    { /* Remove all servers from list */
      current_server = mygrp -> sids . head_sids;
      while (current_server != NULL)
      {
      	next_server = current_server -> next_serverid;
      	delete current_server;
      	current_server = next_server;
      }
      mygrp -> sids . head_sids = NULL;
      mygrp -> sids . no_srs = 0;
    }
    /* If no groups exist in list, it is no longer needed and is removed */
    if (!checksupergrp)
      return(SUCCEEDED);
    else
    {
      if (empty_grp(mygrp))
      {
      	if (remove_super_group(super_grp, mygrp, listisempty) != SUCCEEDED)
	  return(SUPERGRP_NOT_REMOVED);
	else
	  return(SUGRP_REMOVED);
      }
      else
        return(SUCCEEDED);
    }
}/* end remove_server() */

int remove_group (struct GROUP_INFO *super_grp, struct GROUP_DESC *mygrp, struct MID group_id, Boolean all, Boolean checksupergrp)
{ /* Remove a 'group' from an existing super group */
  /* Remove 'group_id' from 'mygrp'(which is in 'super_grp') or remove ALL 
   * groups if 'all' is set.
   * If 'checksupergrp' is set, then if 'super_grp' is empty, delete the 
   * entire structure
   */

   struct GROUPIDS *current_group, *previous_group, *next_group;
   Boolean found, listisempty = TRUE;

   if (super_grp == NULL)
     return(EMPTY_SUPERGRP);
   if (mygrp == NULL)
     return(INVALID_GRP);
   
   if (!all)
   { /* Search through 'mygrp' for 'group_id' */
     
     current_group = mygrp -> grps . head_gids;
     previous_group = NULL;
     found = FALSE;

     while ((current_group != NULL) && (!found))
     {
       if (identical_group(&(current_group->g_id), &group_id)) 
       {
         found = TRUE;
         mygrp -> grps . no_grps--;

         if (previous_group == NULL) /* Is group to be removed at head of list? */
           mygrp->grps.head_gids = mygrp->grps.head_gids->next_groupid;
         else 
           previous_group->next_groupid = current_group->next_groupid;
         /* Takes care of case where removed group is last in list, i.e.
          * new last element (or head - for empty list) now point to NULL
          */

         delete current_group;
       }
       previous_group = current_group;
       current_group = current_group->next_groupid;
     }/* end while */
     
     if (!found)
       return(GRP_NOT_FOUND);
   } /* end if (!all) */
   else
   { /* Remove all groups from list */
     current_group = mygrp -> grps . head_gids;
     while (current_group != NULL)
     {
     	next_group = current_group -> next_groupid;
     	delete current_group;
     	current_group = next_group;
     }
     mygrp -> grps . head_gids = NULL;
     mygrp -> grps . no_grps = 0;
   }

   /* If no servers exist in list, it is no longer needed and is removed */
   if (!checksupergrp)
     return(SUCCEEDED);
   else
   {
     if (empty_grp(mygrp))
     {
      	if (remove_super_group(super_grp, mygrp, listisempty) != SUCCEEDED)
	  return(SUPERGRP_NOT_REMOVED);
	else
	  return(SUGRP_REMOVED);
     }
     else
       return(SUCCEEDED);
   }
} /* end remove_group() */

void set_mid(struct MID *grp)
{
    grp->type = htonl(0);
    grp->hi_part = htonl(0);
    grp->lo_part = htonl(0);
}

int remove_super_group (struct GROUP_INFO *super_grp, struct GROUP_DESC *mygrp, Boolean listisempty)
{ /* Remove a super group from the list of super groups */
  
  /* Remove 'mygrp' from 'super_grp'.  If 'listisempty' is set, do not check
   * contents of group, since it is already empty, otherwise, do
   */
  
  struct GROUP_DESC *current_group, *previous_group;
  struct MID group_id;
  struct MID server;
  Boolean found, checksupergrp = FALSE, all = TRUE;
  int rcode = -1;

  set_mid(&group_id);
  set_mid(&server);

  if (super_grp -> head_gd == NULL)
    return(EMPTY_SUPERGRP);
  found = FALSE;

  /* Search through 'supergrp' for 'mygrp' */
  previous_group = NULL;
  current_group = super_grp -> head_gd;

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "Clientops::REMOVE_SUPER_GROUP(): mygrp <" << ntohl(mygrp->mymid.type) << ","
      << ntohl(mygrp->mymid.hi_part) << "," << ntohs(htonl(mygrp->mymid.lo_part)) << ">\n" << flush;
  debug_stream.unlock();
  
#endif
  
  /* Assume no repetition, this optimises the search time, by not
   * always searching entire list
   */
  while ((current_group != NULL) || (!found))
  {
    if (identical_group(&(current_group -> mymid), &(mygrp -> mymid))) 
    {
      found = TRUE;
      /* Empty list of all server and group entries - if necessary */
      if (!listisempty)
      {
	if ((rcode = remove_group(super_grp, mygrp, group_id, all, checksupergrp)) != SUCCEEDED)
	  return(rcode);
	if ((rcode = remove_server(super_grp, mygrp, server, all, checksupergrp)) != SUCCEEDED)
	  return(rcode);

      }

      if (previous_group == NULL) /* Is group to be removed at head of list? */
	super_grp -> head_gd = super_grp -> head_gd -> next_group;
      else
	previous_group -> next_group = current_group -> next_group;
       /* Takes care of case where removed group is last in list, i.e.
        * new last element (or head - for empty list) now point to NULL
        */

      delete current_group;
      break;
    }

    previous_group = current_group;
    current_group = current_group -> next_group;

  } /* end while */
  
  super_grp -> no_supergrps--;

  if (!found)
    return(GRP_NOT_FOUND);
  else
    return(SUCCEEDED);

} /* end remove_super_group() */
  
int rmall_supergrps (struct GROUP_INFO *super_grp)
{ /* Remove the entire structure of super groups */
  struct GROUP_DESC *current_group, *next_group;
  Boolean listisempty = FALSE; /* Recent change made, should be FALSE? */

  if (super_grp -> head_gd == NULL)
    return(EMPTY_SUPERGRP);
  
  current_group = super_grp -> head_gd;
  next_group = NULL;
  
  while (current_group != NULL)
  { /* Remove all groups from list */
  
    next_group = current_group -> next_group;
    remove_super_group(super_grp, current_group, listisempty);
    current_group = next_group;
  }
  
  super_grp -> no_supergrps = 0; /* In the absence of faults, should be asserted */
  return(SUCCEEDED);
  
} /* end rmall_supergrps() */
  
void traverse_server (struct GROUP_DESC *mygrp)
{ /* Traverse a server structure */
  
  struct SERVERIDS *current_server;


  current_server = mygrp -> sids . head_sids;

  if (current_server == NULL)
    return;

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "SERVERIDS::\n" << flush;
  debug_stream.unlock();
  
#endif

  while (current_server != NULL)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "<" << ntohl(current_server->s_id.type) << "," << ntohl(current_server->s_id.hi_part)
	  << "," << ntohs(htonl(current_server->s_id.lo_part)) << ">\n" << flush;
      debug_stream.unlock();
      
#endif           
    current_server = current_server -> next_serverid;
  }
} /* end traverse_server() */

void traverse_group (struct GROUP_DESC *mygrp)
{ /* Traverse a 'group' structure */
  
  struct GROUPIDS *current_group;
  
  current_group = mygrp -> grps . head_gids;

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "GROUPIDS::\n" << flush;
  debug_stream.unlock();
  
#endif

  while (current_group != NULL)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "<" << ntohl(current_group->g_id.type) << "," << ntohl(current_group->g_id.hi_part)
	  << "," << ntohs(htonl(current_group->g_id.lo_part)) << ">\n" << flush;
      debug_stream.unlock();
      
#endif           
    current_group = current_group -> next_groupid;
  }
} /* end traverse_group() */

void traverse_supergrp (struct GROUP_DESC *mygrp)
{ /* Traverse a super group structure */
  
  if (!empty_grp(mygrp))
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "SUPERGROUP: <" << ntohl(mygrp->mymid.type) << "," << ntohl(mygrp->mymid.hi_part)
	  << "," << ntohs(htonl(mygrp->mymid.lo_part)) << "> contains " << mygrp->grps.no_grps << " groups, "
	      << mygrp->sids.no_srs << " servers as follows\n" << flush;
      debug_stream.unlock();
      
#endif
    traverse_group(mygrp);
    traverse_server(mygrp);
  }
} /* end traverse_supergrp() */

void traverseall_supergrps (struct GROUP_INFO *super_grp)
{ /* Traverse the entire structure of super groups */
  
  struct GROUP_DESC *current_supergrp;

#ifdef DEBUG
  debug_stream.lock();
  
  if (super_grp -> head_gd != NULL)
  {
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "PRINTING ALL SUPERGROUPS: NO SUPERGROUPS = " << super_grp->no_supergrps
	  << "\n" << flush;
  }
  else
  {
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\nPRINT_SUPERGROUPS: NO SUPERGROUPS = " <<
	  super_grp->no_supergrps << "\n" << flush;
  }
  debug_stream.unlock();
  
#endif

  current_supergrp = super_grp -> head_gd;
  while (current_supergrp != NULL)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\nSUPERGROUP:: <" << ntohl(current_supergrp->mymid.type) << ","
	  << ntohl(current_supergrp->mymid.hi_part) << "," << ntohs(htonl(current_supergrp->mymid.lo_part)) << "> contains"
	      << current_supergrp->grps.no_grps << " groups, " << current_supergrp->sids.no_srs
		  << " servers as follows\n" << flush;
      debug_stream.unlock();
      
#endif
    traverse_group(current_supergrp);
    traverse_server(current_supergrp);
    current_supergrp = current_supergrp -> next_group;
  }
} /* end traverseall_supergrps() */

/********************************************/
/* Routines that manipulate the GROUPS type */
/********************************************/

void set_init_type (struct GROUPS *grps)
{

  grps -> groups_type = UNSET_type;

} /* end set_init_type */

int set_srvr_type (struct GROUPS *grps, int server_type)
{

  switch (server_type)
  {
  case UID_type:
    grps -> groups_type = SRVR_type;
    grps -> grp_srvr . srvr . sr_idtype = UID_type;
    break;
    
  case MID_type:
    grps -> groups_type = SRVR_type;
    grps -> grp_srvr . srvr . sr_idtype = MID_type;
    break;

  default:
    grps -> groups_type = grps -> grp_srvr . srvr . sr_idtype = UNSET_type;
    return(FAILED);
  }

  return(SUCCEEDED);

} /* end set_srvr_type */

void set_grp_type (struct GROUPS *grps)
{

  grps -> groups_type = GRP_type;

} /* end set_grp_type */

void get_srvr (struct GROUPS  *grps, struct  MID *server)
{

  server = &(grps -> grp_srvr . srvr . sr_id);

} /* end get_srvr */

void get_grp (struct GROUPS* grps, struct GROUP_INFO *grp)
{

  grp = &(grps -> grp_srvr . group);

} /* end get_grp */

/*******************************************************************/
/* Routines that manipulate the reply group expected by the client */
/*******************************************************************/

void init_replygrp (struct GROUP_INFO  *replygrp, int *no_servers)
{
  /* Keep temporary working copy of super group.  Only server entries
   * are required, all group entries are removed from temporary copy.
   */

  struct GROUP_DESC   *current_sg;
  struct MID          redundantmid;
  Boolean             all = TRUE, checksupergrp = TRUE;

#ifdef DEBUG
  struct SERVERIDS    *srvrs;
#endif

  set_mid(&redundantmid);

  *no_servers = 0;

  current_sg = replygrp -> head_gd;
  while (current_sg != NULL)
  { 
    *no_servers = *no_servers + (current_sg -> sids) . no_srs;

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "init_reply: no. server " << current_sg->sids.no_srs << "\n" << flush;
    srvrs = current_sg->sids.head_sids;
    while (srvrs != NULL)
    {
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "server " << flush;
	printgid(srvrs->s_id);
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "sender " << flush;
	printgid(srvrs->sender);
	srvrs = srvrs -> next_serverid;
    }
    debug_stream.unlock();
    
#endif
    
    /* Remove all group entries */
    remove_group(replygrp, current_sg, redundantmid, all, checksupergrp);
    current_sg = current_sg -> next_group;
  }
} /* end init_replygrp() */

int findserver_insg (struct GROUP_INFO *supergrp, struct GROUP_DESC *mygrp, struct MID sender, struct MID *srvrid)
{
  /* Find 'sender' in 'supergrp' and return head of super group in 'mygrp'
   * (needed for other routines); return SUCCEEDED if found and the 
   * corresponding server id via 'srvrid'; return FAILED otherwise.
   * Return SRVR_NOT_FOUND if supergroup is empty.
   */
  
  struct SERVERIDS *current_server;
  Boolean found = FALSE;

  current_server = supergrp ->head_gd->sids.head_sids;
  if (current_server == NULL)
    return(SRVR_NOT_FOUND);
  do
  {
    if (identical_group(&sender, &(current_server->sender)))  
      found = TRUE;
    else
      current_server = current_server -> next_serverid;
  } while ((!found) && (current_server != NULL));

  if (found)
  {
    mygrp = supergrp->head_gd;
    *srvrid = current_server->s_id;
    return(SUCCEEDED);
  }
  else
    return(FAILED);
  
}/* end findserver_insg() */

int dec_replygrp (struct GROUP_INFO *replygrp, struct MID sender)
{ /* Remove 'sender' from 'replygrp', and remove 'replygrp' if it is empty */

  struct GROUP_DESC   *mygrp;
  struct MID          srvrid;
  Boolean             checksupergroup = TRUE, all = FALSE;
  int                 rcode = FAILED, found;

  mygrp = replygrp -> head_gd;
  if ((found = findserver_insg(replygrp, mygrp, sender, &srvrid)) != SUCCEEDED)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dec_replygrp: server not found in group, rcode was " << found << "\n" << flush;
      debug_stream.unlock();
      
#endif    
  }
  else
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dec_replygrp: server found in group.\ndec_replygrp: sender: " << flush;
      printgid(sender);

      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "server " << flush;
      printgid(srvrid);
      debug_stream.unlock();
      
#endif    
    remove_server(replygrp, mygrp, srvrid, all, checksupergroup);
    rcode = SUCCEEDED;
  }
  return(rcode);

} /* end dec_replygrp() */

Boolean isempty_replygrp (struct GROUP_INFO  *replygrp)
{ /* Group is empty if no servers are remaining
   * In this case, groups within groups are ignored
   */

  return ((replygrp->head_gd == NULL) ? TRUE : FALSE);

} /* end isempty_replygrp() */

int get_next_server (struct GROUP_INFO *replygrp, struct MID *serverid, struct MID *sender)
{
  /* Return the next 'serverid' in the supergroup - if one exists
   * and its corresponding 'sender' entry
   */

  if ((replygrp -> head_gd == NULL) ||
      ((replygrp -> head_gd -> sids) . head_sids == NULL))
    return(NO_SERVERS);
  else
  {
    *serverid = (replygrp -> head_gd -> sids) . head_sids -> s_id;
    *sender = (replygrp -> head_gd -> sids) . head_sids -> sender;
    return(SUCCEEDED);
  }

} /* end get_next_server */

int get_filtserver (struct GROUP_INFO *filt_grp, struct MID *serverid, struct MID *sender)
{
  /* Get the next server that has not replied if one exists and return 
   * SUCCEEDED,otherwise return FAILED.  Call to get_filt_server should 
   * be preceeded by invocation of isempty_filtgrp()
   */

  struct SERVERIDS *current_server;
  Boolean          set = FALSE;

  if ((filt_grp->head_gd == NULL) ||
      (filt_grp->head_gd->sids.head_sids == NULL))
    return(EMPTY_SUPERGRP);

  current_server = filt_grp->head_gd->sids.head_sids;
  do
  {
    if ((set = current_server -> replied))
      current_server = current_server -> next_serverid;

  } while ((current_server != NULL) && (set));

  if (!set)
  {
    *serverid = current_server->s_id;
    *sender   = current_server->sender;
    return(SUCCEEDED);
  }
  else
    return(FAILED);

} /* end get_filtserver() */

int get_noservers (struct GROUP_INFO *filt_grp)
{
  /* Return the number of servers that exist in supplied group */

  struct SERVERIDS *current_server;
  int no_servers;

  no_servers = 0;

  if ((filt_grp->head_gd != NULL) &&
      (filt_grp->head_gd->sids.head_sids != NULL))
  {
    current_server = filt_grp->head_gd->sids.head_sids;
    do
    {
      no_servers++;
      current_server = current_server -> next_serverid;
    } while (current_server != NULL);
  }

  return(no_servers);

} /* end get_noservers() */

void set_grp_info (struct GROUPS *group, struct MID groupid)
{
  /* Set details for group - invoked by initiate when it completes successfully
   * to set its output group parameters
   */
  
  group->groups_type = GRP_type;
  group->grp_srvr.srvr.sr_idtype = 0;
  group->grp_srvr.srvr.sr_id.type = htonl(0);
  group->grp_srvr.srvr.sr_id.hi_part = htonl(0);
  group->grp_srvr.srvr.sr_id.lo_part = htonl(0);

  initialise_supergroup(&(group->grp_srvr.group)); 
  insert_super_group(&(group->grp_srvr.group), groupid);  
  
  /* All routines should succeed, error checks done earlier */

} /* end set_grp_info() */
