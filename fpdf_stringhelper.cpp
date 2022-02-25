#include "fpdf_stringhelper.h"
#include <codecvt>
#include <locale>
#include "public/fpdf_edit.h"

std::wstring GetPlatformWString(FPDF_WIDESTRING wstr)
{
    if (!wstr)
        return std::wstring();
    size_t characters = 0;
    while (wstr[characters])
        ++characters;

    std::wstring platform_string(characters, L'\0');
    for (size_t i = 0; i < characters + 1; ++i) {
        const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&wstr[i]);
        platform_string[i] = ptr[0] + 256 * ptr[1];
    }
    return platform_string;
}

std::vector<FPDF_WCHAR> GetFPDFWideStringBuffer(size_t length_bytes)
{
    {
        return std::vector<FPDF_WCHAR>(length_bytes / sizeof(FPDF_WCHAR));
    }
}

void GetPlatformString(FPDF_WIDESTRING input, std::string &output)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::wstring wstr = GetPlatformWString(input);
    output.append(std::string(converter.to_bytes(wstr.c_str())));
}

const std::string& getObjTypeStr(int objType)
{
//#define FPDF_PAGEOBJ_UNKNOWN 0
//#define FPDF_PAGEOBJ_TEXT 1
//#define FPDF_PAGEOBJ_PATH 2
//#define FPDF_PAGEOBJ_IMAGE 3
//#define FPDF_PAGEOBJ_SHADING 4
//#define FPDF_PAGEOBJ_FORM 5
    static const std::string TYPE_UNKNOWN = "objTypeUnknown";
    static const std::string TYPE_TEXT = "objTypeText";
    static const std::string TYPE_PATH = "objTypePath";
    static const std::string TYPE_IMAGE = "objTypeImage";
    static const std::string TYPE_SHADING = "objTypeShading";
    static const std::string TYPE_FORM = "objTypeFrom";
    switch (objType) {
    case FPDF_PAGEOBJ_TEXT:
        return TYPE_TEXT;
    case FPDF_PAGEOBJ_PATH:
        return TYPE_PATH;
    case FPDF_PAGEOBJ_IMAGE:
        return TYPE_IMAGE;
    case FPDF_PAGEOBJ_SHADING:
        return TYPE_SHADING;
    case FPDF_PAGEOBJ_FORM:
        return TYPE_FORM;
    default:
        return TYPE_UNKNOWN;
    }
}

const std::string &getTypeStr(int type)
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
    static const std::string TYPE_UNKNOWN = "unknown";
    static const std::string TYPE_BOOLEAN = "boolean";
    static const std::string TYPE_NUMBER = "number";
    static const std::string TYPE_STRING = "string";
    static const std::string TYPE_NAME = "name";
    static const std::string TYPE_ARRAY = "array";
    static const std::string TYPE_DICTIONARY = "dictionary";
    static const std::string TYPE_STREAM = "stream";
    static const std::string TYPE_NULLOBJ = "null";
    static const std::string TYPE_REFERENCE = "reference";

    switch (type) {

    case FPDF_OBJECT_BOOLEAN:
        return TYPE_BOOLEAN;
    case FPDF_OBJECT_NUMBER:
        return TYPE_NUMBER;
    case FPDF_OBJECT_STRING:
        return TYPE_STRING;
    case FPDF_OBJECT_NAME:
        return TYPE_NAME;
    case FPDF_OBJECT_ARRAY:
        return TYPE_ARRAY;
    case FPDF_OBJECT_DICTIONARY:
        return TYPE_DICTIONARY;
    case FPDF_OBJECT_STREAM:
        return TYPE_STREAM;
    case FPDF_OBJECT_NULLOBJ:
        return TYPE_NULLOBJ;
    case FPDF_OBJECT_REFERENCE:
        return TYPE_REFERENCE;
    default:
        return TYPE_UNKNOWN;
    }
}
