#ifndef GROUPID_H_
#  include "GroupID.h"
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif

ClientData::ClientData ()
                        : myClient(0),
			  next(0)
{
}

ClientData::~ClientData ()
{
    if (myClient)
	delete myClient;
}


GroupID::GroupID ()
                      : head(0),
			numberOfMembers(0)
{
}

GroupID::~GroupID ()
{
    if (head)
	deleteList();
}

void GroupID::deleteList ()
{
    ClientData* ptr;
    
    while (head)
    {
	ptr = head->next;
	delete head;
	head = ptr;
    }
}

void GroupID::addMember (Client* toAdd)
{
    ClientData* ptr;
    
    if (!toAdd)
	return;
    
    if (!head)
    {
	head = new ClientData;
	ptr = head;
    }
    else
    {
	ptr = head;
	while (ptr->next)
	    ptr = ptr->next;
	
	ptr->next = new ClientData;
	ptr = ptr->next;
    }
    
    numberOfMembers++;
    ptr->myClient = toAdd;
}

void GroupID::deleteMember (Client* toDelete)
{
    ClientData *ptr = 0, *marker = 0;
    Boolean found = FALSE;
    
    if ((!toDelete) || (!head))
	return;
    
    if (head->myClient == toDelete)
    {
	ptr = head;
	head = ptr->next;
	delete ptr;
	numberOfMembers--;
	return;
    }

    ptr = head;
    while ((ptr->next) && (!found))
    {
	if (ptr->next->myClient == toDelete)
	    found = TRUE;
	else
	    ptr = ptr->next;
    }

    if (found)
    {
	marker = ptr;
	ptr = ptr->next;
	marker->next = ptr->next;
	numberOfMembers--;
	delete ptr;
    }
}

ClientData* GroupID::getList () const
{
    return head;
}

int GroupID::getNumberOfMembers () const
{
    return numberOfMembers;
}

Boolean GroupID::pack (Buffer& buff) const
{
    ClientData* ptr = head;
    
    Boolean result = buff.pack(numberOfMembers);
    
    for (int i = 0; ((i < numberOfMembers) && result); i++)
	result = result && ptr->myClient->pack(buff);

    return result;
}

Boolean GroupID::unpack (Buffer buff)
{
    Boolean result;
    ClientData* ptr = 0;
    
    if (numberOfMembers != 0)
	return FALSE;
    
    head = new ClientData;
    result = buff.unpack(numberOfMembers);
    
    for (int i = 0; ((i < numberOfMembers) && result); i++)
    {
	if (!ptr)
	    ptr = head;
	else
	{
	    ptr->next = new ClientData;
	    ptr = ptr->next;
	}
	    
	ptr->myClient = new Client;
	result = ptr->myClient->unpack(buff);
    }
    
    return result;
}
