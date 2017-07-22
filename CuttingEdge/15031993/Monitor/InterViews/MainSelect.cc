#ifndef MAINSELECT_H_
#include "MainSelect.h"
#endif

#ifndef SUBSELECT_H_
#include "SubSelect.h"
#endif

#ifndef NODE_H_
#include "Node.h"
#endif

#include <OS/string.h>
#include <InterViews/patch.h>
#include <InterViews/scrbox.h>
#include <InterViews/layout.h>
#include <InterViews/background.h>
#include <InterViews/label.h>
#include <IV-look/button.h>
#include <IV-look/kit.h>
#include <IV-look/field.h>
#include <IV-look/dialogs.h>

#include <iostream.h>
#include <string.h>
#include <memory.h>

extern int numberOfNodes;
extern ScrollBox* scrBox;
extern Patch* patch;


/*
 * Add class
 */

Add::Add ()
{
    mainWindow = 0;
}

Add::~Add ()
{
    if (Add::myWindow)
    {
	Add::myWindow->unmap();
	delete Add::myWindow;
	Add::myWindow = 0;
    }
}

void Add::addWindow (ApplicationWindow* aw)
{
    if (!mainWindow)
	mainWindow = aw;
}

void Add::execute ()
{
    if ((!Add::myWindow) && (!buttonCheck::inUse))
    {
	FieldEditor* fe = DialogKit::instance()->field_editor("Node name: ", WidgetKit::instance()->style());
	addNode* aN = new addNode(fe);
	Cancel* cnl = new Cancel(ADD);
	Menu* m1 = WidgetKit::instance()->menubar();
	Button* b1 = WidgetKit::instance()->push_button("Cancel", cnl);
	Button* b2 = WidgetKit::instance()->push_button("OK", aN);
	m1->append_item(new MenuItem(b1, b1->state(), b1->action()));
	m1->append_item(new MenuItem(b2, b2->state(), b2->action()));
	PolyGlyph* vbox = LayoutKit::instance()->vbox(fe, m1);
	Add::myWindow = new ApplicationWindow(new Background(vbox,
							     WidgetKit::instance()->background()
							     )
					      );
	Add::myWindow->map();
	buttonCheck::inUse = TRUE;
    }
}


/*
 * Delete class
 */

Delete::Delete ()
{
}

Delete::~Delete ()
{
}

void Delete::execute ()
{
    if ((!buttonCheck::shouldDelete) && (!buttonCheck::inUse))
    {
	buttonCheck::shouldDelete = TRUE;
	deleteNode* dN = new deleteNode;
	Cancel* cnl = new Cancel(DELETE);
	Menu* m1 = WidgetKit::instance()->menubar();
	Button* b1 = WidgetKit::instance()->push_button("Cancel", cnl);
	Button* b2 = WidgetKit::instance()->push_button("OK", dN);
	Label* lb1 = new Label("Select node to delete with RIGHT button",
			       WidgetKit::instance()->font(), WidgetKit::instance()->foreground());
	Label* lb2 = new Label("and click OK, or CANCEL",
			       WidgetKit::instance()->font(), WidgetKit::instance()->foreground());
	m1->append_item(new MenuItem(b1, b1->state(), b1->action()));
	m1->append_item(new MenuItem(b2, b2->state(), b2->action()));
	PolyGlyph* vbox = LayoutKit::instance()->vbox(lb1, lb2, m1);
	Delete::myWindow = new ApplicationWindow(new Background(vbox,
								WidgetKit::instance()->background()
								)
						 );
	Delete::myWindow->map();
	buttonCheck::inUse = TRUE;
    }
}


/*
 * Modify class
 */

Modify::Modify ()
{
}

Modify::~Modify ()
{
}

void Modify::execute ()
{
    if ((!Modify::myWindow) && (!buttonCheck::inUse))
    {
	FieldEditor* fe = DialogKit::instance()->field_editor("Change name to: ", WidgetKit::instance()->style());
	modifyNode* mN = new modifyNode(fe);
	Cancel* cnl = new Cancel(MODIFY);
	Menu* m1 = WidgetKit::instance()->menubar();
	Button* b1 = WidgetKit::instance()->push_button("Cancel", cnl);
	Button* b2 = WidgetKit::instance()->push_button("OK", mN);
	m1->append_item(new MenuItem(b1, b1->state(), b1->action()));
	m1->append_item(new MenuItem(b2, b2->state(), b2->action()));
	PolyGlyph* vbox = LayoutKit::instance()->vbox(fe, m1);
	Modify::myWindow = new ApplicationWindow(new Background(vbox,
								WidgetKit::instance()->background()
								)
						 );
	Modify::myWindow->map();
	buttonCheck::inUse = TRUE;
    }
}



/*
 * Guest class
 */

Output::Output ()
{
}

Output::~Output ()
{
}

void Output::execute ()
{
    cout << "output" << endl;
}



/*
 * addNode class
 */

addNode::addNode (FieldEditor* f_editor)
{
    fe = f_editor;
}

addNode::~addNode ()
{
}

void addNode::execute ()
{
    Node* temp = new Node((fe->text())->string(), 0, NONE);
    scrBox->append(temp);
    patch->reallocate();
    patch->redraw();
    Add::myWindow->unmap();
    delete Add::myWindow;
    Add::myWindow = 0;
    numberOfNodes++;
    buttonCheck::inUse = FALSE;
}


/*
 * deleteNode class
 */

deleteNode::deleteNode ()
{
}

deleteNode::~deleteNode ()
{
}

void deleteNode::execute ()
{
    if (Delete::toRemove != -1)
    {
	scrBox->remove(Delete::toRemove);
	patch->reallocate();
	patch->redraw();
	numberOfNodes--;
	Delete::toRemove = -1;
    }

    if (buttonCheck::shouldDelete)
    {
	buttonCheck::shouldDelete = FALSE;
	buttonCheck::inUse = FALSE;
	if (Delete::myWindow)
	{
	    Delete::myWindow->unmap();
	    delete Delete::myWindow;
	    Delete::myWindow = 0;
	}
    }
}



/*
 * modifyNode class
 */

modifyNode::modifyNode (FieldEditor* f_editor)
{
    fe = f_editor;
}

modifyNode::~modifyNode ()
{
}

void modifyNode::execute ()
{
    buttonCheck::modifyName = (fe->text())->string();
}
