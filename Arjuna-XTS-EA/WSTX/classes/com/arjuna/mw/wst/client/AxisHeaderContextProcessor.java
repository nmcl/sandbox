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

package com.arjuna.mw.wst.client;

import com.arjuna.mwlabs.wsc.common.Schema;

import com.arjuna.wsc.messaging.CoordinationContext;
import com.arjuna.wsc.messaging.saaj.CoordinationContextHelper;

import javax.xml.soap.SOAPHeader;
import javax.xml.soap.SOAPHeaderElement;
import javax.xml.soap.Name;

//import javax.servlet.http.HttpServletRequest;

import org.apache.axis.AxisFault;
import org.apache.axis.handlers.BasicHandler;
import org.apache.axis.Message;
import org.apache.axis.MessageContext;
import org.apache.axis.message.SOAPEnvelope;
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
        // Note the "!" operator - this is the reciprocal of the service side
        if(!context.getPastPivot())
        {
            /*
             * This is an outgoing request.  Insert the context into the header
             * block of the outgoing SOAP message.
             */
            Message msg = context.getRequestMessage();
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
                else
                {
                    txContext = transactionManager.currentTransaction();
                }

                if(txContext != null)
                {
                    com.arjuna.mwlabs.wst.at.context.TxContextImple txContextImple = (com.arjuna.mwlabs.wst.at.context.TxContextImple)txContext;

                    SOAPEnvelope env = msg.getSOAPEnvelope();
        		    SOAPHeader header = env.getHeader();
                    if(header == null) {
                        env.addHeader();
                        header = env.getHeader();
                    }
                    Name name = env.createName(com.arjuna.wsc.impl.messaging.XMLUtil.COORDINATION_CONTEXT_TAG, com.arjuna.wsc.impl.messaging.XMLUtil.WSCOOR_PREFIX, com.arjuna.wsc.impl.messaging.XMLUtil.WSCOOR_NAMESPACE_URI);
                    SOAPHeaderElement headerElement = header.addHeaderElement(name);
		    CoordinationContextHelper.toXML(env, headerElement, txContextImple.context().getCoordinationContext());
                }
            }
            catch (Exception e)
            {
                // TODO: Some logging here
                System.err.println("Error in: com.arjuna.mw.wst.client.AxisHeaderContextProcessor.invoke(MessageContext context)");
                System.err.println("Stack trace: ");
                e.printStackTrace(System.err);
            }
        }
        else
        {
            /* This is an incoming response.  Extract the context from the header
             * block of the SOAP message, and store it in the service
             * infrastructure where it is programmtically accessible to
             * back-end transactional infrastructure.
             *
             * We don't actually throw any exceptions, just in case we
             * have been invoked on a different or null header.
             */
            Message msg = context.getResponseMessage();
            if (msg == null)
            {
                return;
            }

            SOAPEnvelope env = msg.getSOAPEnvelope();
            // Get the WS-Transaction context from the header
            org.apache.axis.message.SOAPHeaderElement header = env.getHeaderByName(_HEADER_BLOCK_URI, _HEADER_BLOCK_NAME);


            if (header != null)
                try
                {
                    // See whether the header contains AT or BA contents (as opposed to some other WS-Coordination protocol contents)
                    org.w3c.dom.Element ctxHeader = header.getAsDOM();
                    org.w3c.dom.NodeList nl1 = ctxHeader.getElementsByTagName(_WST_AT_URI);
                    org.w3c.dom.NodeList nl2 = ctxHeader.getElementsByTagName(_WST_BA_URI);

                    // If they're both null, then there's nothing to do
                    if(nl1 == null && nl2 == null)
                    {
                        return;
                    }

                    // If we have a non-null list, but it doesn't contain extactly one context, then some weirdness has happened.
                    if(nl1 != null && nl1.getLength() != 1)
                    {
                        return;
                    }

                    // If we have a non-null list, but it doesn't contain extactly one context, then some weirdness has happened.
                    if(nl2 != null && nl2.getLength() != 1)
                    {
                        return;
                    }

                    // OK, the header contains something that we're interested in, create a context and resume

		    CoordinationContext ctx = CoordinationContextHelper.fromXML(env, header);
                    com.arjuna.mw.wst.TxContext txContext = (com.arjuna.mw.wst.TxContext)new com.arjuna.mwlabs.wst.at.ContextImple(ctx);

                    if(txContext != null)
                    {
                        com.arjuna.mw.wst.TransactionManagerFactory.transactionManager().resume(txContext);
                    }
                }
                catch (Exception e)
                {
                    // TODO: Some logging here
                    System.err.println("Error in: com.arjuna.mw.wstx.client.AxisHeaderContextProcessor.invoke(MessageContext context)");
                    System.err.println("Stack trace: ");
                    e.printStackTrace(System.err);
                }
        }
    }

    private static final String _HEADER_BLOCK_URI = "http://schemas.xmlsoap.org/ws/2002/08/wscoor";
    private static final String _HEADER_BLOCK_NAME = "CoordinationContext";
    private static final String _WST_AT_URI = "http://schemas.xmlsoap.org/ws/2002/08/wstx";
    private static final String _WST_BA_URI = "http://schemas.xmlsoap.org/ws/2002/08/wsba";
}
