#include <iostream.h>
#include <string.h>
#include <stdlib.h>

#include <InterViews/world.h>

#include "read.h"

void indent(int ind)
{
    for (int i = 0; i < ind; i++)
	cerr << " ";
}

void read_word(HyperWord*& word, int ind)
{
    char input[64];

    char* text, *deck, *card;

    cin >> input;
    text = strdup(input);

    int isActive;
    cin >> isActive;
    if (isActive == 1)
    {
	cin >> input;
	deck = strdup(input);

	cin >> input;
	card = strdup(input);

	indent(ind);
	cerr << "[WORD : " << text << ", " << deck << ", " << card << " ]\n";
    }
    else
    {
	deck = nil;
	card = nil;

	indent(ind);
	cerr << "[WORD : " << text << " ]\n";
    }

    word = new HyperWord(text, deck, card);
}

void read_word_list(HyperWord**& word_l, int& num, int ind)
{
    cin >> num;
    
    word_l = new HyperWord*[num];
    
    indent(ind);
    cerr << "[WORD_LIST : " << num << " ]\n";

    for (int i = 0; i < num; i++)
	read_word(word_l[i], ind + 1);
}

void read_image(HyperImage*& image, int ind)
{
    char input[64];

    char *filename, *deck, *card;

    cin >> input;
    filename = strdup(input);

    int isActive;
    cin >> isActive;
    if (isActive == 1)
    {
	cin >> input;
	deck = strdup(input);

	cin >> input;
	card = strdup(input);

	indent(ind);
	cerr << "[IMAGE : " << filename << ", " << deck << ", " << card << " ]\n";
    }
    else
    {
	deck = nil;
	card = nil;

	indent(ind);
	cerr << "[IMAGE : " << filename << " ]\n";
    }

    image = new HyperImage(filename, deck, card);
}

void read_text(HyperText*& text, int ind)
{
    int         width;
    HyperWord** word_l;
    int         num;

    cin >> width;

    indent(ind);
    cerr << "[TEXT : " << width << " ]\n";

    read_word_list(word_l, num, ind + 1);

    text = new HyperText(word_l, num, width);
}

void read_text_list(HyperText**& text_l, int& num, int ind)
{
    cin >> num;
    
    text_l = new HyperText*[num];
    
    indent(ind);
    cerr << "[TEXT_LIST : " << num << " ]\n";

    for (int i = 0; i < num; i++)
	read_text(text_l[i], ind + 1);
}

void read_card(HyperCard*& card, int ind)
{
    char input[64];
    
    char* name;
    cin >> input;
    name = strdup(input);

    indent(ind);
    cerr << "[CARD : " << name << "]\n";

    int     num;
    Glyph** body_l;
    read_type_list(body_l, num, ind + 1);

    card = new HyperCard(name, body_l, num);
}

void read_card_list(HyperCard**& card_l, int& num, int ind)
{
    cin >> num;
    
    card_l = new HyperCard*[num];

    indent(ind);
    cerr << "[CARD_LIST : " << num << " ]\n";

    for (int i = 0; i < num; i++)
	read_card(card_l[i], ind + 1);
}

void read_deck(HyperDeck*& deck, int ind)
{
    char input[64];
    
    char* name;
    cin >> input;
    name = strdup(input);

    indent(ind);
    cerr << "[DECK : " << name << " ]\n";

    HyperCard** card_l;
    int         num;
    read_card_list(card_l, num, ind + 1);

    deck = new HyperDeck(name, card_l, num);
}

void read_deck_list(HyperDeck**& deck_l, int& num, int ind)
{
    cin >> num;

    deck_l = new HyperDeck*[num];

    indent(ind);
    cerr << "[DECK_LIST : " << num << " ]\n";

    for (int i = 0; i < num; i++)
	read_deck(deck_l[i], ind + 1);
}

void read_window(HyperWindow*& win, int ind)
{
    char input[64];
    
    char* name;
    cin >> input;
    name = strdup(input);

    indent(ind);
    cerr << "[WINDOW : " << name << " ]\n";

    HyperDeck* deck;
    read_deck(deck, ind + 1);

    win = new HyperWindow(name, deck);
}

void read_window_list(HyperWindow**& win_l, int& num, int ind)
{
    cin >> num;

    win_l = new HyperWindow*[num];

    indent(ind);
    cerr << "[WINDOW_LIST : " << num << " ]\n";

    for (int i = 0; i < num; i++)
	read_window(win_l[i], ind + 1);
}

void read_mainwindow(HyperMainWindow*& mainwin, int ind)
{
    char input[64];
    
    char* name;
    cin >> input;
    name = strdup(input);

    indent(ind);
    cerr << "[MAINWINDOW : " << name << " ]\n";

    HyperDeck* deck;
    read_deck(deck, ind + 1);

    mainwin = new HyperMainWindow(name, deck);
}

void read_type(Glyph*& body, int ind)
{
    char input[64];

    char* type;
    cin >> input;
    type = strdup(input);

    indent(ind);
    cerr << "[TYPE : " << type << " ]\n";

    if (strcmp(type, "HyperText") == 0)
    {
	HyperText* text;
	read_text(text, ind + 1);
	body = text;
    }
    else if (strcmp(type, "HyperWord") == 0)
    {
	HyperWord* word;
	read_word(word, ind + 1);
	body = word;
    }
    else if (strcmp(type, "HyperImage") == 0)
    {
	HyperImage* image;
	read_image(image, ind + 1);
	body = image;
    }
    else
    {
	cerr << "read_type :: Unknown type\n";
	exit(9);
    }
}

void read_type_list(Glyph**& type_l, int& num, int ind)
{
    cin >> num;

    type_l = new Glyph*[num];

    indent(ind);
    cerr << "[TYPE_LIST : " << num << " ]\n";

    for (int i = 0; i < num; i++)
	read_type(type_l[i], ind + 1);
}
