#include <locale.h>

#include <iostream.h>

#include <IV-look/kit.h>
#include <InterViews/layout.h>
#include <InterViews/brush.h>
#include <InterViews/canvas.h>
#include <InterViews/color.h>
#include <InterViews/brush.h>

#include "SerialActions.h"

SerialActions::SerialActions()
{
    LayoutKit& layout = *LayoutKit::instance();
    WidgetKit& widget = *WidgetKit::instance();

    hbox = layout.hbox();
    body(layout.hbox(
             layout.hglue(5.0),
             hbox,
             layout.hglue(5.0)
        )
    );

    _brush = new Brush(2.0);
    Resource::ref(_brush);
}

SerialActions::~SerialActions()
{
    Resource::unref(_brush);
    body(NULL);
}

int SerialActions::AddBefore(ActionStructure* newAS)
{
    return DoAddBefore(this, newAS);
}

int SerialActions::DoAddBefore(ActionStructure*, ActionStructure* newAS)
{
    hbox->append(newAS);
    newAS->setParent(this);

    Change();

    return 1;
}

int SerialActions::AddAfter(ActionStructure* newAS)
{
    return DoAddAfter(this, newAS);
}

int SerialActions::DoAddAfter(ActionStructure*, ActionStructure* newAS)
{
    hbox->prepend(newAS);
    newAS->setParent(this);

    Change();

    return 1;
}

int SerialActions::DoAddAbove(ActionStructure*, ActionStructure*)
{
    return 0;
}

int SerialActions::DoAddBelow(ActionStructure*, ActionStructure*)
{
    return 0;
}

int SerialActions::DoRemove(ActionStructure*)
{
    return 0;
}

int SerialActions::DoReplace(ActionStructure* oldAS, ActionStructure* newAS)
{
    GlyphIndex glyphIndex = 0;

    while (glyphIndex < hbox->count())
    {
	if (hbox->component(glyphIndex) == oldAS)
	{
	    cout << "SerialActions::DoReplace() replacing " << (void*) this << '\n';
	    hbox->replace(glyphIndex, newAS);
	    newAS->setParent(this);

	    glyphIndex = hbox->count();
	}
	else
	    glyphIndex++;
    }

    Change();

    return 0;
}

void SerialActions::Change()
{
    cout << "SerialActions::DoChange() " << (void*) this << '\n';

    change(1);

    if (parentStructure != NULL)
	parentStructure->DoChange(this);

    redraw();
}

void SerialActions::DoChange(ActionStructure* as)
{
    cout << "SerialActions::DoChange(" << (void*) as << ") " << (void*) this << '\n';
    GlyphIndex glyphIndex = 0;
    
    while (glyphIndex < hbox->count())
    {
	if (hbox->component(glyphIndex) == as)
	{
	    hbox->change(glyphIndex);
	    cout << "SerialActions::DoChange() changing " << (void*) as << '\n';
	    cout.flush();
	    glyphIndex = hbox->count();
	}
	else
	    glyphIndex++;
    }

    Change();
}

void SerialActions::draw(Canvas* c, const Allocation& a) const
{
    body()->draw(c, a);

    Coord y = ((a.top() - a.bottom()) / 2.0) + a.bottom();

    Allotment allotStart;

    hbox->allotment(0, Dimension_X, allotStart);
    
    Coord xs1 = a.left();
    Coord xe1 = allotStart.begin();

    c->new_path();
    c->move_to (xs1, y);
    c->line_to(xe1, y);
    c->close_path();
    c->stroke(WidgetKit::instance()->foreground(), _brush);

    Allotment allotEnd;
    hbox->allotment(hbox->count() - 1, Dimension_X, allotEnd);

    Coord xs2 = allotEnd.end();
    Coord xe2 = a.right();

    c->new_path();
    c->move_to (xs2, y);
    c->line_to(xe2, y);
    c->close_path();
    c->stroke(WidgetKit::instance()->foreground(), _brush);
}
