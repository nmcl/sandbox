#include <iostream.h>

#include <IV-look/kit.h>
#include <IV-look/dialogs.h>
#include <InterViews/layout.h>

#include "BasicAction.h"
#include "BasicActionDialog.h"

BasicActionDialog::BasicActionDialog()
{
    LayoutKit& layout = *LayoutKit::instance();
    WidgetKit& widget = *WidgetKit::instance();
    DialogKit& dialog = *DialogKit::instance();

    Glyph* vbox;

    body(layout.hbox());

    vbox = layout.vbox();
    vbox->append(widget.label("Name   :"));
    vbox->append(widget.label("Length :"));
    vbox->append(widget.label("Colour :"));
    append(vbox);

    vbox = layout.vbox();
    vbox->append(widget.label("Name   :"));
    vbox->append(widget.label("Length :"));
    vbox->append(widget.label("Colour :"));
    append(vbox);
}

BasicActionDialog::~BasicActionDialog()
{
}
