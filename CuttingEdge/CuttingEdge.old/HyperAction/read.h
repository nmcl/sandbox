#include "HyperWord.h"

extern void read_word(HyperWord*& word, int ind);
extern void read_word_list(HyperWord**& word_l, int ind);

#include "HyperImage.h"

extern void read_image(HyperImage*& image, int ind);

#include "HyperText.h"

extern void read_text(HyperText*& text, int ind);

#include "HyperCard.h"

extern void read_card(HyperCard*& card, int ind);
extern void read_card_list(HyperCard**& card_l, int& num, int ind);

#include "HyperDeck.h"

extern void read_deck(HyperDeck*& deck, int ind);
extern void read_deck_list(HyperDeck**& deck_l, int& num, int ind);

#include "HyperWindow.h"

extern void read_window(HyperWindow*& win, int ind);
extern void read_window_list(HyperWindow**& win_l, int& num, int ind);

#include "HyperMainWindow.h"

extern void read_mainwindow(HyperMainWindow*& mainwin, int ind);

/* Other */

extern void read_type(Glyph*& body, int ind);
extern void read_type_list(Glyph**& body_l, int& num, int ind);
