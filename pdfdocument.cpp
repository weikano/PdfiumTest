#include <string.h>
#include <iostream>

#include "pdfdocument.h"
#include "public/fpdf_structtree.h"
#include "public/fpdf_catalog.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_text.h"

#include "fpdf_stringhelper.h"
#include "assert.h"

static int testPageIndex = 35;

PDFDocument::PDFDocument(const char *path, const char *pwd)
{
    mDoc = FPDF_LoadDocument(path, pwd);
}

PDFDocument::~PDFDocument()
{
    if(mDoc) {
        FPDF_CloseDocument(mDoc);
    }
}

int PDFDocument::getPageCount()
{
    if(mDoc) {
        return FPDF_GetPageCount(mDoc);
    }
    return 0;
}

void PDFDocument::walkStructtree()
{
    if(!FPDFCatalog_IsTagged(mDoc)) {
        return;
    }
    FPDF_PAGE page = FPDF_LoadPage(mDoc, testPageIndex);
    FPDF_STRUCTTREE tree = FPDF_StructTree_GetForPage(page);
    if(tree) {
        int count = FPDF_StructTree_CountChildren(tree);
        printf("tree children size(%d)\n", count);
        for(int i=0;i<count;i++) {
            FPDF_STRUCTELEMENT elem = FPDF_StructTree_GetChildAtIndex(tree, i);
//            printStructElement(elem);
            int size = FPDF_StructElement_CountChildren(elem);
            printStructElement(elem);
            for(int m=0;m<size;m++) {
                FPDF_STRUCTELEMENT e = FPDF_StructElement_GetChildAtIndex(elem, m);
                printStructElement(e);
            }
        }
        FPDF_StructTree_Close(tree);
    }
    FPDF_ClosePage(page);
}

FPDF_PAGE PDFDocument::loadPage(int index)
{
    if(mDoc) {
        return FPDF_LoadPage(mDoc, index);
    }
    return NULL;
}

void PDFDocument::ClosePage(FPDF_PAGE page)
{
    if(page) {
        FPDF_ClosePage(page);
    }
}

void PDFDocument::walkMarkObject()
{
    printFunction(__func__);
    FPDF_PAGE page = FPDF_LoadPage(mDoc, testPageIndex);
    int count = FPDFPage_CountObjects(page);
    printf("pageObjCount(%d)\n", count);
    for(int i=0;i<count;i++) {
        FPDF_PAGEOBJECT obj = FPDFPage_GetObject(page, i);
        int objType = FPDFPageObj_GetType(obj);
        if(objType == FPDF_PAGEOBJ_TEXT) {
            analyzeFPDFTextObj(page, obj);
        }
        int countMark = FPDFPageObj_CountMarks(obj);
        printf("%s, objMarksCount(%d)\n", getObjTypeStr(objType).c_str(), countMark);
        for(int m=0;m<countMark;m++) {
            FPDF_PAGEOBJECTMARK mark = FPDFPageObj_GetMark(obj, m);
            printf("mark at %d\n", m);
            showPageObjectMark(mark);
        }
    }
    FPDF_ClosePage(page);
    printFunction(__func__, false);
}

void PDFDocument::getStructElements(FPDF_PAGE page, std::vector<FPDF_STRUCTELEMENT> &out)
{
    FPDF_STRUCTTREE tree = FPDF_StructTree_GetForPage(page);
    int count = FPDF_StructTree_CountChildren(tree);
    FPDF_STRUCTELEMENT first = FPDF_StructTree_GetChildAtIndex(tree, 0);
    int childCount = FPDF_StructElement_CountChildren(first);
    for(int i=0;i<childCount;i++) {
        out.push_back(FPDF_StructElement_GetChildAtIndex(first, i));
    }
//    for(int i=0;i<count;i++) {
//        out.push_back(FPDF_StructTree_GetChildAtIndex(tree, i));
////        FPDF_STRUCTELEMENT elem = FPDF_StructTree_GetChildAtIndex(tree, i);
////        recursiveTreeElement(elem, out);
//    }
    FPDF_StructTree_Close(tree);
}

void PDFDocument::recursiveTreeElement(FPDF_STRUCTELEMENT element, std::vector<FPDF_STRUCTELEMENT> &out)
{
    if(!element) {
        return;
    }
    out.push_back(element);
    int count = FPDF_StructElement_CountChildren(element);    
    for(int i=0;i<count;i++) {
        FPDF_STRUCTELEMENT child = FPDF_StructElement_GetChildAtIndex(element,i);
        recursiveTreeElement(child, out);
    }
}

