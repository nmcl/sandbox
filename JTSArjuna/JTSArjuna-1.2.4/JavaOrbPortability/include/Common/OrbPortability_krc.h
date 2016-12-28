/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_krc.h,v 1.3 1999/09/10 13:49:11 nmcl Exp $
 */

#ifndef COMMON_ORBPORTABILITY_KRC_H_
#define COMMON_ORBPORTABILITY_KRC_H_

#ifdef CORBA_2_2
#  define NAMESPACE_(p) org.omg./**/p
#  define HELPER_CLASS_(c) c/**/Helper
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/Helper
#  define NARROW_(type,obj) type/**/Helper.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/Helper.narrow(/**/obj/**/)
#  define BOA_(name) _/**/name/**/ImplBase
#  define SCOPED_BOA_(scope,name) scope./**/_/**/name/**/ImplBase
#  define READY_IMPL_(obj) ORBInterface.orb().connect(obj)
#  define DESTROY_IMPL_(obj) ORBInterface.orb().disconnect(obj)
#  define ENUM_VALUE_(v) v/**/.value()
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef CORBA_2_1
#  define NAMESPACE_(p) org.omg./**/p
#  define HELPER_CLASS_(c) c/**/Helper
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/Helper
#  define NARROW_(type,obj) type/**/Helper.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/Helper.narrow(/**/obj/**/)
#  define BOA_(name) _/**/name/**/ImplBase
#  define SCOPED_BOA_(scope,name) scope./**/_/**/name/**/ImplBase
#  define READY_IMPL_(obj) ORBInterface.boa().obj_is_ready(obj)
#  define DESTROY_IMPL_(obj) ORBInterface.boa().deactivate_obj(obj)
#  define ENUM_VALUE_(v) v/**/.value()
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef CORBA_2_0
#  define NAMESPACE_(p) p
#  define HELPER_CLASS_(c) c/**/_var
#  define SCOPED_HELPER_CLASS_(scope,c) scope/**/./**/c/**/_var
#  define NARROW_(type,obj) type/**/_var.narrow(/**/obj/**/)
#  define SCOPED_NARROW_(scope,type,obj) scope/**/./**/type/**/_var.narrow(/**/obj/**/)
#  define BOA_(name) _sk_/**/name
#  define SCOPED_BOA_(scope,name) scope./**/_sk_/**/name
#  define READY_IMPL_(obj) ORBInterface.boa().obj_is_ready(obj)
#  define DESTROY_IMPL_(obj) ORBInterface.boa().dispose(obj)
#  define ENUM_VALUE_(v) v
#  define ENUM_CONST_(c,v) c._/**/v
#endif

#ifdef SUNSJDK
#  define TIE_CLASS_(name) _/**/name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/_/**/name/**/Operations
#  define TIE_INSTANCE_(o) _/**/o/**/Tie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._/**/o/**/Tie
#  define CORBA_BIND_(intf,name) intf/**/Helper.bind(/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/Helper.bind(/**/name/**/)
#endif

#ifdef VISIBROKER
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope./**/name/**/Operations
#  define TIE_INSTANCE_(o) _tie_/**/o
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._tie_/**/o
#ifdef VB_2_5
#  define CORBA_BIND_(intf,name) intf/**/_var.bind(/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/_var.bind(/**/name/**/)
#else
#  define CORBA_BIND_(intf,name) intf/**/Helper.bind(ORBInterface.orb(),/**/name/**/)
#  define SCOPED_CORBA_BIND_(scope,intf,name) scope/**/./**/intf/**/Helper.bind(ORBInterface.orb(),/**/name/**/)
#endif
#endif

#ifdef ORBACUS
#  define TIE_CLASS_(name) name/**/Operations
#  define SCOPED_TIE_CLASS_(scope,name) scope/**/./**/name/**/Operations
#  define TIE_INSTANCE_(o) _/**/o/**/ImplBase_tie
#  define SCOPED_TIE_INSTANCE_(scope,o) scope/**/._/**/o/**/ImplBase_tie
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

#ifndef CORBA_BIND_
#  define CORBA_BIND_(intf,name) (/**/intf/**/) ORBObject.bindToObject(/**/name/**/)
#  define SCOPED__CORBA_BIND_(scope,intf,name) (/**/scope/**/./**/intf/**/) ORBObject.bindToObject(/**/name/**/)
#endif

#endif
