#ifndef _ACTIVECOLOUREDIT_H_
#define _ACTIVECOLOUREDIT_H_

#include <InterViews/monoglyph.h>
#include <IV-look/adjuster.h>

#include "Area.h"
#include "ActiveColour.h"
class ColourValue;
#include "ColourValue.h"

class ActiveColourEdit : public MonoGlyph
{
public:
    ActiveColourEdit(ActiveColour* ac);
    virtual ~ActiveColourEdit();

    void Update();
    void Reload();
    void Save();
    void Quit();
private:
    ActiveColour* activeColour;
    Area*         colourArea;

    ColourValue* redVal;
    ColourValue* greenVal;
    ColourValue* blueVal;
};
	
#endif  _ACTIVECOLOUREDIT_H_
