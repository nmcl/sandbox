#ifndef NODE_H_
#include "Node.h"
#endif

#ifndef DEPLIST_H_
#include "DepList.h"
#endif

#ifndef COMMON_H_
#include "Common.h"
#endif

#ifndef MAINSELECT_H_
#include "MainSelect.h"
#endif

#include <iostream.h>
#include <sys/param.h>
#include <string.h>

#include <InterViews/canvas.h>
#include <InterViews/color.h>
#include <InterViews/font.h>
#include <InterViews/patch.h>
#include <InterViews/session.h>
#include <InterViews/event.h>
#include <InterViews/background.h>
#include <InterViews/layout.h>
#include <InterViews/bitmap.h>
#include <InterViews/stencil.h>
#include <InterViews/scrbox.h>
#include <IV-look/kit.h>


extern int numberOfNodes;
extern ScrollBox* scrBox;
extern Patch* patch;


Node::Node (const char* nodeName, NodeData* data) : InputHandler(0, Session::instance()->style())
{
    DependencyLevel temp = NONE;

    body(0);
    listWindow = 0;
    myData = data;

    hostName = new char[strlen(nodeName)];
    ::memset(hostName, '\0', strlen(nodeName)+1);
    ::memcpy(hostName, nodeName, strlen(nodeName));
    
    if (data)
	data->getMostDependent(temp);
    
#ifdef DEBUG
    cout << "name: " << nodeName << endl;

    if (myData)
	cout << *myData << endl;
    else
	cout << "No dependency" << endl;
#endif    
    
    depWindow = 0;
    displayMethod = TRUE;
    displayNode(temp, displayMethod);
}

Node::Node (const char* nodeName, NodeData* data, DependencyLevel temp) : InputHandler(0, Session::instance()->style())
{
    depWindow = 0;
    listWindow = 0;
    myData = data;
    body(0);

    hostName = new char[strlen(nodeName)];
    ::memset(hostName, '\0', strlen(nodeName)+1);
    ::memcpy(hostName, nodeName, strlen(nodeName));

    displayMethod = FALSE;
    displayNode(temp, displayMethod);
}

Node::~Node ()
{
    if (hostName)
	delete hostName;
    if (myData)
    {
	delete myData;
	myData = 0;
    }
    if (depWindow)
    {
	depWindow->unmap();
	delete depWindow;
    }
    if (listWindow)
	Resource::unref(listWindow);
    body(0);
}

char* Node::getNodeName () const
{
    return hostName;
}

void Node::changeNodeName (const char* nodeName)
{
    DependencyLevel temp = NONE;
    
    if (hostName)
	delete hostName;
    
    hostName = new char[strlen(nodeName)+1];
    ::memset(hostName, '\0', strlen(nodeName)+1);
    ::memcpy(hostName, nodeName, strlen(nodeName));

    if (myData)
	myData->getMostDependent(temp);

    displayNode(temp, displayMethod);
    patch->reallocate();
    patch->redraw();
}

void Node::displayNode (DependencyLevel temp, Boolean opt)
{
    LayoutKit& layout = *LayoutKit::instance();

    if (opt)
    {
	PolyGlyph *bmLayout = layout.vbox(layout.vglue(2.5),
					  NodeData::getBitmap(temp),
					  layout.vglue(2.5)
					  );
    
	contents = new Label(hostName, WidgetKit::instance()->font(), NodeData::getColor(temp));
	
	vbox = layout.vbox(layout.vglue(2.5),
			   contents,
			   layout.vglue(2.5)
			   );

	if (!body())
	{
	    body(layout.hbox(bmLayout,
			     layout.hglue(2.5),
			     vbox,
			     layout.hglue(2.5)
			     )
		 );
	}
	else
	{
	    replace(1, layout.hbox(bmLayout,
				   layout.hglue(2.5),
				   vbox,
				   layout.hglue(2.5)
				   )
		    );
	}
    }
    else
    {
	PolyGlyph *bmLayout = layout.vbox(layout.vspace(2.5),
					  NodeData::getBitmap(temp)
					  );

	contents = new Label(hostName, WidgetKit::instance()->font(), NodeData::getColor(temp));

    
	vbox = layout.vbox(layout.vspace(2.5),
			   layout.hbox(layout.vbox(layout.vspace(2.5),
						   contents
						   ),
				       layout.hglue(2.5),
				       bmLayout
				       )
			   );
    
	if (!body())
	    body(vbox);
	else
	    replace(1, vbox);
    }
}

void Node::draw (Canvas* c, const Allocation& a) const
{
    body()->draw(c, a);
}

void Node::press (const Event& e)
{
    Glyph* hbox = 0;
    
    focus(this);

    if (e.pointer_button() == Event::right)
    {
	if (depWindow)
	{
	    depWindow->unmap();
	    delete depWindow;
	    depWindow = 0;
	    Resource::unref(listWindow);
	    listWindow = 0;

	    if (!buttonCheck::shouldDelete)
		return;
	}

	if (buttonCheck::shouldDelete)
	{
	    Boolean found = FALSE;
	    
	    for (int k = 0; (k < numberOfNodes) && (!found); k++)
	    {
		if (scrBox->component(k) == this)
		{
		    found = TRUE;
		    break;
		}
	    }
	    
	    if (found)
		Delete::toRemove = k;
	    else
		cout << "Error. Could not find node to delete!" << endl;

	    return;
	}
    }

    if (e.pointer_button() == Event::middle)
    {
	if (buttonCheck::modifyName)
	    changeNodeName(buttonCheck::modifyName);

	if (Modify::myWindow)
	{
	    Modify::myWindow->unmap();
	    delete Modify::myWindow;
	    Modify::myWindow = 0;
	}	

	buttonCheck::inUse = FALSE;
    }

    if (e.pointer_button() == Event::left)
    {
	if (depWindow)
	{
	    depWindow->unmap();
	    delete depWindow;
	    depWindow = 0;
	    Resource::unref(listWindow);
	    listWindow = 0;

	    if (!buttonCheck::shouldDelete)
		return;
	}

	NodeData *ptr = myData;
	LayoutKit& layout = *LayoutKit::instance();
	listWindow = LayoutKit::instance()->vscrollbox(myData->numberOfDeps());
	Resource::ref(listWindow);
	
	while (ptr)
	{
	    DepList* d = new DepList(myData, ptr->hostName);
	    ptr = ptr->next;
	    listWindow->append(d);
	}

	hbox = LayoutKit::instance()->hbox(listWindow, WidgetKit::instance()->vscroll_bar(listWindow));
	Patch *p = new Patch(hbox);
	
	depWindow = new ApplicationWindow(new Background(p,
							 WidgetKit::instance()->background()
							 )
					  );
	depWindow->map();
    }
}

