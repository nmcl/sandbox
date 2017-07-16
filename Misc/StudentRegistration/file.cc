#include <stdio.h>
#include <ndbm.h>
#include <sys/file.h>
#include <memory.h>
#include "userdb.h"
#include "Requests.h"

/*
 * Arjuna specific header files.
 */

#include <sys/param.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream.h>
#include <StubGen/ClientRpc.h>
#include <StubGen/ClientRpcMan.h>
#include <SOutCome_stub.h>
#include <StudentRecord_stub.h>
#include <ReplicatedCounter_stub.h>
#include <Arjuna/AtomicA.h>
#include "arjuna.h"

StudentRecord*    theRecord = NULL;
ClientRpc*        clientRpc = (ClientRpc*) 0;
ClientRpcManager* RpcCont   = (ClientRpcManager*) 0;
DBM *dbFile = NULL;
AtomicAction* action = NULL;
Boolean (*oldHandler) (RPC_Status, Int32);
datum Key, Data;

extern char *RecordLocked;


Boolean rpc_handler (RPC_Status, Int32) 
{
    return FALSE;
}

void tidyUp ()
{
    if (action)
    {
	delete action;
	action = NULL;
    }
    if (sr)
    {
	delete sr;
	sr = NULL;
    }
    if (RpcCont)
    {
	delete RpcCont;
	RpcCont = NULL;
    }
}

/*
 * Cycle through the list of available server machines, based upon
 * the student id, trying to activate the server.
 */

Boolean getServer (const char* stunumber, Uid& id)
{
    if (theRecord != NULL)  // we already have a record server!
	return FALSE;
  
    char hostName[MAXHOSTNAMELEN];

    for (;;)
    {
	int rem = atoi(stunumber) % 60;
	int pref = rem % NumberOfServers;
	SOutCome result = UNKNOWN;
  
	::strcpy(hostName, ServerList[pref]);

	oldHandler = setHandler(rpc_handler);
	clientRpc = new ClientRpc("RecordServer");
	clientRpc->setHost(hostName);
	clientRpc->setRetry(serverRetry);
	clientRpc->setTimeout(serverTimeout);
	RpcCont = new ClientRpcManager(clientRpc);

	/*
	 * Recreate existing record, or create a new record?
	 */
	
	if (id != NIL_UID)
	    theRecord = new StudentRecord(result, id, RpcCont);
	else
	{
	    theRecord = new StudentRecord(result, RpcCont);
	    id = theRecord->get_uid();
	}

	if (result == DONE)
	    return TRUE;

	delete sr;
	delete RpcCont;

	for (int i = pref; i < NumberOfServers -1; i++)
	    ServerList[i] = ServerList[i+1];
	NumberOfServers--;

	if (NumberOfServers <= MIN_SERVERS)
	    return FALSE;
    }

    return FALSE;  // remove compiler warning
}

/*
 * Returns a counter value.
 */

int getCounter (int& error)
{
    Boolean res = FALSE;
    int counterValue = 0;
    ClientRpc* cRpc = new ClientRpc("ReplicatedCounter");
    cRpc->setHost(counterHost);
    cRpc->setRetry(serverRetry);
    cRpc->setTimeout(serverTimeout);
    ClientRpcManager* RpcC = new ClientRpcManager(clientRpc);
    ReplicatedCounter* counter = new ReplicatedCounter(res, counterUid, RpcC);

    if (res)
    {
	AtomicAction B;

	B.Begin();

	counter->getCounter(counterValue, res);

	if (res)
	    B.End();
	else
	{
	    B.Abort();
	    counterValue = -1;
	    error = 2;
	}
    }
    else
    {
	counterValue = -1;
	error = 2;
    }
	
    delete counter;
    delete RpcC;

    return counterValue;
}

/*
 * Using the counter value as the key into the db, get the data entry,
 * and extract from that the student number and the id.
 */

Boolean getNewStudentNumber (int counterValue, char* StudNumb, Uid& id)
{
    dbFile = dbm_open(newKeyMap, O_RDONLY, 0);

    if (!dbFile)
	return FALSE;

    Boolean result = FALSE;
    
    Key.dptr = (char*) counterValue;
    Key.dsize = sizeof(counterValue);

    Data = dbm_fetch(dbFile, Key);

    if (Data.dsize > 0)
    {
	::memcpy(StudNumb, Data.dptr, STUDENTNUMBER);
	Uid tempId(Data.ptr+STUDENTNUMBER);
	id = tempId;
	result = TRUE;
    }

    dbm_close(dbFile);

    return result;
}

