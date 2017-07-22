#ifndef SUBSELECT_H_
#include "SubSelect.h"
#endif

#ifndef MAINSELECT_H_
#include "MainSelect.h"
#endif


Cancel::Cancel (windowType w)
{
    wT = w;
}

Cancel::~Cancel ()
{
}

void Cancel::execute ()
{
    switch (wT)
    {
    case ADD:
	Add::myWindow->unmap();
	delete Add::myWindow;
	Add::myWindow = 0;
	buttonCheck::inUse = FALSE;
	break;
    case MODIFY:
	Modify::myWindow->unmap();
	delete Modify::myWindow;
	Modify::myWindow = 0;
	buttonCheck::inUse = FALSE;
	buttonCheck::modifyName = 0;
	break;
    case DELETE:
	Delete::myWindow->unmap();
	delete Delete::myWindow;
	Delete::myWindow = 0;
	buttonCheck::inUse = FALSE;
	buttonCheck::shouldDelete = FALSE;
	Delete::toRemove = -1;
	break;
    }
}
