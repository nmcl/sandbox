#include <IV-look/adjustable.h>
#include <IV-look/adjuster.h>
#include <IV-look/bevel.h>
#include <IV-look/kit.h>
#include <IV-look/panner.h>
#include <InterViews/background.h>
#include <InterViews/box.h>
#include <InterViews/center.h>
#include <InterViews/glue.h>
#include <InterViews/label.h>
#include <InterViews/margin.h>
#include <InterViews/patch.h>
#include <InterViews/session.h>
#include <InterViews/style.h>
#include <InterViews/window.h>
#include <OS/math.h>
#include <stdio.h>

class Valuator : public Adjustable {
public:
    Valuator(Coord lower, Coord upper);

    virtual Coord lower(DimensionName) const;
    virtual Coord upper(DimensionName) const;
    virtual Coord length(DimensionName) const;
    virtual Coord cur_lower(DimensionName) const;
    virtual Coord cur_upper(DimensionName) const;
    virtual Coord cur_length(DimensionName) const;

    void init(Coord value);
    virtual void scroll_to(DimensionName, Coord position);
    virtual void scroll_forward(DimensionName);
    virtual void scroll_backward(DimensionName);
private:
    Coord lower_;
    Coord span_;
    Coord curvalue_;
};

Valuator::Valuator(Coord lower, Coord upper) {
    lower_ = lower;
    span_ = upper - lower;
    curvalue_ = (lower + upper) / 2;
}

Coord Valuator::lower(DimensionName) const { return lower_; }
Coord Valuator::upper(DimensionName) const { return lower_ + span_; }
Coord Valuator::length(DimensionName) const { return span_; }

Coord Valuator::cur_lower(DimensionName d) const {
    return curvalue_ - cur_length(d) / 2;
}

Coord Valuator::cur_upper(DimensionName d) const {
    return curvalue_ + cur_length(d) / 2;
}

Coord Valuator::cur_length(DimensionName) const { return 20.0; }

void Valuator::init(Coord value) {
    curvalue_ = value;
    notify();
}

void Valuator::scroll_to(DimensionName, Coord position) {
    if (position != curvalue_) {
	Coord p = position;
	if (p <= lower_) {
	    p = lower_;
	} else if (p >= lower_ + span_) {
	    p = lower_ + span_;
	}
	curvalue_ = p;
	notify();
    }
}

void Valuator::scroll_forward(DimensionName d) {
    scroll_to(d, curvalue_ + 10.0);
}

void Valuator::scroll_backward(DimensionName d) {
    scroll_to(d, curvalue_ - 10.0);
}

class ValueDisplay : public MonoGlyph, public Adjuster {
public:
    ValueDisplay(Valuator*, Style*);
    virtual ~ValueDisplay();

    virtual void update_all();
private:
    Style* style_;
    Patch* patch_;
};

ValueDisplay::ValueDisplay(
    Valuator* v, Style* s
) : MonoGlyph(nil), Adjuster(v) {
    Resource::ref(s);
    style_ = s;
    patch_ = new Patch(nil);
    body(patch_);
}

ValueDisplay::~ValueDisplay() {
    Resource::unref(style_);
}

void ValueDisplay::update_all() {
    Adjustable* a = subject();
    Coord v = a->cur_lower(Dimension_X) + a->cur_length(Dimension_X) / 2;
    char buf[20];
    sprintf(buf, "value = %f", v);
    patch_->body(
	new HMargin(
	    new Label(buf, style_->font(), style_->foreground()),
	    0.0, fil, 0.0, 0.0, fil, 0.0
	)
    );
    patch_->redraw();
    patch_->reallocate();
    patch_->redraw();
}

int main(int argc, char** argv) {
    Session* session = new Session("ScrollTest", argc, argv);
    Style* style = session->style();
    Kit* kit = Kit::instance();
    Valuator* v1 = new Valuator(-100.0, 100.0);
    Valuator* v2 = new Valuator(-100.0, 100.0);
    Window* w = new ApplicationWindow(
	new Background(
	    new LRBox(
		new TBBox(
		    new VCenter(new VGlue, 1.0),
		    new ValueDisplay(v1, style),
		    new ValueDisplay(v2, style),
		    new VGlue,
		    kit->hscroll_bar(v1, style)
		),
		kit->vscroll_bar(v2, style)
	    ),
	    style->background()
	)
    );
    v1->init(0.0);
    v2->init(0.0);
    return session->run_window(w);
}


