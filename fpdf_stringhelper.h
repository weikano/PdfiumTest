#ifndef FPDF_STRINGHELPER_H
#define FPDF_STRINGHELPER_H
#include <string>
#include <vector>

#include "public/fpdfview.h"

FPDF_WIDESTRING GetFPDFWideString(const std::wstring& wstr);
std::wstring GetPlatformWString(FPDF_WIDESTRING wstr);

std::vector<FPDF_WCHAR> GetFPDFWideStringBuffer(size_t length_bytes);

void GetPlatformString(FPDF_WIDESTRING wstr, std::string& output);

const std::string &getTypeStr(int type);

const std::string &getObjTypeStr(int objType);

//void utf8_to_utf16(unsigned char* const utf8_str, int utf8_str_size, char16_t* utf16_str_output, int utf16_str_output_size);

#endif // FPDF_STRINGHELPER_H