/*
 * Try to get the overflow lock. We attempt this a few times in
 * case of lock conflict.
 */

Boolean lock (int fd)
{
    struct flock arg;
    Boolean result = FALSE;
    int retry = fileRetry;
    
    arg.l_whence = SEEK_SET;
    arg.l_start = 0;
    arg.l_len = 0;
    arg.l_type = F_WRLCK;

    do
    {
	if (::fcntl(fd, F_SETLKW, &arg) == -1)
	{
	    retry--;
	    sleep(fileTimeout);
	}
	else
	    result = TRUE;
      
    } while ((retry > 0) && (!result));

    return result;
}

/*
 * Release the lock. If this fails, then there's very little we
 * can do anyway.
 */
 
void unlock (int fd)
{
    struct flock arg;

    arg.l_whence = SEEK_SET;
    arg.l_start = 0;
    arg.l_len = 0;
    arg.l_type = F_UNLCK;

    ::fcntl(fd, F_SETLK, &arg);
}

/*
 * Given the student number, find the corresponding Uid. This may
 * involve looking in all three of the dbm files.
 */

Boolean mapId (const char* stunumber, Uid& id)
{
    Uid* tempId = (Uid*) 0;  
    dbFile = dbm_open(staticKeyMap, O_RDONLY, 0);

    if (dbFile == NULL)
	return FALSE;

    Key.dptr = stunumber;
    Key.dsize = STUDENTNUMBER;
  
    Data = dbm_fetch(dbFile, Key);

    /*
     * If not in standard mapping file, look in overflow files.
     * The first file contains the mapping of new ids to Uids (read only).
     * The second file is for unexpected returning students (writable).
     */
  
    if (Data.dsize <= 0)
    {
	dbm_close(dbFile);  // main db

	/*
	 * Try mapping for new students.
	 */

	dbFile = dbm_open(newKeyMap, O_RDONLY, 0666);

	if (dbFile == NULL)
	    return FALSE;

	Data = dbm_fetch(dbFile, Key);

	if (Data.dsize <= 0)
	{
	    /*
	     * Try mapping for unexpected returners.
	     */

	    dbm_close(dbFile);  // new student db
	    
	    /*
	     * Open and get lock to prevent inconsistent accesses.
	     */
	    
	    int fd = ::open(overflowLockFile, O_RDWR | O_CREAT | SYNC, 0666);

	    if ((fd == -1) || (!lock(fd)))
		return FALSE;

	    /*
	     * We only need this in read mode, since we are not
	     * about to modify its contents.
	     */
	    
	    dbFile = dbm_open(overflowKeyMap, O_RDONLY, 0666);

	    if (dbFile == NULL)
		return FALSE;
	    
	    Data = dbm_fetch(dbFile, Key);

	    Boolean result = FALSE;
	    
	    if (Data.dsize > 0)
	    {
		tempId = new Uid(Data.dptr);
		result = TRUE;
	    }
	
	    dbm_close(dbFile);  // overflow db
	    unlock(fd);
	    ::close(fd);

	    if (!result)
		return FALSE;
	}
	else
	{
	    tempId = new Uid(Data.dptr);
	    dbm_close(dbFile);  // new student db
	}
    }
    else
    {
	tempId = new Uid(Data.dptr);
	dbm_close(dbFile);  // main db
    }

    id = *tempId;
    delete tempId;

    return TRUE;
}

/*
 * Lock is used to determine whether we open-read-close within
 * this routine.
 */

