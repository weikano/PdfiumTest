#ifndef ANNOT_CONST_H
#define ANNOT_CONST_H

#include <string>
#include "public/fpdf_annot.h"

extern const char *kContents ;
extern const char *kIReaderNoteID ;
extern const char *kIReaderNoteLastModifiedTime ;
extern const char *kIReaderNoteRange;
//extern wchar_t *kTestTimeStamp = L"12345678";

void FPDFAnnotation_GetStringValueImpl(FPDF_ANNOTATION annot, const char* key, std::string& out);
#endif // ANNOT_CONST_H
