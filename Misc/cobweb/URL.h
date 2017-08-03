#ifndef COBWEB_URL_H_
#define COBWEB_URL_H_

class ostream;

class URL
{
public:
    URL();
    virtual ~URL();

    void parse(const char*);

    ostream& print (ostream&) const;
    ostream& printInternal (ostream&) const;

private:

    // URL State
    int accessLength;
    char *access;
    int hostLength;
    char *host;
    int absoluteLength;
    char *absolute;
    int relativeLength;
    char *relative;
    int anchorLength;
    char *anchor;
};

extern ostream& operator<< (ostream& strm, const URL&);

#endif // COBWEB_URL_H_
