#ifndef PAGEOBJMARKVALUE_H
#define PAGEOBJMARKVALUE_H
union PageObjMarkValue
{
    //#define FPDF_OBJECT_UNKNOWN 0
    //#define FPDF_OBJECT_BOOLEAN 1
    //#define FPDF_OBJECT_NUMBER 2
    //#define FPDF_OBJECT_STRING 3
    //#define FPDF_OBJECT_NAME 4
    //#define FPDF_OBJECT_ARRAY 5
    //#define FPDF_OBJECT_DICTIONARY 6
    //#define FPDF_OBJECT_STREAM 7
    //#define FPDF_OBJECT_NULLOBJ 8
    //#define FPDF_OBJECT_REFERENCE 9
    float number;
    bool b;

};

#endif // PAGEOBJMARKVALUE_H
