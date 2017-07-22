#include <locale.h>

#include <iostream.h>

#include <IV-look/kit.h>
#include <InterViews/layout.h>
#include <InterViews/brush.h>
#include <InterViews/canvas.h>
#include <InterViews/color.h>
#include <InterViews/brush.h>

#include "BasicActionFigure.h"
#include "BasicAction.h"

BasicAction::BasicAction()
{
    LayoutKit& layout = *LayoutKit::instance();

    contents = new BasicActionFigure("Apple", 40.0, 60.0);

    vbox = layout.vbox(
	       layout.vglue(2.5),
               contents,
               layout.vglue(2.5)
           );

    body(layout.hbox(
	     layout.hglue(5.0),
	     vbox,
	     layout.hglue(5.0)
         )
    );

    _brush = new Brush(2.0);
    Resource::ref(_brush);
}

BasicAction::~BasicAction()
{
    Resource::unref(_brush);
    body(NULL);
}

int BasicAction::DoAddBefore(ActionStructure*, ActionStructure*)
{
    return 0;
}

int BasicAction::DoAddAfter(ActionStructure*, ActionStructure*)
{
    return 0;
}

int BasicAction::DoAddAbove(ActionStructure*, ActionStructure*)
{
    return 0;
}

int BasicAction::DoAddBelow(ActionStructure*, ActionStructure*)
{
    return 0;
}

int BasicAction::DoRemove(ActionStructure*)
{
    return 0;
}

int BasicAction::DoReplace(ActionStructure*, ActionStructure*)
{
    return 0;
}

void BasicAction::Change()
{
    cout << "BasicAction::Change() " << (void*) this << '\n';

    vbox->change(1);
    body()->change(1);

    if (parentStructure != NULL)
	parentStructure->DoChange(this);

    redraw();
}

void BasicAction::DoChange(ActionStructure* as)
{
    cout << "BasicActions::DoChange(" << (void*) as << ") " << (void*) this << '\n';

    Change();
}

void BasicAction::draw(Canvas* c, const Allocation& a) const
{
    body()->draw(c, a);

    Allotment allotCont;

    vbox->allotment(1, Dimension_X, allotCont);
    
    Coord y = ((a.top() - a.bottom()) / 2.0) + a.bottom();

    Coord xs1 = a.left();
    Coord xe1 = allotCont.begin();
    Coord xs2 = allotCont.end();
    Coord xe2 = a.right();

    c->new_path();
    c->move_to (xs1, y);
    c->line_to(xe1, y);
    c->close_path();
    c->stroke(WidgetKit::instance()->foreground(), _brush);

    c->new_path();
    c->move_to (xs2, y);
    c->line_to(xe2, y);
    c->close_path();
    c->stroke(WidgetKit::instance()->foreground(), _brush);
}

void BasicAction::setHight(Coord hight)
{
    contents->setHight(hight);
}

void BasicAction::setWidth(Coord width)
{
    contents->setWidth(width);
}

Coord BasicAction::getHight()
{
    return contents->getHight();
}

Coord BasicAction::getWidth()
{
    return contents->getWidth();
}
