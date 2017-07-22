#include <InterViews/background.h>
#include <InterViews/geometry.h>
#include <InterViews/box.h>
#include <InterViews/glue.h>
#include <InterViews/window.h>
#include <InterViews/session.h>
#include <InterViews/patch.h>
#include <InterViews/layout.h>
#include <InterViews/polyglyph.h>
#include <InterViews/scrbox.h>
#include <InterViews/telltale.h>
#include <IV-look/button.h>
#include <IV-look/kit.h>

#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <strings.h>
#include <sys/param.h>
#include <memory.h>
#include <stdlib.h>

#ifndef NODE_H_
#include "Node.h"
#endif

#ifndef MAINSELECT_H_
#define _INIT_
#include "MainSelect.h"
#undef _INIT_
#endif

#ifndef COMMON_H_
#include "Common.h"
#endif

    
const int maxNumberOfNodes = 50;

int numberOfNodes = 0;
char* defaultName = "DependencyData";
char* fileName = 0;
char* nodeNames[maxNumberOfNodes];
ScrollBox* scrBox = 0;
Patch* patch = 0;


int main(int argc, char **argv)
{
    Boolean isNew = FALSE;
    Glyph *hbox = 0, *vbox = 0;
    NodeData* tempStore[maxNumberOfNodes];
    long totalDependency = 0;

    for (int j = 0; j < maxNumberOfNodes; j++)
    {
	nodeNames[j] = new char[MAXHOSTNAMELEN];
	::memset(nodeNames[j], '\0', MAXHOSTNAMELEN);
	tempStore[j] = 0;
    }
    
    for (j = 0; j < argc; j++)
    {
	if (strcmp(argv[j], "-help") == 0)
	{
	    cout << "MonitorDisplay [-file <datafile>] [-new]\n" << flush;
	    return 1;
	}

	if (strcmp(argv[j], "-file") == 0)
	    fileName = argv[j+1];

	if (strcmp(argv[j], "-new") == 0)
	    isNew = TRUE;
    }

    if (!fileName)
	fileName = defaultName;

    Session *s = new Session("DependencyDisplay", argc, argv, 0, 0);    

    if (!isNew)
    {
	ifstream inFile(fileName, ios::in);
    
	if (!inFile)
	{
	    cerr << "Cannot open " << fileName << " for input.\n" << flush;
	    return -1;
	}

	inFile >> numberOfNodes;

	for (j = 0; j < numberOfNodes; j++)
	{
	    NodeData *data = 0, *toAdd = 0, *ptr = 0;
	    char val;
	    
	    inFile >> nodeNames[j];
	    inFile >> val;

	    if (val == 'D')
	    {
		int number;

		inFile >> number;

		for (int i = 0; i < number; i++)
		{
		    if (!data)
		    {
			data = new NodeData;
			ptr = data;
		    }
		    else
		    {
			toAdd = new NodeData;
			ptr = toAdd;
		    }
		    
		    inFile >> ptr->hostName;
		    inFile >> val;

		    ptr->level = (DependencyLevel) atoi(&val);
		    data->addToList(ptr);
		}

		inFile >> val;
	    }
	    
	    tempStore[j] = data;
	    if (tempStore[j]->getTotalDependency() > totalDependency)
		totalDependency = tempStore[j]->getTotalDependency();
	}

	scrBox = LayoutKit::instance()->vscrollbox(numberOfNodes);

	for (j = 0; j < numberOfNodes; j++)
	{
	    Node* temp = new Node(nodeNames[j], tempStore[j],
				  (DependencyLevel)(tempStore[j]->getTotalDependency()*DEFINITE/totalDependency));
	    scrBox->append(temp);
	}
    }

    if (!scrBox)
	scrBox = LayoutKit::instance()->vscrollbox(1);

    Menu* m1 = WidgetKit::instance()->menubar();
    Button* b1 = WidgetKit::instance()->push_button("Node Dependencies", new Output);
    Button* b2 = WidgetKit::instance()->push_button("Add", new Add);
    Button* b3 = WidgetKit::instance()->push_button("Delete", new Delete);
    Button* b4 = WidgetKit::instance()->push_button("Modify", new Modify);
    Button* b5 = WidgetKit::instance()->push_button("Find", new Output);
    Button* b6 = WidgetKit::instance()->push_button("Output", new Output);
    Button* b7 = WidgetKit::instance()->push_button("Quit", WidgetKit::instance()->quit());
    m1->append_item(new MenuItem(b1, b1->state(), b1->action()));
    m1->append_item(new MenuItem(b2, b2->state(), b2->action()));
    m1->append_item(new MenuItem(b3, b3->state(), b3->action()));
    m1->append_item(new MenuItem(b4, b4->state(), b4->action()));
    m1->append_item(new MenuItem(b5, b5->state(), b5->action()));
    m1->append_item(new MenuItem(b6, b6->state(), b6->action()));
    m1->append_item(new MenuItem(b7, b7->state(), b7->action()));

    hbox = LayoutKit::instance()->hbox(scrBox, WidgetKit::instance()->vscroll_bar(scrBox));
    vbox = LayoutKit::instance()->vbox(hbox, m1);
    patch = new Patch(vbox);
    Window* window = new ApplicationWindow(new Background(patch,
							  WidgetKit::instance()->background()
							  )
					   );
    return s->run_window(window);
}

