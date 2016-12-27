/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Gandiva.h,v 1.11 1998/01/12 12:53:26 nmcl Exp $
 */

#ifndef GANDIVA_H_
#define GANDIVA_H_

class ClassName;

class Gandiva
{
public:
    class Base
    {
    public:
        class Action
        {
        public:
	    static const ClassName& name();
        };
    };

    class Common
    {
    public:
	class Thread
	{
	public:
	    static const ClassName& name ();
	};
	
	class ThreadPool
	{
	public:
	    static const ClassName& name ();
	};
    };

    class Control
    {
    public:
	class ThreadControl
	{
	public:
	    static const ClassName& name ();
	};
    };
    
    class Interface
    {
    public:
        class Buffer
        {
        public:
	    static const ClassName& name();
        };

        class Inventory
        {
        public:
	    static const ClassName& name();
        };

        class NameService
        {
        public:
	    static const ClassName& name();
        };
    };

    class Implementation
    {
    public:
        class Buffer
        {
        public:
			static const ClassName& name();

	    class QANBO
	    {
	    public:
		static const ClassName& name();
	    };
        };

        class Inventory
        {
        public:
	    static const ClassName& name();

	    class CreateFunctionList
	    {
	    public:
		static const ClassName& name ();
	    };
	    
	    class GandivaDedicated
	    {
	    public:
		static const ClassName& name();
	    };
        };

        class NameService
        {
        public:
	    static const ClassName& name();

	    class INS
	    {
	    public:
		static const ClassName& name();
	    };

	    class LNS
	    {
	    public:
			static const ClassName& name();
	    };

	    class SNS
	    {
	    public:
			static const ClassName& name();
	    };

		class RNS
		{
		public:
			static const ClassName& name ();
		};

        };
    };
};

#endif
