/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_krc.h,v 1.2.4.1.2.4.26.1.2.2.4.2.50.1.40.1 2001/12/11 13:37:21 nmcl Exp $
 */

#ifndef COMMON_ORBPORTABILITY_STDC_H_
#define COMMON_ORBPORTABILITY_STDC_H_

#ifdef CORBA_2_4
#  define POA_(name) org.omg.PortableServer./**/name
#  define NAMESPACE_(p) org.omg./**/p
#  define HELPER_CLASS_(c) c/**/Helper
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/Helper
#  define NARROW_(type,obj) type/**/Helper.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/Helper.narrow(/**/obj/**/)
#  define SKEL_(name) name/**/POA
#  define SCOPED_SKEL_(scope,name) scope.name/**/POA
#  define READY_NAMED_IMPL_(obj,name) OAInterface.objectIsReady(obj,name)
#  define READY_IMPL_(obj) OAInterface.objectIsReady(obj)
#  define DESTROY_IMPL_(obj) OAInterface.shutdownObject(obj)
#  define ENUM_VALUE_(v) v/**/.value()
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef CORBA_2_3
#  define POA_(name) org.omg.PortableServer./**/name
#  define NAMESPACE_(p) org.omg./**/p
#  define HELPER_CLASS_(c) c/**/Helper
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/Helper
#  define NARROW_(type,obj) type/**/Helper.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/Helper.narrow(/**/obj/**/)
#ifndef DAISJ2
#  define SKEL_(name) name/**/POA
#  define SCOPED_SKEL_(scope,name) scope.name/**/POA
#else
#  define SKEL_(name) POA_/**/name
#  define SCOPED_SKEL_(scope,name) POA_/**/scope.name
#endif
#  define READY_NAMED_IMPL_(obj,name) OAInterface.objectIsReady(obj,name)
#  define READY_IMPL_(obj) OAInterface.objectIsReady(obj)
#  define DESTROY_IMPL_(obj) OAInterface.shutdownObject(obj)
#  define ENUM_VALUE_(v) v/**/.value()
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef CORBA_2_2
#  define NAMESPACE_(p) org.omg./**/p
#  define HELPER_CLASS_(c) c/**/Helper
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/Helper
#  define NARROW_(type,obj) type/**/Helper.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/Helper.narrow(/**/obj/**/)
#  define SKEL_(name) _/**/name/**/ImplBase
#  define SCOPED_SKEL_(scope,name) scope./**/_/**/name/**/ImplBase
#  define READY_NAMED_IMPL_(obj,name) OAInterface.objectIsReady(obj)
#  define READY_IMPL_(obj) OAInterface.objectIsReady(obj)
#  define DESTROY_IMPL_(obj) OAInterface.shutdownObject(obj)
#  define ENUM_VALUE_(v) v/**/.value()
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef CORBA_2_1
#  define NAMESPACE_(p) org.omg./**/p
#  define HELPER_CLASS_(c) c/**/Helper
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/Helper
#  define NARROW_(type,obj) type/**/Helper.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/Helper.narrow(/**/obj/**/)
#  define SKEL_(name) _/**/name/**/ImplBase
#  define SCOPED_SKEL_(scope,name) scope./**/_/**/name/**/ImplBase
#  define READY_NAMED_IMPL_(obj,name) OAInterface.boa().obj_is_ready(obj)
#  define READY_IMPL_(obj) OAInterface.objectIsReady(obj)
#  define DESTROY_IMPL_(obj) OAInterface.shutdownObject(obj)
#  define ENUM_VALUE_(v) v/**/.value()
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef CORBA_2_0
#  define NAMESPACE_(p) p
#  define HELPER_CLASS_(c) c/**/_var
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/_var
#  define NARROW_(type,obj) type/**/_var.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/_var.narrow(/**/obj/**/)
#  define SKEL_(name) _sk_/**/name
#  define SCOPED_SKEL_(scope,name) scope./**/_sk_/**/name
#  define READY_NAMED_IMPL_(obj,name) OAInterface.boa().obj_is_ready(obj)
#  define READY_IMPL_(obj) OAInterface.objectIsReady(obj)
#  define DESTROY_IMPL_(obj) OAInterface.shutdownObject(obj)
#  define ENUM_VALUE_(v) v
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef HPORB
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#  define TIE_INSTANCE_(o) o/**/POATie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/.o/**/POATie
#  define CORBA_BIND_(intf,poaname,name) intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,poaname,name) scope/**/./**/intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#endif

