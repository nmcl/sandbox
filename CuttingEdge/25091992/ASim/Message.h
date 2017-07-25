extern "C" {
    char *malloc(int);
    int free(char*);
    void bzero(char*, int);
    void bcopy(char*, char*, int);
}

extern Glyph *VMessage(const Font*, const Color*, char*, int);

extern Glyph *Message(const Font*, const Color*, char*);
