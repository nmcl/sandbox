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

#ifndef STUB
#include <StubGen/RpcBuffer.h>
#include <StubGen/ClientRpcMan.h>


#ifndef NAMESERVER_SERVER
#undef NameServer
#define NameServer RemoteNameServer
#endif

class RemoteNameServer: public RemoteLockManager
{
public:
	RemoteNameServer ( int& , ClientRpcManager *_cRpcMan_ = ClientRpcManager::createInstance("NameServer",1));
	RemoteNameServer ( const Uid& , int& , ClientRpcManager *_cRpcMan_ = ClientRpcManager::createInstance("NameServer",1));
	~RemoteNameServer ( );
	Boolean getView ( ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	Boolean lockGroup ( const Uid& , const char* , pid_t , LockMode , int& , unsigned long& );
	Boolean getUseCount ( const Uid& , long& , int& , unsigned long& );
	Boolean traverseDatabase ( ReplicaDescriptor& , Uid& , int& , const char* , pid_t , int& , unsigned long& );
	OutCome addReplicas ( const ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome deleteReplicas ( const Uid& , const ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& , const Boolean&  = false);
	OutCome alterLocation ( const Uid& , const Uid& , const char* , const char* , pid_t , int& , unsigned long& );
	OutCome exclude ( const ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome include ( const ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome releaseGroup ( const Uid& , const char* , pid_t , int& , unsigned long& );
	Boolean groupAssociation ( const Uid& , Uid& , int& , unsigned long& );
	OutCome recover ( const char* , int& , unsigned long& );
	Boolean testAndSet ( ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome needsUpdating ( const Uid& , Boolean& , ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& );
	OutCome getNodeUpdateList ( ReplicaDescriptor& , ReplicaDescriptor& , const char* , int& , unsigned long& );
	Boolean getGroupViewState ( ObjectState& , int& , unsigned long& );
	Boolean imposeNewGroupViewState ( ObjectState& , int& );
	Boolean getReplicaState ( const Uid& , ObjectState& , int& , unsigned long& );
	Boolean imposeReplicaState ( const Uid& , ObjectState& , int& , unsigned long& );
	OutCome addTemplate ( const Uid& , Buffer& , const char* , pid_t , int& , unsigned long& );
	Boolean getTemplate ( const Uid& , Buffer& , LockMode , int& , unsigned long& );
	OutCome deleteTemplate ( const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome replaceTemplate ( const Uid& , Buffer& , const char* , pid_t , int& , unsigned long& );
	OutCome assignCompositeObject ( const Uid& , const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome assignCompositeObject ( const Uid& , const ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& );
	OutCome removeCompositeObject ( const Uid& , const Uid& , const char* , pid_t , int& , unsigned long& );
	OutCome removeCompositeObject ( const Uid& , const ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& );
	void storeName ( Boolean& , Buffer , Buffer , int& , unsigned long& );
	void fetchName ( Boolean& , Buffer , Buffer& , int& , unsigned long& );
	void removeName ( Boolean& , Buffer , int& , unsigned long& );
	void replaceName ( Boolean& , Buffer , Buffer , int& , unsigned long& );
	void getNextName ( Boolean& , Buffer& , int& , int& , unsigned long& );
	void reset ( int& );
	virtual Boolean save_state ( ObjectState& , ObjectType );
	virtual Boolean restore_state ( ObjectState& , ObjectType );
	virtual const TypeName type ( ) const ;
protected:
	RemoteNameServer(const ClientRpcManager&, const RpcBuffer&);
private:
	ClientRpcManager _clientManager;
	RpcBuffer _myHashVal;
}
;


#ifdef NAMESERVER_SERVER
#undef NameServer
#define NameServer LocalNameServer
#endif

class LocalNameServer;

class ServerNameServer: public ServerLockManager
{

public:
	ServerNameServer ();
	~ServerNameServer ();

	void serve (int, char **);
	Int32 dispatchToClass (LocalNameServer*, Int32, RpcBuffer&, RpcBuffer&);

private:
	Int32 dispatchToOper (LocalNameServer *,Int32, RpcBuffer&, RpcBuffer&);

	Int32 NameServer200632073(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 NameServer238333945(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 NameServer177161750(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getView189166844(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 lockGroup81205148(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getUseCount130535164(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 traverseDatabase119504620(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 addReplicas222535708(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 deleteReplicas232059006(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 alterLocation175794844(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 exclude15081196(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 include15073868(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 releaseGroup109857964(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 groupAssociation75444940(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 recover42677660(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 testAndSet262775036(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 needsUpdating82735980(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getNodeUpdateList131495436(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getGroupViewState230028300(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 imposeNewGroupViewState49971337(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getReplicaState150067804(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 imposeReplicaState229743500(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 addTemplate57558620(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getTemplate150841692(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 deleteTemplate54505916(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 replaceTemplate52922652(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 assignCompositeObject176394412(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 assignCompositeObject57978364(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 removeCompositeObject55701260(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 removeCompositeObject17515596(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 storeName139252988(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 fetchName32268428(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 removeName234338428(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 replaceName14100364(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 getNextName264496828(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 reset20145801(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 save_state173681173(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 restore_state165059637(LocalNameServer *, RpcBuffer&, RpcBuffer&);
	Int32 type152344342(LocalNameServer *, RpcBuffer&, RpcBuffer&);

	LocalNameServer *theRealObject;
	RpcBuffer _constructArgs_;
	Int32 _constructOpcode_;
	Int32 _constructRefCount_;
	RpcBuffer _constructResult_;
};
#endif


#ifdef STUB
#  define LocalNameServer NameServer
#endif

/* @Remote, @NoMarshall */ class LocalNameServer: public LocalLockManager
{
public:
	/* @NoMarshall */ LocalNameServer ( int& );
	/* @NoMarshall */ LocalNameServer ( const Uid& , int& );
	/* @NoMarshall */ ~LocalNameServer ( );
	/* @NoMarshall */ Boolean getView ( ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ Boolean lockGroup ( const Uid& , const char* , pid_t , LockMode , int& , unsigned long& );
	/* @NoMarshall */ Boolean getUseCount ( const Uid& , long& , int& , unsigned long& );
	/* @NoMarshall */ Boolean traverseDatabase ( ReplicaDescriptor& , Uid& , int& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome addReplicas ( const ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome deleteReplicas ( const Uid& , const ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& , const Boolean&  = false);
	/* @NoMarshall */ OutCome alterLocation ( const Uid& , const Uid& , const char* , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome exclude ( const ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome include ( const ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome releaseGroup ( const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ Boolean groupAssociation ( const Uid& , Uid& , int& , unsigned long& );
	/* @NoMarshall */ OutCome recover ( const char* , int& , unsigned long& );
	/* @NoMarshall */ Boolean testAndSet ( ReplicaDescriptor& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome needsUpdating ( const Uid& , Boolean& , ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome getNodeUpdateList ( ReplicaDescriptor& , ReplicaDescriptor& , const char* , int& , unsigned long& );
	/* @NoMarshall */ Boolean getGroupViewState ( ObjectState& , int& , unsigned long& );
	/* @NoMarshall */ Boolean imposeNewGroupViewState ( ObjectState& , int& );
	/* @NoMarshall */ Boolean getReplicaState ( const Uid& , ObjectState& , int& , unsigned long& );
	/* @NoMarshall */ Boolean imposeReplicaState ( const Uid& , ObjectState& , int& , unsigned long& );
	/* @NoMarshall */ OutCome addTemplate ( const Uid& , Buffer& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ Boolean getTemplate ( const Uid& , Buffer& , LockMode , int& , unsigned long& );
	/* @NoMarshall */ OutCome deleteTemplate ( const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome replaceTemplate ( const Uid& , Buffer& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome assignCompositeObject ( const Uid& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome assignCompositeObject ( const Uid& , const ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome removeCompositeObject ( const Uid& , const Uid& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ OutCome removeCompositeObject ( const Uid& , const ReplicaDescriptor& , const char* , pid_t , int& , unsigned long& );
	/* @NoMarshall */ void storeName ( Boolean& , Buffer , Buffer , int& , unsigned long& );
	/* @NoMarshall */ void fetchName ( Boolean& , Buffer , Buffer& , int& , unsigned long& );
	/* @NoMarshall */ void removeName ( Boolean& , Buffer , int& , unsigned long& );
	/* @NoMarshall */ void replaceName ( Boolean& , Buffer , Buffer , int& , unsigned long& );
	/* @NoMarshall */ void getNextName ( Boolean& , Buffer& , int& , int& , unsigned long& );
	/* @NoMarshall */ void reset ( int& );
	/* @NoMarshall */ virtual Boolean save_state ( ObjectState& , ObjectType );
	/* @NoMarshall */ virtual Boolean restore_state ( ObjectState& , ObjectType );
	/* @NoMarshall */ virtual const TypeName type ( ) const ;
private:
	GroupView* GV;
	ServerView* SV;
};

