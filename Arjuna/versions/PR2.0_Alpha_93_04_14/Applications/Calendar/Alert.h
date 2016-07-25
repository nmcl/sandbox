/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Alert.h,v 1.2 1993/03/18 13:50:39 nmcl Exp $
 */

#include <InterViews/action.h>

class Button;
class Glyph;
class Session;
class TransientWindow;

class Alert
{
public:
    Alert(Session *s, Window *p, Glyph *msg, Glyph *btn_msg);
    ~Alert();

    void run();

protected:
    void make_alert();
    void exit_alert();

private:
    Session *session;
    Glyph *message;
    Glyph *button_msg;
    TransientWindow *alert_window;
    Window *parent;
    Button *button;
    int flag;
};

ActionCallbackdeclare(Alert);
