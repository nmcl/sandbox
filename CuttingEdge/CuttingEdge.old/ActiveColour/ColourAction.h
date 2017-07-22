#ifndef _COLOURACTION_H_
#define _COLOURACTION_H_

#include <InterViews/action.h>

class ActiveColourEdit;
#include "ActiveColourEdit.h"

enum ColourActionType { Reload_env, Save_env, Quit_env };

class ColourAction : public Action
{
    enum ColourActionType type;
    ActiveColourEdit* root;
public:
    ColourAction(ActiveColourEdit*, enum ColourActionType);
    ~ColourAction();

    virtual void execute();
};

#endif  _COLOURACTION_H_
