#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/label.h>
#include <InterViews/box.h>
#include <InterViews/background.h>
#include <InterViews/glue.h>
#include <IV-look/button.h>
#include <IV-look/kit.h>

#include "Area.h"
#include "ColourAction.h"
#include "ActiveColourEdit.h"

ActiveColourEdit::ActiveColourEdit(ActiveColour* ac)
{
    int     res;
    char*   name;
    ColorIntensity r, g, b;

    activeColour = ac;

    res = 0;
    ac->getName(res, name);
    if (res == 0)
	cerr << "Unable to get ""Name"" of ""ActiveColour""\n";

    res = 0;
    ac->getColour(res, r, g, b);
    if (res == 0)
	cerr << "Unable to get ""Colour"" of ""ActiveColour""\n";
    else
    {
	redVal   = new ColourValue(this, r);
	greenVal = new ColourValue(this, g);
	blueVal  = new ColourValue(this, b);

	Button*     moverRedUp     = Kit::instance()->up_mover(redVal, Session::instance()->style());
	Glyph*      scrollBarRed   = Kit::instance()->vscroll_bar(redVal, Session::instance()->style());
	Button*     moverRedDown   = Kit::instance()->down_mover(redVal, Session::instance()->style());
	Button*     moverGreenUp   = Kit::instance()->up_mover(greenVal, Session::instance()->style());
	Glyph*      scrollBarGreen = Kit::instance()->vscroll_bar(greenVal, Session::instance()->style());
	Button*     moverGreenDown = Kit::instance()->down_mover(greenVal, Session::instance()->style());
	Button*     moverBlueUp    = Kit::instance()->up_mover(blueVal, Session::instance()->style());
	Glyph*      scrollBarBlue  = Kit::instance()->vscroll_bar(blueVal, Session::instance()->style());
	Button*     moverBlueDown  = Kit::instance()->down_mover(blueVal, Session::instance()->style());

	Button*     reloadButton   = Kit::instance()->simple_push_button("Reload", Session::instance()->style(),
                                                                         new ColourAction(this, Reload_env));
	Button*     saveButton     = Kit::instance()->simple_push_button("Save", Session::instance()->style(),
                                                                         new ColourAction(this, Save_env));
	Button*     quitButton     = Kit::instance()->simple_push_button("Quit", Session::instance()->style(),
                                                                         new ColourAction(this, Quit_env));
        

	Label*      label          = new Label(name, Session::instance()->style()->font(), Session::instance()->style()->foreground());
	Background* bg             = new Background(nil, new Color(r, g, b));
        colourArea                 = new Area(bg, 120, 120);

	LRBox*      box            = new LRBox(new TBBox(label, colourArea, new LRBox(reloadButton, new HGlue, saveButton, new HGlue, quitButton)),
                                               new TBBox(moverRedUp, scrollBarRed, moverRedDown),
                                               new TBBox(moverGreenUp, scrollBarGreen, moverGreenDown),
                                               new TBBox(moverBlueUp, scrollBarBlue, moverBlueDown));
	body(box);
    }
}

ActiveColourEdit::~ActiveColourEdit()
{
}

void ActiveColourEdit::Update()
{
    ColorIntensity r, g, b;

    r = redVal->get_value();
    g = greenVal->get_value();
    b = blueVal->get_value();

    colourArea->body(new Background(nil, new Color(r, g, b)));

    colourArea->redraw();
}

void ActiveColourEdit::Reload()
{
    ColorIntensity r, g, b;
    int res;

    activeColour->getColour(res, r, g, b);

    if (res == 1)
    {
	redVal->set_value(r);
	greenVal->set_value(g);
        blueVal->set_value(b);
    }
}

void ActiveColourEdit::Save()
{
    ColorIntensity r, g, b;
    int res;

    r = redVal->get_value();
    g = greenVal->get_value();
    b = blueVal->get_value();

    activeColour->setColour(res, r, g, b);
}

void ActiveColourEdit::Quit()
{
    Session::instance()->quit();
}

