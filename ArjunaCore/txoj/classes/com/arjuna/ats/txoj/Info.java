/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import java.util.Hashtable;

/**
 * Module specific implementation of the interface object.
 *
 * @author Richard Begg (richard_begg@hp.com)
 * @version $Id: Info.javatmpl,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 * @since HPTS 3.0
 */
public class Info
{
    public String toString()
    {
        StringBuffer moduleInfo = new StringBuffer();

        moduleInfo.append("<module-info name=\"txoj\">");
        moduleInfo.append("<source-identifier>unknown</source-identifier>");
        moduleInfo.append("<build-information>Arjuna Technologies [mcl] (Windows 2000 5.1)</build-information>");
        moduleInfo.append("<version>unknown</version>");
        moduleInfo.append("<date>2003/Jun/26 11:05</date>");
        moduleInfo.append("<notes></notes>");
        moduleInfo.append("<configuration>");
	moduleInfo.append("<lock-store-root>"+com.arjuna.ats.txoj.common.Configuration.lockStoreRoot()+"</lock-store-root>");
        moduleInfo.append("</configuration>");
        moduleInfo.append("</module-info>");

        return(moduleInfo.toString());
    }
}
