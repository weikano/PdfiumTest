#ifndef FPDF_STRINGHELPER_H
#define FPDF_STRINGHELPER_H
#include <string>
#include "public/fpdfview.h"
#include <vector>
std::wstring GetPlatformWString(FPDF_WIDESTRING wstr);


std::vector<FPDF_WCHAR> GetFPDFWideStringBuffer(size_t length_bytes);

void GetPlatformString(FPDF_WIDESTRING wstr, std::string& output);

const std::string &getTypeStr(int type);

const std::string &getObjTypeStr(int objType);

#endif // FPDF_STRINGHELPER_H
