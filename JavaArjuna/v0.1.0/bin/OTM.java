/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id
 */

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaArjunaLite.Common.*;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaGandiva.Common.*;
import com.sun.java.swing.*;
import com.sun.java.swing.tree.*;
import com.sun.java.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import java.util.Enumeration;
import java.net.InetAddress;

import java.io.IOException;
import java.net.UnknownHostException;

/*
 * Currently only looks at this machine.
 */

class DirEntry
{

public DirEntry ()
    {
	node = null;
	fullPathName = null;
    }

public DirEntry (DefaultMutableTreeNode n, String s)
    {
	node = n;
	fullPathName = s;
    }
    
public DefaultMutableTreeNode node;
public String fullPathName;
    
};

public class OTM extends JSplitPane
{

public OTM ()
    {
        super(HORIZONTAL_SPLIT);

	String localHost = null;
	
	try
	{
	    InetAddress myAddress = InetAddress.getLocalHost();
	    localHost = myAddress.getHostName();
	}
	catch (UnknownHostException e)
	{
	    localHost = "LocalHostUnknown";
	}

	_machines.addElement(localHost);
	localHost = null;
	
        // Create the nodes.
	
        topMachine = new DefaultMutableTreeNode("Currently registered machines.");
        topTran = new DefaultMutableTreeNode("Currently running transactions.");	

	createNodes(topMachine);
	
        // Create a tree that allows one selection at a time.
	
        tree = new JTree(topMachine);
	transactions = new JTree(topTran);
	
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);

        // Listen for when the selection changes.
	
	MouseListener ml = new MouseAdapter() {
             public void mouseClicked(MouseEvent e) {
		 int selRow = tree.getRowForLocation(e.getX(), e.getY());
		 TreePath selPath = tree.getPathForLocation(e.getX(), e.getY());
		 
		 if (selRow != -1) {
		     if (e.getClickCount() == 2) {
			 DefaultMutableTreeNode node = (DefaultMutableTreeNode) selPath.getPathComponent(selRow);

			 if (node.isLeaf())
			 {
			     DefaultMutableTreeNode parent = (DefaultMutableTreeNode) node.getParent();
		    
			     getTransactions((String) parent.getUserObject(), topTran);

			     parent = null;
			 }
			 else
			     modifyTransactionView();
		     }
		 }
	     }
	};
	
	tree.addMouseListener(ml);
 
        // Create the scroll pane and add the tree to it.
	
        JScrollPane treeView = new JScrollPane(tree);
	JScrollPane tranView = new JScrollPane(transactions);
	    
        // Add the scroll panes to this panel.
	
        add(treeView);
	add(tranView);

        Dimension minimumSize = new Dimension(100, 100);

        treeView.setMinimumSize(minimumSize);
	tranView.setMinimumSize(minimumSize);
	
	/*
	 * setDividerLocation(300); //XXX: ignored! bug 4101306
	 * workaround for bug 4101306:
	 */
	
        treeView.setPreferredSize(new Dimension(100, 100)); 

        setPreferredSize(new Dimension(500, 300));
    }

private void createNodes (DefaultMutableTreeNode topMachine)
    {
	int number = _machines.size();
        DefaultMutableTreeNode machine = null;
	
	if (number == 0)
	{
	    topMachine.add(new DefaultMutableTreeNode("No machines registered."));
	}
	else
	{
	    String machineName = null;
	    
	    for (int i = 0; i < number; i++)
	    {
		machineName = (String) _machines.elementAt(i);
		machine = new DefaultMutableTreeNode(machineName);

		System.out.println("adding 'Double-click' to "+machineName);
		
		machine.add(new DefaultMutableTreeNode("Double-click to monitor."));
		topMachine.add(machine);

		System.out.println("adding to root "+(String) topMachine.getUserObject());
	    }
	}
    }

private void modifyTransactionView ()
    {
	if (transactions.isCollapsed(1))
	    transactions.expandRow(1);
	else
	    transactions.collapseRow(1);
    }
    
