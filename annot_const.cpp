#include "annot_const.h"

#include <iostream>
#include <vector>
#include "fpdf_stringhelper.h"

const char *kContents = "Contents";
const char *kIReaderNoteID = "kIReaderNoteID";
const char *kIReaderNoteLastModifiedTime = "kIReaderNoteLastModifiedTime";
const char *kIReaderNoteRange = "kIReaderNoteRange";

void FPDFAnnotation_GetStringValueImpl(FPDF_ANNOTATION annot, const char *key, std::string &out)
{    
    FPDF_OBJECT_TYPE type = FPDFAnnot_GetValueType(annot, key);    
//    std::string stringstr;
    if(type == FPDF_OBJECT_STRING) {
        unsigned long len = FPDFAnnot_GetStringValue(annot, key, nullptr, 0);
        std::vector<FPDF_WCHAR> buf = GetFPDFWideStringBuffer(len);
        FPDFAnnot_GetStringValue(annot, key, buf.data(), len);
        GetPlatformString(buf.data(), out);        
    }    
}
