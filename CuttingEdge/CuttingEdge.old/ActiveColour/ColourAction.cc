
#include "ColourAction.h"

ColourAction::ColourAction(ActiveColourEdit* r, enum ColourActionType t)
{
    root = r;
    type = t;
}

ColourAction::~ColourAction()
{
}

void ColourAction::execute()
{
    if (type == Reload_env)
	root->Reload();
    if (type == Save_env)
	root->Save();
    if (type == Quit_env)
	root->Quit();
}
