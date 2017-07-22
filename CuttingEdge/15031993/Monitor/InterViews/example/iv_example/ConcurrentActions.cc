#include <locale.h>

#include <iostream.h>

#include <IV-look/kit.h>
#include <InterViews/layout.h>
#include <InterViews/brush.h>
#include <InterViews/canvas.h>
#include <InterViews/color.h>
#include <InterViews/brush.h>

#include "ConcurrentActions.h"

ConcurrentActions::ConcurrentActions()
{
    LayoutKit& layout = *LayoutKit::instance();
    WidgetKit& widget = *WidgetKit::instance();

    vbox = layout.vbox();

    body(layout.hbox(
            layout.hglue(5.0),
            vbox,
            layout.hglue(5.0)
        )
    );

    _brush = new Brush(2.0);
    Resource::ref(_brush);
}

ConcurrentActions::~ConcurrentActions()
{
    Resource::unref(_brush);
    body(NULL);
}

int ConcurrentActions::AddAbove(ActionStructure* newAS)
{
    return DoAddAbove(this, newAS);
}

int ConcurrentActions::AddBelow(ActionStructure* newAS)
{
    return DoAddBelow(this, newAS);
}

int ConcurrentActions::DoAddBefore(ActionStructure*, ActionStructure*)
{
    return 0;
}

int ConcurrentActions::DoAddAfter(ActionStructure*, ActionStructure*)
{
    return 0;
}

int ConcurrentActions::DoAddAbove(ActionStructure*, ActionStructure* newAS)
{
    vbox->append(newAS);
    vbox->change(vbox->count() - 1);
    newAS->setParent(this);

    Change();

    return 1;
}

int ConcurrentActions::DoAddBelow(ActionStructure*, ActionStructure* newAS)
{
    vbox->prepend(newAS);
    vbox->change(0);
    newAS->setParent(this);

    Change();

    return 1;
}

int ConcurrentActions::DoRemove(ActionStructure*)
{
    return 0;
}

int ConcurrentActions::DoReplace(ActionStructure* oldAS, ActionStructure* newAS)
{
    GlyphIndex glyphIndex = 0;

    while (glyphIndex < vbox->count())
    {
	if (vbox->component(glyphIndex) == oldAS)
	{
	    cout << "ConcurrentActions::DoReplace() replacing " << (void*) this << '\n';
	    vbox->replace(glyphIndex, newAS);
	    vbox->change(glyphIndex);

	    newAS->setParent(this);

	    glyphIndex = vbox->count();
	}
	else
	    glyphIndex++;
    }

    Change();

    return 0;
}


void ConcurrentActions::Change()
{
    cout << "ConcurrentActions::DoChange() " << (void*) this << '\n';

    change(1);

    if (parentStructure != NULL)
	parentStructure->DoChange(this);

    redraw();
}

void ConcurrentActions::DoChange(ActionStructure* as)
{
    cout << "ConcurrentActions::DoChange(" << (void*) as << ") " << (void*) this << '\n';

    GlyphIndex glyphIndex = 0;

    while (glyphIndex < vbox->count())
    {
	if (vbox->component(glyphIndex) == as)
	{
	    vbox->change(glyphIndex);
	    cout << "ConcurrentActions::DoChange() changing " << (void*) as << '\n';
	    cout.flush();
	    glyphIndex = vbox->count();
	}
	else
	    glyphIndex++;
    }

    Change();
}

void ConcurrentActions::draw(Canvas* c, const Allocation& a) const
{
    body()->draw(c, a);

    Allotment allotCont_X;
    Allotment allotCont_Y;

    Coord y = ((a.top() - a.bottom()) / 2.0) + a.bottom();

    Coord xs1 = a.left();
    Coord xe2 = a.right();

    for (int i = 0; i < vbox->count(); i++)
    {
	vbox->allotment(i, Dimension_X, allotCont_X);
	vbox->allotment(i, Dimension_Y, allotCont_Y);
    
	Coord yc  = ((allotCont_Y.end() - allotCont_Y.begin()) / 2.0) + allotCont_Y.begin();

	Coord xe1 = allotCont_X.begin();
	Coord xs2 = allotCont_X.end();

	c->new_path();
	c->move_to (xs1, y);
	c->line_to(xe1, yc);
	c->close_path();
	c->stroke(WidgetKit::instance()->foreground(), _brush);

	c->new_path();
	c->move_to (xs2, yc);
	c->line_to(xe2, y);
	c->close_path();
	c->stroke(WidgetKit::instance()->foreground(), _brush);
    }
}
