#ifndef _COLOURVALUE_H_
#define _COLOURVALUE_H_

#include <IV-look/adjuster.h>
#include <IV-look/adjustable.h>

class ActiveColourEdit;
#include "ActiveColourEdit.h"

class ColourValue : public Adjustable, public Adjuster {
public:
    ColourValue(ActiveColourEdit* root, Coord value);

    virtual Coord lower(DimensionName) const;
    virtual Coord upper(DimensionName) const;
    virtual Coord length(DimensionName) const;
    virtual Coord cur_lower(DimensionName) const;
    virtual Coord cur_upper(DimensionName) const;
    virtual Coord cur_length(DimensionName) const;

    virtual void scroll_to(DimensionName, Coord position);
    virtual void scroll_forward(DimensionName);
    virtual void scroll_backward(DimensionName);

    virtual void  set_value(Coord);
    virtual Coord get_value() const;

    virtual void update(DimensionName);
    virtual void update_all();
private:
    Coord curvalue_;
    ActiveColourEdit* root;
};

#endif  _COLOURVALUE_H_