#ifdef SUNSJDK
#ifdef JDK_1_2
#  define TIE_CLASS_(name) _/**/name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/_/**/name/**/Operations
#  define TIE_INSTANCE_(o) _/**/o/**/Tie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._/**/o/**/Tie
#else
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#  define TIE_INSTANCE_(o) o/**/_Tie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/.o/**/_Tie
#endif
#  define CORBA_BIND_(intf,name) intf/**/Helper.bind(/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/Helper.bind(/**/name/**/)
#endif

#ifdef VISIBROKER
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#ifndef HAVE_POA
#  define TIE_INSTANCE_(o) _tie_/**/o
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._tie_/**/o
#ifdef VB_VERSION_2_5
#  define CORBA_BIND_(intf,name) intf/**/_var.bind(/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/_var.bind(/**/name/**/)
#else
#  define CORBA_BIND_(intf,name) intf/**/Helper.bind(ORBInterface.orb(),/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/Helper.bind(ORBInterface.orb(),/**/name/**/)
#endif
#else
#  define TIE_INSTANCE_(o) o/**/POATie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/.o/**/POATie
#  define SCOPED_CORBA_BIND_(scope,intf,poaname,name) scope/**/./**/intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#endif
#endif

#ifdef ORBIX2000
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#  define TIE_INSTANCE_(o) o/**/POATie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/.o/**/POATie
#  define CORBA_BIND_(intf,poaname,name) intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,poaname,name) scope/**/./**/intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#endif

#ifdef ORBACUS
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope/**/./**/name/**/Operations
#ifdef HAVE_POA
#  define TIE_INSTANCE_(o) o/**/POATie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/.o/**/POATie
#else
#  define TIE_INSTANCE_(o) _/**/o/**/ImplBase_tie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._/**/o/**/ImplBase_tie
#endif
#endif

#ifdef ORBIXWEB
#  define TIE_CLASS_(name) _/**/name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/_/**/name/**/Operations
#  define TIE_INSTANCE_(o) _tie_/**/o
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._tie_/**/o
#ifndef NO_BIND
#  define CORBA_BIND_(intf,name) intf/**/Helper.bind(/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/Helper.bind(/**/name/**/)
#endif
#endif

#ifdef DAISJ2
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#  define TIE_INSTANCE_(o) POA_/**/o/**/_tie
#  define SCOPED_TIE_INSTANCE_(scope,o) POA_/**/scope/**/./**/o/**/_tie
#endif

#ifdef JBROKER
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#ifdef HAVE_POA
#  define TIE_INSTANCE_(o) o/**/POATie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/./**/o/**/POATie
#else
#  define TIE_INSTANCE_(o) _/**/o/**/Tie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._/**/o/**/Tie
#endif
#  define CORBA_BIND_(intf,name) intf/**/Helper.bind(/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/Helper.bind(/**/name/**/)
#endif

#ifdef JACORB
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#  define TIE_INSTANCE_(o) o/**/POATie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/.o/**/POATie
#  define CORBA_BIND_(intf,poaname,name) intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,poaname,name) scope/**/./**/intf/**/Helper.bind(ORBInterface.orb(),/**/poaname/**/,/**/name/**/)
#endif

#ifdef HAVE_POA
#  define CORBA_REF_(obj,intf) intf/**/Helper.narrow(OAInterface.corbaReference(obj))
#else
#  define CORBA_REF_(obj,intf) obj
#endif

#ifndef CORBA_BIND_
#  define CORBA_BIND_(intf,name) (/**/intf/**/) ORBObject.bindToObject(/**/name/**/)
#  define SCOPED__CORBA_BIND_(scope,intf,name) (/**/scope/**/./**/intf/**/) ORBObject.bindToObject(/**/name/**/)
#endif

#endif
