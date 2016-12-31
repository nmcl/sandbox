/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.3.2.1.2.1 2000/05/04 15:47:55 nmcl Exp $
 */














package com.arjuna.JTSArjuna;

import com.arjuna.JavaHarness.ModuleInfo;
import java.io.*;
import java.util.Hashtable;

public class Info implements ModuleInfo
{

public Info ()
    {
	_attributes = new Hashtable();
	
	_attributes.put("PRODUCT", "JTSArjuna" );
	_attributes.put("MAJOR VERSION", "0" );
	_attributes.put("MINOR VERSION", "0" );
	_attributes.put("CANDIDATE RELEASE", "none" );
	_attributes.put("RELEASE", "internal" );
	_attributes.put("MODULE", "JTSArjuna" );
	_attributes.put("SOURCE IDENTIFIER", "unknown" );
	_attributes.put("BUILD_INFORMATION", "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put("VERSION", "2" );
	_attributes.put("DATE", "Tue Aug 1 17:11:51 BST 2000" );
	_attributes.put("NOTES", "none" );

	_constituents = new ModuleInfo[7];
	
	_constituents[0] = new com.arjuna.JavaHarness.Info("\t");
	_constituents[1] = new com.arjuna.ArjunaCommon.Info("\t");
	_constituents[2] = new com.arjuna.ArjunaLite.Info("\t");
	_constituents[3] = new com.arjuna.OrbCommon.Info("\t");
	_constituents[4] = new com.arjuna.CosTransactions.Info("\t");
	_constituents[5] = new com.arjuna.JDBC.Info("\t");
	_constituents[6] = new com.arjuna.JDBC2.Info("\t");
    }

public final void setPreString (String s)
    {
	_preString = s;
    }

public final void getInfo ()
    {
	System.out.println(toString());
    }

public String query (String attribute)
    {
	return (String) _attributes.get(attribute);
    }

public ModuleInfo[] constituents ()
    {
	return null;
    }

public String toString ()
    {
	String product = "PRODUCT: "+ "JTSArjuna" ;
	String version = "VERSION: "+ "2" ;
	String major = "MAJOR VERSION: "+ "0" ;
	String minor = "MINOR VERSION: "+ "0" ;
	String candidate;
	
	if ("none"  == "none")
	    candidate = null;
	else
	    candidate = "CANDIDATE RELEASE: "+ "none" ;

	String release = "RELEASE: "+ "internal" ;
	String module = "MODULE: "+ "JTSArjuna" ;
	String source = "SOURCE IDENTIFIER: "+ "unknown" ;
	String build = "BUILD INFORMATION: "+ "Arjuna Solutions Ltd. [unknown]" ;
	String date = "DATE: "+ "Tue Aug 1 17:11:51 BST 2000" ;
	String notes = "NOTES: "+ "none" ;
	String constituents = "CONSTITUENT MODULE INFORMATION:";
	String myInfo;
	
	if (candidate != null)
	    myInfo = product+"\n"+version+"\n"+major+"\n"+minor+"\n"+candidate+"\n"+release+"\n"+module+"\n"+source+"\n"+build+"\n"+date+"\n"+notes+"\n"+constituents;
	else
	    myInfo = product+"\n"+version+"\n"+major+"\n"+minor+"\n"+release+"\n"+module+"\n"+source+"\n"+build+"\n"+date+"\n"+notes+"\n"+constituents;

	String toReturn = myInfo;
	
	for (int i = 0; i < 7; i++)
	{
	    toReturn = toReturn + "\n\n" + _constituents[i].toString();
	}

	return toReturn;
    }

private Hashtable _attributes;
private ModuleInfo[] _constituents;
private String _preString = "";
    
};


