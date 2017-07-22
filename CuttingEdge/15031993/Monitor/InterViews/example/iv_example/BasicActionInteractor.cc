#include <iostream.h>

#include <InterViews/style.h>
#include <InterViews/session.h>
#include <InterViews/dialog.h>
#include <InterViews/input.h>
#include <InterViews/event.h>

#include "Main.h"
#include "BasicActionDialog.h"
#include "BasicActionInteractor.h"

BasicActionInteractor::BasicActionInteractor()
{
}

BasicActionInteractor::~BasicActionInteractor()
{
}

void BasicActionInteractor::press(const Event& e)
{
    int res = 0;
        
    focus(this);
    if (! e.control_is_down())
    {
	if (e.pointer_button() == Event::left)
	    res = ActionStructure::AddBefore(new BasicActionInteractor);
	else if (e.pointer_button() == Event::right)
	    res = ActionStructure::AddAfter(new BasicActionInteractor);
	else if (e.pointer_button() == Event::middle)
	{
	    if (e.shift_is_down())
		res = ActionStructure::AddAbove(new BasicActionInteractor);
	    else
		res = ActionStructure::AddBelow(new BasicActionInteractor);
	}
	else
	    cout << "BasicActionInteractor::press(const Event&): no button\n";
    }
    else
    {
	Coord val;
	
	if (e.pointer_button() == Event::left)
	{
	    val = getWidth();

	    val += 10.0;

	    if (val > 260.0)
		val = 260.0;
	    
	    setWidth(val);
	}

	if (e.pointer_button() == Event::right)
	{
	    val = getWidth();

	    val -= 10.0;
	    if (val < 40.0)
		val = 40.0;
	    
	    setWidth(val);
	}
    }

    Change();
    
    patch->reallocate();
    patch->redraw();
    
    cout << "BasicActionInteractor::press(const Event&) res = " << res << '\n';
    cout.flush();
}

void BasicActionInteractor::keystroke(const Event&)
{
    cout << "BasicActionInteractor::keystroke(const Event&)\n";
    cout.flush();

    Glyph*  glyph  = new BasicActionDialog;
    Style*  style  = Session::instance()->style();
    Dialog* dialog = new Dialog(glyph, style);
    
    dialog->post_at(250.0, 250.0);
    dialog->run();
}