int DBFetch (char* StudNumb, char* Reply, int Lock)
{
    /*
     * Check we are not already servicing a student record.
     */
    
    if ((::memcpy(RecordLocked, NORECORD, STUDENTNUMBER) != 0) && (Lock))
	return 11;

    /*
     * If we have a running action, then abort it. (We should
     * never encounter this situation, but ...)
     */
    
    if (action)
    {
	action->Abort();
	action = NULL;
    }
    
    /*
     * Get mapping of student id to Uid. If this fails, then
     * we must give up.
     */

    Uid id;
    
    if (!mapId(StudNumb, id))
    {
	cerr << "Failed to map student number to Uid" << endl;
	return 1;
    }

    /*
     * Now try to activate a server to access the object through.
     */
    
    if (!getServer(StudNumb, id))
    {
	cerr << "Failed to get student record server." << endl;
	return 3;
    }

    SOutcome res = UNKNOWN;
    Buffer buff;
    int value = 4;  // return code

    /*
     * This action will typically out-live this routine, and only
     * get terminated within save or close.
     */
    
    action = new AtomicAction;
    action->Begin();
    
    if (!Lock)  // open, get record, close.
    {
	theRecord->fetchRecord(res, buff);

	if (res != DONE)  // lock conflict
	{
	    cerr << "Failed to fetch record." << endl;
	    action->Abort();
	}
	else
	{
	    /*
	     * Keep the atomic action running until close/save called.
	     */
	    
	    void* ptr = NULL;
	    size_t length = 0;
	    
	    if (buff.unpack(ptr, length))
	    {
		::memcpy(Reply, ptr, length);
		::delete [] ptr;
	    
		value = 0;
		action->End();  // finished ok
	    }
	    else
		action->Abort();  // unpack error
	}
	
	tidyUp();  // finished with this server and action.

	return value;
    }

    theRecord->fetchExclusiveRecord(res, buff);

    if (res != DONE)
    {
	action->Abort();
	tidyUp();
    }
    else
    {
	::memcpy(RecordLocked, StudNumb, STUDENTNUMBER);

	/*
	 * Keep the atomic action running until close/save called.
	 */
	    
	void* ptr = NULL;
	size_t length = 0;

	if (buff.unpack(ptr, length))
	{
	    ::memcpy(Reply, ptr, length);
	    ::delete [] ptr;
	    value = 0;
	}
	else
	{
	    action->Abort();  // SHOULD WE?
	    tidyUp();
	    value = 12;
	}
    }

    return value;
}

int DBStore (char* NewData)
{
    /*
     * Are we running an action and servicing a student?
     */
    
    if ((!action) || (::memcmp(RecordLocked, NewData, STUDENTNUMBER) != 0))
	return 11;

    SOutcome res = UNKNOWN;
    Buffer buff;
    int value = 4;  // return code

    if (!buff.packNonMappedBlock(NewData, RECSIZE))
    {
	action->Abort();
	value = 12;  // general fault
    }
    else
    {
	sr->replaceRecord(res, buff);

	if (res != DONE)
	    action->Abort();
	else
	{
	    action->End();
	    value = 0;
	}
    }

    ::memcpy(RecordLocked, NORECORD, STUDENTNUMBER);
    
    tidyUp();  // SHOULD WE ABORT AND TEAR DOWN THE SERVER FOR PACK ERROR?
    
    return value;
}

/*
 * Since this operation means close and do not modify the record, we
 * may as well abort.
 */

int DBClose (char* StudNum)
{
    if ((!action) || (::memcmp(RecordLocked, StudNum, STUDENTNUMBER) != 0))
	return 6;

    action->Abort();
    tidyUp();
    
    ::memcpy(RecordLocked, NORECORD, STUDENTNUMBER);
    
    return 0;
}