void PDFDocument::printStructElement(FPDF_STRUCTELEMENT elem)
{
    if(!elem) {
        return;
    }
    printf("=============ElementStart===============\n");
    int id = FPDF_StructElement_GetMarkedContentID(elem);
    printf("id(%d)\n",id);
    //get AltText
    unsigned long altTextLen = FPDF_StructElement_GetAltText(elem, nullptr, 0);
    unsigned short altTextBuf[altTextLen+1];
    memset(altTextBuf, 0, sizeof(altTextBuf));
    FPDF_StructElement_GetAltText(elem, altTextBuf, altTextLen);
    std::string altText;
    GetPlatformString(altTextBuf, altText);
    printf("altTextLen(%ld), altText(%s)\n", altTextLen, altText.c_str());
//    std::wstring altText = GetPlatformWString(altTextBuf);
//    std::wcout<<L"altText:"<<altText<<",";
    //get ActualText
    unsigned long actualTextLen = FPDF_StructElement_GetActualText(elem, nullptr, 0);
    unsigned short actualTextBuf[actualTextLen+1];
    memset(actualTextBuf, 0, sizeof(actualTextBuf));
    FPDF_StructElement_GetActualText(elem, actualTextBuf, actualTextLen);
    std::string actualText;
    GetPlatformString(actualTextBuf, actualText);
    printf("actualText(%ld),actualText(%s)\n", actualTextLen, actualText.c_str());
//    std::wstring actualText = GetPlatformWString(actualTextBuf);
//    std::wcout<<L"actualText:"<<actualText<<",";
    //GetType
    unsigned long typeLen = FPDF_StructElement_GetType(elem, nullptr, 0);
    unsigned short typeBuf[typeLen+1];
    memset(typeBuf, 0, sizeof(typeBuf));
    FPDF_StructElement_GetType(elem, typeBuf, typeLen);
    std::string type;
    GetPlatformString(typeBuf, type);
    printf("typeLen(%ld), type(%s)\n", typeLen, type.c_str());
//    std::wstring type = GetPlatformWString(typeBuf);
//    std::wcout<<L"type:"<<type<<",";
    //GetLang
    unsigned long langLen = FPDF_StructElement_GetLang(elem, nullptr, 0);
    unsigned short langBuf[langLen+1];
    memset(langBuf, 0, sizeof(langBuf));
    FPDF_StructElement_GetLang(elem, langBuf, langLen);
    std::string lang;
    GetPlatformString(langBuf, lang);
    printf("langLen(%ld), lang(%s)\n", langLen, lang.c_str());
//    std::wstring lang = GetPlatformWString(langBuf);
//    std::wcout<<L"lang:"<<lang<<",";
    //GetObjType
    unsigned long objTypeLen = FPDF_StructElement_GetObjType(elem, nullptr, 0);
    unsigned short objTypeBuf[objTypeLen+1];
    memset(objTypeBuf, 0, sizeof(objTypeBuf));
    FPDF_StructElement_GetObjType(elem, objTypeBuf, objTypeLen);
    std::string objTypeStr;
    GetPlatformString(objTypeBuf, objTypeStr);
    printf("objTypeLen(%ld), objType(%s)\n", objTypeLen, objTypeStr.c_str());
//    std::wstring objType = GetPlatformWString(objTypeBuf);
//    std::wcout<<L"objType:"<<objType<<",";
    //GetTitle
    unsigned long titleLen = FPDF_StructElement_GetTitle(elem, nullptr, 0);
    unsigned short titleBuf[titleLen+1];
    memset(titleBuf, 0, sizeof(titleBuf));
    FPDF_StructElement_GetTitle(elem, titleBuf, titleLen);
    std::string title;
    GetPlatformString(titleBuf, title);
    printf("titleLen(%ld), title(%s)\n", titleLen, title.c_str());
//    std::wstring title = GetPlatformWString(titleBuf);
//    std::wcout<<L"title:"<<title<<",";
    //GetAttributes
    int attrCount = FPDF_StructElement_GetAttributeCount(elem);
    for(int j=0;j<attrCount;j++) {
        FPDF_STRUCTELEMENT_ATTR attr = FPDF_StructElement_GetAttributeAtIndex(elem, j);
        int ac = FPDF_StructElement_Attr_GetCount(attr);
        for(int ai=0;ai<ac;ai++) {
            unsigned long attrNameLen;
            if(FPDF_StructElement_Attr_GetName(attr, ai, nullptr, 0, &attrNameLen)) {
                char attrNameBuf[attrNameLen];
                memset(attrNameBuf, 0, sizeof(attrNameBuf));
                FPDF_StructElement_Attr_GetName(attr, ai, attrNameBuf, attrNameLen, nullptr);
                printf("attrName(%s)\n", attrNameBuf);
//                std::cout<<"   attrName:" << attrNameBuf <<",";
                FPDF_OBJECT_TYPE attrType = FPDF_StructElement_Attr_GetType(attr, attrNameBuf);
                if(attrType == FPDF_OBJECT_NUMBER) {
                    float num_val;
                    FPDF_StructElement_Attr_GetNumberValue(attr, attrNameBuf, &num_val);
                    printf("attrNumberValue(%f)\n", num_val);
//                    std::cout<<" attrNumberValue: " << num_val;
                }else if(attrType == FPDF_OBJECT_STRING) {
                    unsigned long valueLen;
                    if(FPDF_StructElement_Attr_GetStringValue(attr, attrNameBuf, nullptr, 0, &valueLen)) {
                        char attrValueBuf[valueLen];
                        FPDF_StructElement_Attr_GetStringValue(attr, attrNameBuf, attrValueBuf, valueLen, nullptr);
                        printf("attrStringValue(%s)\n", attrValueBuf);
//                        std::cout<<" attrStringValue: " << attrValueBuf;
                    }
                }else if(attrType == FPDF_OBJECT_BOOLEAN) {
                    FPDF_BOOL boolValue;
                    FPDF_StructElement_Attr_GetBooleanValue(attr, attrNameBuf, &boolValue);
                    printf("attrBoolValue(%d)\n", boolValue);
                }else if(attrType == FPDF_OBJECT_STREAM) {
                    printf("attrBlobValue\n");
                }
            }

        }        
    }
    printf("=============ElementEnd===============\n");
}

