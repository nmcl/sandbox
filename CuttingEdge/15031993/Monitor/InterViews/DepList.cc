#ifndef DEPLIST_H_
#include "DepList.h"
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
#include <IV-look/kit.h>


DepList::DepList (NodeData* data, const char* name) : InputHandler(0, Session::instance()->style())
{
    LayoutKit& layout = *LayoutKit::instance();
    DependencyLevel level = NONE;

    aw = 0;
    tw = 0;
    
    ::memset(hostName, '\0', MAXHOSTNAMELEN);
    ::memcpy(hostName, name, strlen(name));

    groupData = data;
    level = data->getDependency(hostName);
    
    PolyGlyph *bmLayout = layout.vbox(layout.vglue(2.5),
				      layout.hbox(NodeData::getBitmap(level),
						  layout.hglue(2.5)
						  ),
				      layout.vglue(2.5)
				      );
          
    contents = new Label(hostName, WidgetKit::instance()->font(), NodeData::getColor(level));
    
    vbox = layout.vbox(layout.vglue(2.5),
		       contents,
		       layout.vglue(2.5)
		       );
    
    body(layout.hbox(bmLayout,
		     layout.hglue(10.0),
		     vbox,
		     layout.hglue(25.0)
		     )
	 );
}

DepList::~DepList ()
{
    if (aw)
    {
	aw->unmap();
	delete aw;
    }
    
    if (tw)
    {
	tw->unmap();
	delete tw;
    }
    
    body(0);
}

void DepList::draw (Canvas* c, const Allocation& a) const
{
    body()->draw(c, a);
}

void DepList::press (const Event& e)
{
    focus(this);

    if (e.pointer_button() == Event::middle)
    {
	if (tw)
	{
	    tw->unmap();
	    delete tw;
	    tw = 0;
	}
	else
	{
	    DependencyLevel temp = groupData->getDependency(hostName);

	    tw = new ApplicationWindow(new Background(new Label(NodeData::getString(temp),
								WidgetKit::instance()->font(),
								NodeData::getColor(temp)
								),
						      WidgetKit::instance()->background()
						      )
				       );
	    tw->map();
	}
    }
}
