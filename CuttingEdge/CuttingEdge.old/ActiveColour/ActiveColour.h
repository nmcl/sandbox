#ifndef _ACTIVECOLOUR_H_
#define _ACTIVECOLOUR_H_

#include <InterViews/color.h>
#include <Arjuna/LockManager.h>

#define  COLOURNAME_SIZE 32

class ActiveColour : public LockManager
{
public:
    static ActiveColour* Create(int& res, Uid obj_uid);
    static ActiveColour* Create(int& res, char* name, ColorIntensity r,
                                ColorIntensity g, ColorIntensity b);
    static void          Destroy();

    void getName(int& res, char*& name);

    void getColour(int& res, ColorIntensity& r, ColorIntensity& g,
                   ColorIntensity& b);

    void getRed  (int& res, ColorIntensity& r);
    void getGreen(int& res, ColorIntensity& g);
    void getBlue (int& res, ColorIntensity& b);

    void setColour(int& res, ColorIntensity r, ColorIntensity g,
                   ColorIntensity b);

    void setRed  (int& res, ColorIntensity r);
    void setGreen(int& res, ColorIntensity g);
    void setBlue (int& res, ColorIntensity b);

    virtual bool save_state(ObjectState&, ObjectType);
    virtual bool restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    ColorIntensity r;
    ColorIntensity g;
    ColorIntensity b;

    char colourName[COLOURNAME_SIZE];

    ActiveColour(int& res, Uid obj_uid);
    ActiveColour(int& res, char* name, ColorIntensity r,
                 ColorIntensity g, ColorIntensity b);
    virtual ~ActiveColour();

    static ActiveColour* head;
    ActiveColour*        next;
};
	
#endif  _ACTIVECOLOUR_H_
