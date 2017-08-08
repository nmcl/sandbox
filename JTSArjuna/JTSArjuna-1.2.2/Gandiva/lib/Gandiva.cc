/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Gandiva.cc,v 1.9 1998/01/12 13:05:06 nmcl Exp $
 */

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

static const ClassName *gandivaBaseActionClassName = NULL;

const ClassName& Gandiva::Base::Action::name()
{
    if (gandivaBaseActionClassName == NULL)
        gandivaBaseActionClassName = new ClassName("ActionBase");

    return *gandivaBaseActionClassName;
}

static const ClassName *gandivaCommonThreadClassName = NULL;

const ClassName& Gandiva::Common::Thread::name()
{
    if (gandivaCommonThreadClassName == NULL)
        gandivaCommonThreadClassName = new ClassName("Thread");

    return *gandivaCommonThreadClassName;
}

static const ClassName *gandivaCommonThreadPoolClassName = NULL;

const ClassName& Gandiva::Common::ThreadPool::name()
{
    if (gandivaCommonThreadPoolClassName == NULL)
        gandivaCommonThreadPoolClassName = new ClassName("ThreadPool");

    return *gandivaCommonThreadPoolClassName;
}

static const ClassName *gandivaControlThreadControlClassName = NULL;

const ClassName& Gandiva::Control::ThreadControl::name()
{
    if (gandivaControlThreadControlClassName == NULL)
        gandivaControlThreadControlClassName = new ClassName("ThreadControl");

    return *gandivaControlThreadControlClassName;
}

static const ClassName *gandivaInterfaceBufferClassName = NULL;

const ClassName& Gandiva::Interface::Buffer::name()
{
    if (gandivaInterfaceBufferClassName == NULL)
        gandivaInterfaceBufferClassName = new ClassName("Buffer");

    return *gandivaInterfaceBufferClassName;
}

static const ClassName *gandivaInterfaceInventoryClassName = NULL;

const ClassName& Gandiva::Interface::Inventory::name()
{
    if (gandivaInterfaceInventoryClassName == NULL)
        gandivaInterfaceInventoryClassName = new ClassName("Inventory");

    return *gandivaInterfaceInventoryClassName;
}

static const ClassName *gandivaInterfaceNameServiceClassName = NULL;

const ClassName& Gandiva::Interface::NameService::name()
{
    if (gandivaInterfaceNameServiceClassName == NULL)
        gandivaInterfaceNameServiceClassName = new ClassName("NameService");

    return *gandivaInterfaceNameServiceClassName;
}

static const ClassName *gandivaImplementationBufferClassName = NULL;

const ClassName& Gandiva::Implementation::Buffer::name()
{
    if (gandivaImplementationBufferClassName == NULL)
        gandivaImplementationBufferClassName = new ClassName("BufferImple");

    return *gandivaImplementationBufferClassName;
}

static const ClassName *gandivaImplementationBufferQANBOClassName = NULL;

const ClassName& Gandiva::Implementation::Buffer::QANBO::name()
{
    if (gandivaImplementationBufferQANBOClassName == NULL)
        gandivaImplementationBufferQANBOClassName = new ClassName("QANBOBufferImple");

    return *gandivaImplementationBufferQANBOClassName;
}

static const ClassName *gandivaImplementationInventoryClassName = NULL;

const ClassName& Gandiva::Implementation::Inventory::name()
{
    if (gandivaImplementationInventoryClassName == NULL)
        gandivaImplementationInventoryClassName = new ClassName("InventoryImple");

    return *gandivaImplementationInventoryClassName;
}

static const ClassName *gandivaImplementationInventoryCreateFunctionListClassName = NULL;

const ClassName& Gandiva::Implementation::Inventory::CreateFunctionList::name()
{
    if (gandivaImplementationInventoryCreateFunctionListClassName == NULL)
        gandivaImplementationInventoryCreateFunctionListClassName = new ClassName("CreateFunctionListInventoryImple");

    return *gandivaImplementationInventoryCreateFunctionListClassName;
}

static const ClassName *gandivaImplementationInventoryGandivaDedicatedClassName = NULL;

const ClassName& Gandiva::Implementation::Inventory::GandivaDedicated::name()
{
    if (gandivaImplementationInventoryGandivaDedicatedClassName == NULL)
        gandivaImplementationInventoryGandivaDedicatedClassName = new ClassName("GandivaDedicatedInventoryImple");

    return *gandivaImplementationInventoryGandivaDedicatedClassName;
}

static const ClassName *gandivaImplementationNameServiceClassName = NULL;

const ClassName& Gandiva::Implementation::NameService::name()
{
    if (gandivaImplementationNameServiceClassName == NULL)
        gandivaImplementationNameServiceClassName = new ClassName("NameServiceImple");

    return *gandivaImplementationNameServiceClassName;
}

static const ClassName *gandivaImplementationNameServiceINSClassName = NULL;

const ClassName& Gandiva::Implementation::NameService::INS::name()
{
    if (gandivaImplementationNameServiceINSClassName == NULL)
        gandivaImplementationNameServiceINSClassName = new ClassName("INSNameServiceImple");

    return *gandivaImplementationNameServiceINSClassName;
}

static const ClassName *gandivaImplementationNameServiceLNSClassName = NULL;

const ClassName& Gandiva::Implementation::NameService::LNS::name()
{
    if (gandivaImplementationNameServiceLNSClassName == NULL)
        gandivaImplementationNameServiceLNSClassName = new ClassName("LNSNameServiceImple");

    return *gandivaImplementationNameServiceLNSClassName;
}

static const ClassName *gandivaImplementationNameServiceSNSClassName = NULL;

const ClassName& Gandiva::Implementation::NameService::SNS::name()
{
    if (gandivaImplementationNameServiceSNSClassName == NULL)
        gandivaImplementationNameServiceSNSClassName = new ClassName("SNSNameServiceImple");

    return *gandivaImplementationNameServiceSNSClassName;
}

static const ClassName *gandivaImplementationNameServiceRNSClassName = NULL;

const ClassName& Gandiva::Implementation::NameService::RNS::name()
{
    if (gandivaImplementationNameServiceRNSClassName == NULL)
        gandivaImplementationNameServiceRNSClassName = new ClassName("RNSNameServiceImple");

    return *gandivaImplementationNameServiceRNSClassName;
}