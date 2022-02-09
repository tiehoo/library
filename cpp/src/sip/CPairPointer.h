#ifndef _CPAIRPOINTER_H
#define _CPAIRPOINTER_H

// 
class CPairPointer
{
public: 
    int     start;
    int     length;
    // 
    inline CPairPointer()
    {
    }
    // 
    inline CPairPointer(int startP, int lengthP)
    {
        start   = startP;
        length  = lengthP;
    }
    // 
    inline const CPairPointer& operator = (const CPairPointer& objPairPointer)
    {
        start   = objPairPointer.start;
        length  = objPairPointer.length;
    }
    // 
    ~CPairPointer()
    {
    }
};

#endif
