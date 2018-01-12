/*
 * Copyright (C) 2002, 2003
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AxisHeaderContextProcessor.java,v 1.6 2003/04/04 15:00:55 nmcl Exp $
 */

package com.arjuna.mw.wst.service;

import org.apache.axis.AxisFault;
import org.apache.axis.handlers.BasicHandler;
import org.apache.axis.Message;
import org.apache.axis.MessageContext;
import org.apache.axis.message.SOAPEnvelope;
import org.apache.axis.message.SOAPHeaderElement;
import org.apache.axis.transport.http.HTTPConstants;
import org.apache.axis.utils.DOM2Writer;

/**
 * The class is used to perform WS-Transaction context insertion
 * and extraction for application level SOAP messages on the Apache
 * AXIS platform.
 */
public class AxisHeaderContextProcessor extends BasicHandler
{ 
    public void invoke(MessageContext context) throws AxisFault
    {
        if (context.getPastPivot()) 
        {
            /*
             * This is a response.  Insert the context into the header
             * block of the outgoing SOAP message.
             */
            Message msg = context.getResponseMessage();
            if (msg == null)
                return;

            /*
             * Check to see if we have WS-Transaction context.
             */
            try 
            {
                // Get the current context from the appropriate WSTX location?
                // org.w3c.dom.Node node = ServiceManagement.disassociateContext().toDOM();
                
                // Try to suspend the current transaction as the outgoing message passes through.

                com.arjuna.mw.wst.TransactionManager transactionManager = com.arjuna.mw.wst.TransactionManagerFactory.transactionManager();
                com.arjuna.mw.wst.TxContext txContext = null;
                // If we're not in a transaction, then just return
                if(transactionManager == null)
                {
                    return;
                }
                else // Otherwise suspend the transaction (and don't put a header on the wire).
                {
                   txContext = transactionManager.suspend();
                }
            }
            catch (Exception e) 
            {
                // TODO: Some logging here
                System.err.println("Error in: com.arjuna.mw.wstx.service.AxisHeaderContextProcessor.invoke(MessageContext context)");
                System.err.println("Stack trace: ");
                e.printStackTrace(System.err);
            }
        } 
        else 
        {
            /* This is a request.  Extract the context from the header
             * block of the SOAP message, and store it in the service
             * infrastructure where it is programmtically accessible to
             * back-end transactional infrastructure.
             *
             * We don't actually throw any exceptions, just in case we
             * have been invoked on a different or null header.
             */
            Message msg = context.getRequestMessage();
            if (msg == null)
            {
                return;
            }

            SOAPEnvelope env = msg.getSOAPEnvelope();
            // Get the WS-Transaction context from the header
            SOAPHeaderElement header = env.getHeaderByName(_HEADER_BLOCK_URI, _HEADER_BLOCK_NAME);


            if (header != null)
            {
                try 
                {
                    SOAPHeaderElement she = env.getHeaderByName(_HEADER_BLOCK_URI, _HEADER_BLOCK_NAME);

                    if(she != null)
                    {
                        com.arjuna.wsc.messaging.CoordinationContext cc = com.arjuna.wsc.messaging.saaj.CoordinationContextHelper.fromXML(env, she);
                    
                        if(cc != null)
                        {
                            com.arjuna.mw.wst.TxContext txContext = (com.arjuna.mw.wst.TxContext)new com.arjuna.mwlabs.wst.at.context.TxContextImple(cc);
                            if(txContext != null)
                            {
                                com.arjuna.mw.wst.TransactionManagerFactory.transactionManager().resume(txContext);
                            }
                        }
                    }


                    //                    java.util.Iterator it = header.getChildElements();
                    //                    while(it.hasNext())
                    //                    {
                    //                        SOAPHeaderElement ele = (SOAPHeaderElement)it.next();
                    //                        if(ele.getElementName().getLocalName().toLowerCase().equals(_HEADER_BLOCK_NAME.toLowerCase())
                    //                            && ele.getNamespaceURI().equals(_HEADER_BLOCK_URI))
                    //                        {
                    //                            java.util.Iterator childIterator = ele.getChildElements();
                    //                            while(childIterator.hasNext())
                    //                            {
                    //                                
                    //                            }
                    //                        }
                    //                    }

                    // See whether the header contains AT or BA contents (as opposed to some other WS-Coordination protocol contents)
                    //                    org.w3c.dom.Element ctxHeader = header.getAsDOM();
                    //                    org.w3c.dom.NodeList nl1 = ctxHeader.getElementsByTagName(_WST_AT_URI);
                    //                    org.w3c.dom.NodeList nl2 = ctxHeader.getElementsByTagName(_WST_BA_URI);
                    //                    
                    //                    // If they're both null, then there's nothing to do
                    //                    if(nl1 == null && nl2 == null)
                    //                    {
                    //                        return;
                    //                    }
                    //
                    //                    // If we have a non-null list, but it doesn't contain extactly one context, then some weirdness has happened.
                    //                    if(nl1 != null && nl1.getLength() != 1)
                    //                    {
                    //                        return;
                    //                    }
                    //                    
                    //                    // If we have a non-null list, but it doesn't contain extactly one context, then some weirdness has happened.
                    //                    if(nl2 != null && nl2.getLength() != 1)
                    //                    {
                    //                        return;
                    //                    }
                    //                    
                    //                    // OK, the header contains something that we're interested in, create a context and resume
                    //                    com.arjuna.mw.wst.TxContext txContext = (com.arjuna.mw.wst.TxContext)new com.arjuna.mwlabs.wst.at.ContextImple(ctxHeader);
                    //
                    //                    if(txContext != null)
                    //                    {
                    //                        com.arjuna.mw.wst.TransactionManagerFactory.transactionManager().resume(txContext);
                    //                    }
                }
                catch (Exception e) 
                {
                    // TODO: Some logging here
                    System.err.println("Error in: com.arjuna.mw.wstx.service.AxisHeaderContextProcessor.invoke(MessageContext context)");
                    System.err.println("Stack trace: ");
                    e.printStackTrace(System.err);
                }
            }
        }
    }

    private static final String _HEADER_BLOCK_URI = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";
    private static final String _HEADER_BLOCK_NAME = "CoordinationContext";
    private static final String _COORDINATION_TYPE_ELEMENT = "CoordinationType";
    private static final String _WST_AT_URI = "http://schemas.xmlsoap.org/ws/2002/08/wstx";
    private static final String _WST_BA_URI = "http://schemas.xmlsoap.org/ws/2002/08/wsba";
}
