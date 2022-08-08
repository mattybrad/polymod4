#ifndef Module_h
#define Module_h

class Module
{
public:
    Module();
    float freq;
    static float processA(Module module, float inVal);
    static float processB(Module module, float inVal);

private:
};

#endif