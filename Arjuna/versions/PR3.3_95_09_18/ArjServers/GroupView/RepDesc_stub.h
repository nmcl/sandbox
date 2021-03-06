/*
 * Automatically Generated by:
 * Arjuna Stub Generator Version 5.5.3 - $Date: 1996/09/10 15:38:44 $
 *
 * Do Not Edit By Hand
 *
 */

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef REPDESC_STUB_H_
#define REPDESC_STUB_H_

#ifndef STUBGEN_TYPES_H_
#  include <StubGen/Types.h>
#endif

class RpcBuffer;
class ClientRpcManager;


#include <Common/Uid_stub.h>

#include "ReplicaData_stub.h"
/* @NoRemote */ class RpcBuffer ;
/* @NoRemote */ class ostream ;
/* @NoRemote */ class GroupData ;
/* @NoRemote, @UserMarshall */ class ReplicaDescriptor: public ReplicaData
{
	/* @NoMarshall */ friend RpcBuffer& operator << ( RpcBuffer& , ReplicaDescriptor* );
	/* @NoMarshall */ friend RpcBuffer& operator << ( RpcBuffer& , ReplicaDescriptor& );
	/* @NoMarshall */ friend RpcBuffer& operator << ( RpcBuffer& , const ReplicaDescriptor* );
public:
	/* @NoMarshall */ ReplicaDescriptor ( );
	/* @NoMarshall */ ReplicaDescriptor ( const ReplicaDescriptor& );
	/* @NoMarshall */ ReplicaDescriptor ( const ReplicaDescriptor* );
	/* @NoMarshall */ ReplicaDescriptor ( const GroupData* );
	/* @NoMarshall */ virtual ~ReplicaDescriptor ( );
	/* @NoMarshall */ Boolean pack ( Buffer& , int  = -1) const ;
	/* @NoMarshall */ Boolean unpack ( Buffer& , int  = -1);
	/* @NoMarshall */ ReplicaDescriptor& operator = ( const ReplicaDescriptor& );
	/* @NoMarshall */ ReplicaDescriptor& operator = ( const GroupData& );
	/* @NoMarshall */ ReplicaDescriptor& operator += ( const GroupData& );
	/* @NoMarshall */ ReplicaDescriptor& operator += ( const ReplicaDescriptor& );
	/* @NoMarshall */ Boolean operator == ( const ReplicaDescriptor& );
	/* @NoMarshall */ Boolean operator != ( const ReplicaDescriptor& );
	/* @NoMarshall */ void getGroupList ( const Uid& , ReplicaDescriptor*& , Boolean  = false) const ;
	/* @NoMarshall */ virtual ostream& print ( ostream& ) const ;
	ReplicaDescriptor* next;
private:
	/* @NoMarshall */ ReplicaDescriptor* getTemplate ( const Uid& , Boolean ) const ;
	/* @NoMarshall */ void getReplicaView ( const Uid& , ReplicaDescriptor*& ) const ;
	/* @NoMarshall */ ReplicaDescriptor* getOwnTemplate ( const Uid& , Boolean ) const ;
	/* @NoMarshall */ void makeViewFromTemplate ( const Uid& , ReplicaDescriptor*& , Boolean  = false) const ;
	const ReplicaDescriptor* groupPtr;
public:
	/* @NoMarshall */ void getExcludedList ( const Uid& , ReplicaDescriptor*& ) const ;
	/* @NoMarshall */ void getExcludedList ( ReplicaDescriptor*& ) const ;
	/* @NoMarshall */ void getAllMembers ( const Uid& , ReplicaDescriptor*& , Boolean  = false) const ;
	/* @NoMarshall */ ReplicaDescriptor* addToList ( const ReplicaDescriptor& );
	/* @NoMarshall */ ReplicaDescriptor* addToList ( const GroupData& );
	/* @NoMarshall */ void getCachedGroupList ( const Uid& , ReplicaDescriptor*& ) const ;
	/* @NoMarshall */ void testAndSetObjectNames ( const TypeName );
	/* @NoMarshall */ void setAllHostNames ( const char* );
	/* @NoMarshall */ void getTemplate ( GroupData*& ) const ;
	/* @NoMarshall */ Boolean templateMember ( const Uid& ) const ;
	/* @NoMarshall */ long getNumber ( ) const ;
	/* @NoMarshall */ void reset ( );
	/* @NoMarshall */ void getNextGroup ( const ReplicaDescriptor*& );
	/* @NoMarshall */ Boolean isMemberOfList ( const Uid& ) const ;
	/* @NoMarshall */ void marshall ( RpcBuffer& ) const ;
	/* @NoMarshall */ void unmarshall ( RpcBuffer& );
};

#ifndef STUB

extern RpcBuffer& operator<< ( RpcBuffer&, const ReplicaDescriptor *);
extern RpcBuffer& operator>> ( RpcBuffer&, ReplicaDescriptor *&);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, const ReplicaDescriptor& topack);
extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ReplicaDescriptor& tounpack);
#endif

extern ostream& operator << ( ostream& , const ReplicaDescriptor& );
extern RpcBuffer& operator << ( RpcBuffer& , ReplicaDescriptor* );
extern RpcBuffer& operator << ( RpcBuffer& , ReplicaDescriptor& );
extern RpcBuffer& operator << ( RpcBuffer& , const ReplicaDescriptor* );
extern RpcBuffer& operator >> ( RpcBuffer& , ReplicaDescriptor*& );
extern RpcBuffer& operator >> ( RpcBuffer& , ReplicaDescriptor& );
extern const ReplicaDescriptor NIL_RD;

#endif  /* REPDESC_STUB_H_ */