void PDFDocument::showPageObjectMark(FPDF_PAGEOBJECTMARK mark)
{
    printf("===========%s begin===========\n", __func__);
    unsigned long len = 0;
    printf("try parse name\n");
    if(FPDFPageObjMark_GetName(mark, nullptr, 0, &len)) {
        unsigned short nameBuf[len+1];
        memset(nameBuf, 0, sizeof(nameBuf));
        FPDFPageObjMark_GetName(mark, nameBuf, sizeof(nameBuf), &len);
        std::string name;
        GetPlatformString(nameBuf, name);
        printf("name: %s \n", name.c_str());
    }
    int pc = FPDFPageObjMark_CountParams(mark);
    printf("try parse params size(%d)\n", pc);
    for(int i=0;i<pc;i++) {
        if(FPDFPageObjMark_GetParamKey(mark, i, nullptr, 0, &len)) {
            unsigned short keyBuf[len+1];
            memset(keyBuf, 0, sizeof(keyBuf));
            FPDFPageObjMark_GetParamKey(mark, i, keyBuf, sizeof(keyBuf), &len);
            std::string key;
            GetPlatformString(keyBuf, key);
            printf("key(%s), ", key.c_str());
            int type = FPDFPageObjMark_GetParamValueType(mark, key.c_str());
            const std::string typeStr = getTypeStr(type);
            printf("type(%s), ", typeStr.c_str());
            if(type == FPDF_OBJECT_NUMBER) {
                int iNum;
                FPDFPageObjMark_GetParamIntValue(mark, key.c_str(), &iNum);
                printf("intValue(%d) \n", iNum);
            }else if(type == FPDF_OBJECT_STRING) {
                if(FPDFPageObjMark_GetParamStringValue(mark, key.c_str(), nullptr, 0, &len)) {
                    unsigned short valueBuf[len+1];
                    memset(valueBuf, 0, sizeof(valueBuf));
                    FPDFPageObjMark_GetParamStringValue(mark, key.c_str(), valueBuf, sizeof(valueBuf), &len);
                    std::string strValue;
                    GetPlatformString(valueBuf, strValue);
                    printf("stringValue(%s) \n", strValue.c_str());
                }
            }else if(type == FPDF_OBJECT_STREAM) {
                printf("blobValue ignored \n");
            }
        }
    }
    printf("========%s end=======\n", __func__);

}

void PDFDocument::printFunction(const char *func, bool begin)
{
    printf("======begin %s %s ==========\n", func, begin ? "begin" : "end");
}

void PDFDocument::analyzeFPDFTextObj(FPDF_PAGE page, FPDF_PAGEOBJECT text_obj)
{
    FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
    unsigned long len = FPDFTextObj_GetText(text_obj, text_page, nullptr, 0);
    std::vector<FPDF_WCHAR> data = GetFPDFWideStringBuffer(len);
    FPDFTextObj_GetText(text_obj, text_page, data.data(), len);
    std::string text;
    GetPlatformString(data.data(), text);
    float font_size = 0;
    FPDFTextObj_GetFontSize(text_obj, &font_size);
//    FPDF_FONT font = FPDFTextObj_GetFont(text_obj);
//    len = FPDFFont_GetFontName(font, nullptr, 0);
//    char fontBuf[len];
//    FPDFFont_GetFontName(font, fontBuf, len);
//    int flags = FPDFFont_GetFlags(font);
//    int angle = 0;
//    FPDFFont_GetItalicAngle(font, &angle);
//    FPDFFont_Close(font);
    printf("font_size(%f), text_content(%s)\n",font_size, text.c_str());
    FPDFText_ClosePage(text_page);
//    printf("%s(text:%s, font_size:%f, font_name:%s, flag:%d, angle:%d)\n", __func__, text.c_str(), font_size, fontBuf, flags, angle);
}