void CreateBlankRec (char* Record)
{
    char *Surname = "", *OtherNames = "", *StudType = "U",
	*CourseType = "T", *MStat = "S", *Sex = "M", *Title = "",
	*Initials = "", *DOB = "", *MOB = "", *YOB = "", *National = "2826",
	*BirthCountry = "2826", *Domicile = "2826", *RStat = "H",
	*FeeStat = "", *XTime = "F", *XYear = "F", *Reside = "", *Occup = "",
	*HAdd1 = "", *HAdd2 = "", *HAdd3 = "", *HAdd4 = "", *HAdd5 = "",
	*HPostcode = "", *HTelephone = "", *TTAdd1 = "", *TTAdd2 = "",
	*TTAdd3 = "", *TTAdd4 = "", *TTAdd5 = "", *TTPostcode = "",
	*TTTelephone = "", *NOKSurname = "", *NOKForename = "", *NOKRel = "",
	*NOKAdd1 = "", *NOKAdd2 = "", *NOKAdd3 = "",*NOKAdd4 = "",
	*NOKAdd5 = "", *NOKPostcode = "", *NOKTelephone = "", *StudyCourse = "",
	*Stage = "", *GAB = "", *TuitFee = "", *Tutor = "00000000",
	*Ethnic = "99", *Disabled = "9", *Disability = "", *UKCounty = "",
	*TutDept = "0000", *LocalAddr = "", *DateStamp = "00000000003600  T",
	*AccessState = "D", *Sem1 = "", *Sem2 = "", *Sem3 = "";

    memcpy(&Record[ 9 ], Surname, strlen(Surname));
    memcpy(&Record[ 47 ], OtherNames, strlen(OtherNames));
    memcpy(&Record[ 112 ], StudType, strlen(StudType));
    memcpy(&Record[ 113 ], CourseType, strlen(CourseType));
    memcpy(&Record[ 114 ], MStat, strlen(MStat));
    memcpy(&Record[ 115 ], Sex, strlen(Sex));
    memcpy(&Record[ 116 ], Title, strlen(Title));
    memcpy(&Record[ 120 ], Initials, strlen(Initials));
    memcpy(&Record[ 124 ], DOB, strlen(DOB));
    memcpy(&Record[ 126 ], MOB, strlen(MOB));
    memcpy(&Record[ 128 ], YOB, strlen(YOB));
    memcpy(&Record[ 132 ], National, strlen(National));
    memcpy(&Record[ 136 ], BirthCountry, strlen(BirthCountry));
    memcpy(&Record[ 140 ], Domicile, strlen(Domicile));
    memcpy(&Record[ 144 ], RStat, strlen(RStat));
    memcpy(&Record[ 145 ], FeeStat, strlen(FeeStat));
    memcpy(&Record[ 146 ], XTime, strlen(XTime));
    memcpy(&Record[ 147 ], XYear, strlen(XYear));
    memcpy(&Record[ 148 ], Reside, strlen(Reside));
    memcpy(&Record[ 149 ], Occup, strlen(Occup));
    memcpy(&Record[ 152 ], HAdd1, strlen(HAdd1));
    memcpy(&Record[ 182 ], HAdd2, strlen(HAdd2));
    memcpy(&Record[ 212 ], HAdd3, strlen(HAdd3));
    memcpy(&Record[ 242 ], HAdd4, strlen(HAdd4));
    memcpy(&Record[ 272 ], HAdd5, strlen(HAdd5));
    memcpy(&Record[ 302 ], HPostcode, strlen(HPostcode));
    memcpy(&Record[ 310 ], HTelephone, strlen(HTelephone));
    memcpy(&Record[ 330 ], TTAdd1, strlen(TTAdd1));
    memcpy(&Record[ 360 ], TTAdd2, strlen(TTAdd2));
    memcpy(&Record[ 390 ], TTAdd3, strlen(TTAdd3));
    memcpy(&Record[ 420 ], TTAdd4, strlen(TTAdd4));
    memcpy(&Record[ 450 ], TTAdd5, strlen(TTAdd5));
    memcpy(&Record[ 480 ], TTPostcode, strlen(TTPostcode));
    memcpy(&Record[ 488 ], TTTelephone, strlen(TTTelephone));
    memcpy(&Record[ 508 ], NOKSurname, strlen(NOKSurname));
    memcpy(&Record[ 538 ], NOKForename, strlen(NOKForename));
    memcpy(&Record[ 568 ], NOKRel, strlen(NOKRel));
    memcpy(&Record[ 591 ], NOKAdd1, strlen(NOKAdd1));
    memcpy(&Record[ 621 ], NOKAdd2, strlen(NOKAdd2));
    memcpy(&Record[ 651 ], NOKAdd3, strlen(NOKAdd3));
    memcpy(&Record[ 681 ], NOKAdd4, strlen(NOKAdd4));
    memcpy(&Record[ 711 ], NOKAdd5, strlen(NOKAdd5));
    memcpy(&Record[ 741 ], NOKPostcode, strlen(NOKPostcode));
    memcpy(&Record[ 749 ], NOKTelephone, strlen(NOKTelephone));
    memcpy(&Record[ 769 ], StudyCourse, strlen(StudyCourse));
    memcpy(&Record[ 773 ], Stage, strlen(Stage));
    memcpy(&Record[ 774 ], GAB, strlen(GAB));
    memcpy(&Record[ 780 ], TuitFee, strlen(TuitFee));
    memcpy(&Record[ 789 ], Tutor, strlen(Tutor));
    memcpy(&Record[ 797 ], Ethnic, strlen(Ethnic));
    memcpy(&Record[ 799 ], Disabled, strlen(Disabled));
    memcpy(&Record[ 800 ], Disability, strlen(Disability));
    memcpy(&Record[ 802 ], UKCounty, strlen(UKCounty));
    memcpy(&Record[ 806 ], TutDept, strlen(TutDept));
    memcpy(&Record[ 810 ], LocalAddr, strlen(LocalAddr));
    memcpy(&Record[ 812 ], DateStamp, strlen(DateStamp));
    memcpy(&Record[ 831 ], AccessState, strlen(AccessState));
    memcpy(&Record[ 832 ], Sem1, strlen(Sem1));
    memcpy(&Record[ 916 ], Sem2, strlen(Sem2));
    memcpy(&Record[ 1000 ], Sem3, strlen(Sem3));
};

