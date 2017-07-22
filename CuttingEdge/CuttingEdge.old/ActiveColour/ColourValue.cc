#include "ColourValue.h"

#define DELTA 0.05

ColourValue::ColourValue(ActiveColourEdit* r, Coord value)
{
    root      = r;
    curvalue_ = value;
    connect(this);
}

Coord ColourValue::lower(DimensionName) const { return 0.0; }
Coord ColourValue::upper(DimensionName) const { return 1.0; }
Coord ColourValue::length(DimensionName) const { return 1.0; }

Coord ColourValue::cur_lower(DimensionName d) const {
    return curvalue_ - DELTA;
}

Coord ColourValue::cur_upper(DimensionName d) const {
    return curvalue_ + DELTA;
}

Coord ColourValue::cur_length(DimensionName) const { return DELTA; }

void ColourValue::scroll_to(DimensionName, Coord position) {
    if (position != curvalue_) {
        Coord p = position;
        if (p <= 0.0) {
            p = 0.0;
        } else if (p >= 1.0) {
            p = 1.0;
        }
        curvalue_ = p;
        notify();
    }
}

void ColourValue::scroll_forward(DimensionName d) {
    scroll_to(d, curvalue_ + DELTA);
}

void ColourValue::scroll_backward(DimensionName d) {
    scroll_to(d, curvalue_ - DELTA);
}

void ColourValue::set_value(Coord v)
{
    curvalue_ = v;
    notify();
}

Coord ColourValue::get_value() const
{
    return curvalue_;
}

void ColourValue::update(DimensionName)
{
    if (root != nil)
	root->Update();
}

void ColourValue::update_all()
{
    if (root != nil)
	root->Update();
}