private void getTransactions (String machineName, DefaultMutableTreeNode topTran)
    {
	int count = transactions.getRowCount();

	for (int i = count; i > 0; i--)
	{
	    transactions.collapseRow(i);
	    transactions.removeSelectionRow(i);
	}

	topTran.removeAllChildren();
	transactions.update(transactions.getGraphics());
	_dirs = new Vector();
	
	String root = null;
	String storeImple = null;
	DefaultMutableTreeNode top = new DefaultMutableTreeNode("Transactions on: "+machineName);
	
	try
	{
	    ClassName actionStoreType = new ClassName(System.getProperty(JavaArjunaLiteNames.BasicAction_actionStore(), JavaArjunaLiteNames.Implementation_ObjectStore_defaultActionStore().stringForm()));

	    ObjectStore imple = new ObjectStore(actionStoreType, root);
	    InputObjectState types = new InputObjectState();

	    if (imple.allTypes(types))
	    {
		String fullPathName = null;

		try
		{
		    boolean endOfList = false;
		    DefaultMutableTreeNode currentNode = null;
		    DefaultMutableTreeNode currentRoot = top;

		    topTran.add(currentRoot);
		    
		    while (!endOfList)
		    {
			fullPathName = types.unpackString();

			if (fullPathName.compareTo("") == 0)
			    endOfList = true;
			else
			{
			    InputObjectState uids = new InputObjectState();
			    String nodeName = stripName(fullPathName);

			    currentNode = new DefaultMutableTreeNode(nodeName);
			    _dirs.addElement(new DirEntry(currentNode, fullPathName));

			    currentRoot = findRoot(top, currentNode);
			    currentRoot.add(currentNode);

			    if (imple.allObjUids(fullPathName, uids))
			    {
				Uid theUid = new Uid(Uid.nullUid());
				
				try
				{
				    boolean endOfUids = false;
				    
				    while (!endOfUids)
				    {
					theUid.unpack(uids);

					if (theUid.equals(Uid.nullUid()))
					    endOfUids = true;
					else
					{
					    DefaultMutableTreeNode tranID = new DefaultMutableTreeNode(theUid.stringForm());

					    tranID.add(new DefaultMutableTreeNode(new String("status: "+imple.currentState(theUid, fullPathName))));

					    currentNode.add(tranID);
					}
				    }
				}
				catch (Exception e)
				{
				    // end of uids!
				}
			    }
			}
		    }
		}
		catch (Exception e)
		{
		    // end of list!
		}
	    }
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}

	DefaultTreeModel model = (DefaultTreeModel) transactions.getModel();

	model.reload();
	
	transactions.update(transactions.getGraphics());
    }
    
private DefaultMutableTreeNode findRoot (DefaultMutableTreeNode top, DefaultMutableTreeNode curr)
    {
	DefaultMutableTreeNode root = top;
	
	if (curr != null)
	{
	    int number = _dirs.size();
	    String name = fullNodeName(curr);

	    if (number != 0)
	    {
		for (int i = 0; i < number; i++)
		{
		    DirEntry dirEntry = (DirEntry) _dirs.elementAt(i);
		    
		    if (dirEntry != null)
		    {
			DefaultMutableTreeNode node = dirEntry.node;
			String dirName = dirEntry.fullPathName;
			
			if ((name.indexOf(dirName) != -1) && (node != curr))
			{
			    root = node;
			}
		    }
		}
	    }
	}
	
	return root;
    }

private String fullNodeName (DefaultMutableTreeNode curr)
    {
	String root = "StateManager";

	if (curr != null)
	{
	    int number = _dirs.size();

	    if (number != 0)
	    {
		for (int i = 0; i < number; i++)
		{
		    DirEntry dirEntry = (DirEntry) _dirs.elementAt(i);

		    if (dirEntry != null)
		    {
			if (dirEntry.node == curr)
			{
			    return dirEntry.fullPathName;
			}
		    }
		}

		root = (String) curr.getUserObject();
	    }
	}

	return root;
    }
    
private String stripName (String name)
    {
	String root = null;
	
	if (name != null)
	{
	    int number = _dirs.size();

	    if (number != 0)
	    {
		for (int i = 0; i < number; i++)
		{
		    DirEntry dirEntry = (DirEntry) _dirs.elementAt(i);

		    if (dirEntry != null)
		    {
			DefaultMutableTreeNode node = dirEntry.node;
			String dirName = dirEntry.fullPathName;
			int subString = name.indexOf(dirName);

			if ((subString != -1) && (name.compareTo(dirName) != 0))
			{
			    root = name.substring(subString+dirName.length()+1);
			}
		    }
		}
	    }

	    if (root == null)
		root = name;
	}
	else
	    root = "StateManager";
	
	return root;
    }    

private void printChildren (DefaultMutableTreeNode currentNode)
    {
	if (currentNode != null)
	{
	    Enumeration children = currentNode.children();

	    if (children != null)
	    {
		String name = (String) currentNode.getUserObject();

		System.out.println("Node: "+name);
		
		while (children.hasMoreElements())
		{
		    DefaultMutableTreeNode node = (DefaultMutableTreeNode) children.nextElement();

		    name = (String) node.getUserObject();

		    System.out.println("\tChild: "+name);
		}
	    }
	}
    }
    
public static void main(String[] args)
    {
        /*
         * Create a window. Use JFrame since this window will include 
         * lightweight components.
         */
	
        JFrame frame = new JFrame("OTM Transaction Monitor");

        WindowListener l = new WindowAdapter() {
            public void windowClosing(WindowEvent e) {System.exit(0);}
        };
	
        frame.addWindowListener(l);

        frame.getContentPane().add("Center", new OTM());
        frame.pack();
        frame.show();
    }

private Vector _machines = new Vector();
private Vector _dirs = new Vector();
private static DefaultMutableTreeNode topMachine;
private static DefaultMutableTreeNode topTran;
private static JTree tree;
private static JTree transactions;

};