/*
 * If the student number is all 0s then we must get a new one from
 * the newKeyMap, where the mapping to Uid is already set. Otherwise
 * we must create a new student is and assign the mapping to Uid in
 * the overflowKeyMap.
 */

int DBNew (char* StudNumb, char* Reply)
{
    /*
     * Is there a running action, or are we servicing a student?
     */
    
    if ((action) || (::memcmp(RecordLocked, NORECORD, STUDENTNUMBER) != 0))
	return 15;

    Uid id(NIL_UID);
    char NewKey[STUDENTNUMBER];
    
    if (::memcmp(StudNumb, CREATENEWPATTERN, STUDENTNUMBER) == 0)
    {
	/*
	 * All 0s.
	 */
	
	int error = 2;
	int counterValue = getCounter(error);

	if (counterValue < 0)
	    return error;

	if (!getNewStudentNumber(counterValue, NewKey, id))
	    return 3;

	if (!getServer(NewKey, id))
	    return 3;
    }
    else
    {
	::memcpy(NewKey, StudNumb, STUDENTNUMBER);
	
	/*
	 * Have a student number, but no mapping to Uid. So, create
	 * a new persistent object, and get its Uid for insertion
	 * in the overflow db.
	 */

	if (!getServer(NewKey, id))
	    return 3;

	int fd = ::open(overflowLockFile, O_RDWR | O_CREAT | SYNC, 0666);
	int error = 6;
	
	if ((fd == -1) || (!lock(fd)))
	{
	    tidyUp();
	    return 6;
	}

	dbFile = dbm_open(overflowKeyMap, O_RDWR, 0666);
	
	if (dbFile)
	{
	    Key.dptr = NewKey;
	    Key.dsize = STUDENTNUMBER;
	    Data.dptr = ;
	    Data.dsize = ;

	    if (dbm_store(dbFile, Key, Data, DBM_REPLACE) != -1)
		error = 0;
	}

	dbm_close(dbFile);
	unlock(fd);
	::close(fd);
	
	if (error > 0)
	{
	    tidyUp();
	    return error;
	}
    }
    
    ::memset(&Reply[ 23 ], ' ', RECSIZE);
    ::memcpy(&Reply[ 23 ], NewKey, STUDENTNUMBER);
    CreateBlankRec(&Reply[ 23 ]);

    Buffer buff;
    SOutcome res = UNKNOWN;

    /*
     * If we fail at this point then we have already set the mapping
     * up in the db file for unexpected arrivals. This should not be
     * a problem for subsequent retries.
     */
	 
    if (!buff.packNonMappedBlock(Reply[23], RECSIZE))
    {
	tidyUp();
	return 13;
    }

    action = new AtomicAction;
    action->Begin();

    theRecord->replaceRecord(buff, res);

    if (res != DONE)
    {
	action->Abort();
	tidyUp();
	return 13;
    }

    ::memcpy(RecordLocked, NewKey, STUDENTNUMBER);
    
    return 0;
}

/*
 * Delete a student record which has already been opened, i.e., we
 * have an action and server running.
 */

int DBDel (char* StudNumb)
{
    if ((::memcmp(RecordLocked, NORECORD, STUDENTNUMBER) == 0) ||
	(::memcmp(RecordLocked, StudNumb, STUDENTNUMBER) != 0) || (!action))
	return 6;

    SOutcome res = UNKNOWN;
    int value = 5;
    
    theRecord->deleteRecord(res);

    if (res == DONE)
    {
	action->End();
	value = 0;
    }
    else
	action->Abort();

    tidyUp();

    return value;
}
