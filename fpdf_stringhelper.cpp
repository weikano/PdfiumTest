#include "fpdf_stringhelper.h"
#include <codecvt>
#include <locale>
#include <malloc.h>
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

FPDF_WIDESTRING GetFPDFWideString(const std::wstring& wstr)
{
    size_t length = sizeof(uint16_t) * (wstr.length() + 1);
    printf("%s len:%zu\n", __func__, length);
    GetFPDFWideStringBuffer(length);
//    unsigned short result[length];
    unsigned short *result = (unsigned short*)malloc(length);
    size_t i=0;
    for(wchar_t w: wstr) {
        result[i++] = w & 0xff;
        result[i++] = (w>>8) & 0xff;
    }
    result[i++] = 0;
    result[i] = 0;
    return result;
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

//void utf8_to_utf16(unsigned char * const utf8_str, int utf8_str_size, char16_t *utf16_str_output, int utf16_str_output_size)
//{
//    unsigned char* utf8_currentCodeUnit = utf8_str;
//    char16_t* utf16_currentCodeUnit = utf16_str_output;
//    int utf8_str_iterator = 0;
//    int utf16_str_iterator = 0;

//    //In a while loop, we check if the UTF-16 iterator is less than the max output size. If true, then we check if UTF-8 iterator
//    //is less than UTF-8 max string size. This conditional checking based on order of precedence is intentionally done so it
//    //prevents the while loop from continuing onwards if the iterators are outside of the intended sizes.
//    while (*utf8_currentCodeUnit && (utf16_str_iterator < utf16_str_output_size || utf8_str_iterator < utf8_str_size)) {
//        //Figure out the current code unit to determine the range. It is split into 6 main groups, each of which handles the data
//        //differently from one another.
//        if (*utf8_currentCodeUnit < 0x80) {
//            //0..127, the ASCII range.

//            //We directly plug in the values to the UTF-16 code unit.
//            *utf16_currentCodeUnit = (char16_t) (*utf8_currentCodeUnit);
//            utf16_currentCodeUnit++;
//            utf16_str_iterator++;

//            //Increment the current code unit pointer to the next code unit
//            utf8_currentCodeUnit++;

//            //Increment the iterator to keep track of where we are in the UTF-8 string
//            utf8_str_iterator++;
//        }
//        else if (*utf8_currentCodeUnit < 0xC0) {
//            //0x80..0xBF, we ignore. These are reserved for UTF-8 encoding.
//            utf8_currentCodeUnit++;
//            utf8_str_iterator++;
//        }
//        else if (*utf8_currentCodeUnit < 0xE0) {
//            //128..2047, the extended ASCII range, and into the Basic Multilingual Plane.

//            //Work on the first code unit.
//            char16_t highShort = (char16_t) ((*utf8_currentCodeUnit) & 0x1F);

//            //Increment the current code unit pointer to the next code unit
//            utf8_currentCodeUnit++;

//            //Work on the second code unit.
//            char16_t lowShort = (char16_t) ((*utf8_currentCodeUnit) & 0x3F);

//            //Increment the current code unit pointer to the next code unit
//            utf8_currentCodeUnit++;

//            //Create the UTF-16 code unit, then increment the iterator.
//            //Credits to @tbeu.
//            //Thanks to @k6l2 for explaining why we need 6 instead of 8.
//            //It's because 0x3F is 6 bits of information from the low short. By shifting 8 bits, you are
//            //adding 2 extra zeroes in between the actual data of both shorts.
//            int unicode = (highShort << 6) | lowShort;

//            //Check to make sure the "unicode" is in the range [0..D7FF] and [E000..FFFF].
//            if ((0 <= unicode && unicode <= 0xD7FF) || (0xE000 <= unicode && unicode <= 0xFFFF)) {
//                //Directly set the value to the UTF-16 code unit.
//                *utf16_currentCodeUnit = (char16_t) unicode;
//                utf16_currentCodeUnit++;
//                utf16_str_iterator++;
//            }

//            //Increment the iterator to keep track of where we are in the UTF-8 string
//            utf8_str_iterator += 2;
//        }
//        else if (*utf8_currentCodeUnit < 0xF0) {
//            //2048..65535, the remaining Basic Multilingual Plane.

//            //Work on the UTF-8 code units one by one.
//            //If drawn out, it would be 1110aaaa 10bbbbcc 10ccdddd
//            //Where a is 4th byte, b is 3rd byte, c is 2nd byte, and d is 1st byte.
//            char16_t fourthChar = (char16_t) ((*utf8_currentCodeUnit) & 0xF);
//            utf8_currentCodeUnit++;
//            char16_t thirdChar = (char16_t) ((*utf8_currentCodeUnit) & 0x3C) >> 2;
//            char16_t secondCharHigh = (char16_t) ((*utf8_currentCodeUnit) & 0x3);
//            utf8_currentCodeUnit++;
//            char16_t secondCharLow = (char16_t) ((*utf8_currentCodeUnit) & 0x30) >> 4;
//            char16_t firstChar = (char16_t) ((*utf8_currentCodeUnit) & 0xF);
//            utf8_currentCodeUnit++;

//            //Create the resulting UTF-16 code unit, then increment the iterator.
//            int unicode = (fourthChar << 12) | (thirdChar << 8) | (secondCharHigh << 6) | (secondCharLow << 4) | firstChar;

//            //Check to make sure the "unicode" is in the range [0..D7FF] and [E000..FFFF].
//            //According to math, UTF-8 encoded "unicode" should always fall within these two ranges.
//            if ((0 <= unicode && unicode <= 0xD7FF) || (0xE000 <= unicode && unicode <= 0xFFFF)) {
//                //Directly set the value to the UTF-16 code unit.
//                *utf16_currentCodeUnit = (char16_t) unicode;
//                utf16_currentCodeUnit++;
//                utf16_str_iterator++;
//            }

//            //Increment the iterator to keep track of where we are in the UTF-8 string
//            utf8_str_iterator += 3;
//        }
//        else if (*utf8_currentCodeUnit < 0xF8) {
//            //65536..10FFFF, the Unicode UTF range

//            //Work on the UTF-8 code units one by one.
//            //If drawn out, it would be 11110abb 10bbcccc 10ddddee 10eeffff
//            //Where a is 6th byte, b is 5th byte, c is 4th byte, and so on.
//            char16_t sixthChar = (char16_t) ((*utf8_currentCodeUnit) & 0x4) >> 2;
//            char16_t fifthCharHigh = (char16_t) ((*utf8_currentCodeUnit) & 0x3);
//            utf8_currentCodeUnit++;
//            char16_t fifthCharLow = (char16_t) ((*utf8_currentCodeUnit) & 0x30) >> 4;
//            char16_t fourthChar = (char16_t) ((*utf8_currentCodeUnit) & 0xF);
//            utf8_currentCodeUnit++;
//            char16_t thirdChar = (char16_t) ((*utf8_currentCodeUnit) & 0x3C) >> 2;
//            char16_t secondCharHigh = (char16_t) ((*utf8_currentCodeUnit) & 0x3);
//            utf8_currentCodeUnit++;
//            char16_t secondCharLow = (char16_t) ((*utf8_currentCodeUnit) & 0x30) >> 4;
//            char16_t firstChar = (char16_t) ((*utf8_currentCodeUnit) & 0xF);
//            utf8_currentCodeUnit++;

//            int unicode = (sixthChar << 4) | (fifthCharHigh << 2) | fifthCharLow | (fourthChar << 12) | (thirdChar << 8) | (secondCharHigh << 6) | (secondCharLow << 4) | firstChar;
//            char16_t highSurrogate = (unicode - 0x10000) / 0x400 + 0xD800;
//            char16_t lowSurrogate = (unicode - 0x10000) % 0x400 + 0xDC00;

//            //Set the UTF-16 code units
//            *utf16_currentCodeUnit = lowSurrogate;
//            utf16_currentCodeUnit++;
//            utf16_str_iterator++;

//            //Check to see if we're still below the output string size before continuing, otherwise, we cut off here.
//            if (utf16_str_iterator < utf16_str_output_size) {
//                *utf16_currentCodeUnit = highSurrogate;
//                utf16_currentCodeUnit++;
//                utf16_str_iterator++;
//            }

//            //Increment the iterator to keep track of where we are in the UTF-8 string
//            utf8_str_iterator += 4;
//        }
//        else {
//            //Invalid UTF-8 code unit, we ignore.
//            utf8_currentCodeUnit++;
//            utf8_str_iterator++;
//        }
//    }

//    //We clean up the output string if the UTF-16 iterator is still less than the output string size.
//    while (utf16_str_iterator < utf16_str_output_size) {
//        *utf16_currentCodeUnit = '\0';
//        utf16_currentCodeUnit++;
//        utf16_str_iterator++;
//    }
//}
