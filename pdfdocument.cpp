#include <string.h>
#include <iostream>
#include <malloc.h>
#include <assert.h>
#include <algorithm>

#include "pdfdocument.h"

#include "public/fpdf_structtree.h"
#include "public/fpdf_catalog.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_text.h"
#include "public/fpdf_annot.h"
#include "public/cpp/fpdf_scopers.h"

#include "fpdf_stringhelper.h"

#include "filewriterimpl.h"

#include "PDFUtil.h"
#include "annot_const.h"
#include "ireadernote.h"


static int testPageIndex = 35;

//format string
template<typename ...Args>
std::string string_format(const std::string& format, Args... args)
{
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args...);
    char bytes[size];
    snprintf(bytes, size, format.c_str(), args...);
    return std::string(bytes);
}

bool FS_RECTF_unset(const FS_RECTF &in) {
    return in.left == 0 && in.top == 0 && in.bottom == 0 && in.right == 0;
}

void FS_RECTF_union(FS_RECTF &in, FS_RECTF *target) {
    if(FS_RECTF_unset(in)) {
        in.left = target->left;
        in.top = target->top;
        in.right = target->right;
        in.bottom = target->bottom;
    }else {
        in.left = std::min(in.left, target->left);
        in.right = std::max(in.right, target->right);
        in.top = std::max(in.top, target->top);
        in.bottom = std::max(in.bottom, target->bottom);
    }
}

void FS_RECTF_init(FS_RECTF &rect)
{
    rect.left = 0;
    rect.top = 0;
    rect.right = 0;
    rect.bottom = 0;
}

void FS_RECTF_union(FS_RECTF &in, double l, double t, double r, double b) {
    if(l == r || t == b) {
        return;
    }
    if(FS_RECTF_unset(in)) {
        in.left = l;
        in.top = t;
        in.right = r;
        in.bottom = b;
    }else {
        in.left = std::min(in.left, (float)l);
        in.right = std::max(in.right, (float)r);
        in.top = std::max(in.top, (float)t);
        in.bottom = std::min(in.bottom, (float)b);
    }
}

void FS_RECTF_union(FS_RECTF &in, const FS_RECTF &charbox)
{
    FS_RECTF_union(in, charbox.left, charbox.top, charbox.right, charbox.bottom);
}

static bool FS_RECT_insect_vertical(const FS_RECTF &charbox, const FS_RECTF &linebox)
{
    return !(charbox.bottom > linebox.top || charbox.top < linebox.bottom);
}


void FS_RECTF_set(FS_RECTF &target, double l, double r, double b, double t)
{
    target.left = l;
    target.right =r;
    target.bottom = b;
    target.top = t;
}

void FS_RECTF_lines(FS_RECTF &charbox, std::vector<FS_RECTF> &lines)
{
    int index = -1;
    for(int i=0, size = lines.size(); i<size; i++) {
        if(FS_RECT_insect_vertical(charbox, lines.at(i))) {
            index = i;
            break;
        }
    }
    if(index == -1) {
        lines.push_back(charbox);
    }else {
        auto tmp = lines[index];
        FS_RECTF_union(tmp, charbox);
        lines[index] = tmp;
    }
}

static void getBoundedText(FPDF_TEXTPAGE text_page, const FS_RECTF &rect);
static void rect2quadpoint(const FS_RECTF &rect, FS_QUADPOINTSF &point)
{
    point.x1 = rect.left;
    point.y1 = rect.top;

    point.x2 = rect.right;
    point.y2 = rect.top;

    point.x3 = rect.left;
    point.y3 = rect.bottom;

    point.x4 = rect.right;
    point.y4 = rect.bottom;
}
static void getFileIdentifier(FPDF_DOCUMENT doc, FPDF_FILEIDTYPE type)
{
    unsigned long buflen = FPDF_GetFileIdentifier(doc, type, nullptr, 0);
    if(buflen > 0){
        unsigned char buf[buflen];
        FPDF_GetFileIdentifier(doc, type, buf, buflen);
        for(int i=0;i<buflen;i++) {
            printf("%hx", buf[i]);
        }
        printf("\n");
    }

//    printf("len :%lu, id: %s\n", buflen, buf);
}

PDFDocument::PDFDocument(const char *path, const char *pwd)
{
    mDoc = FPDF_LoadDocument(path, pwd);
    getFileIdentifier(mDoc, FILEIDTYPE_PERMANENT);
    getFileIdentifier(mDoc, FILEIDTYPE_CHANGING);    
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

void PDFDocument::walkAnnotations()
{
    int pageSize = FPDF_GetPageCount(mDoc);
    for(int i=0;i<pageSize; i++) {
        ScopedFPDFPage page = ScopedFPDFPage(FPDF_LoadPage(mDoc, i));
//        FPDF_PAGE page = FPDF_LoadPage(mDoc, i);
        ScopedFPDFTextPage text_page = ScopedFPDFTextPage(FPDFText_LoadPage(page.get()));
        analyzeAnnotations(i, page.get(), text_page.get());
//        FPDF_ClosePage(page);
//        FPDFText_ClosePage(text_page);
    }
}

void PDFDocument::walkTextObject()
{
    int pageSize = FPDF_GetPageCount(mDoc);
    for(int i=0;i<pageSize; i++) {
        FPDF_PAGE page = FPDF_LoadPage(mDoc, i);
        analyzeTextObject(i, page);
        FPDF_ClosePage(page);
    }
}

void PDFDocument::removeAllAnnotations()
{
    //todo weikan
}

static void setAnnotStringValue(const FPDF_ANNOTATION &annot, const std::string& key, const std::string &value)
{
    printf("AnnotationSetValue key:%s, value:%s\n", key.c_str(), value.c_str());
    int utfLen = PDFUtil::utf8_length(value.c_str(), value.length());
    unsigned short buf[utfLen+1];
    utfLen = PDFUtil::utf8_to_ucs2(buf, utfLen+1, value.c_str(), -1);
    buf[utfLen] = L'\0';
    FPDFAnnot_SetStringValue(annot, key.c_str(), buf);
}

void PDFDocument::addAnnotations(int startIndex, int endIndex, const std::string &msg)
{
    printf("======%s=====\n", __func__);
    ScopedFPDFPage page = ScopedFPDFPage(FPDF_LoadPage(mDoc, 0));
    ScopedFPDFAnnotation annot = ScopedFPDFAnnotation(FPDFPage_CreateAnnot(page.get(), FPDF_ANNOT_UNDERLINE));
    ScopedFPDFTextPage text_page = ScopedFPDFTextPage(FPDFText_LoadPage(page.get()));   
    //get the rect for text range
    FS_RECTF rect;
    FS_RECTF_init(rect);

    int charCount = FPDFText_CountChars(text_page.get());
    endIndex = std::min(endIndex, charCount -1);
    printf("======start-end:%d, %d=====\n", startIndex, endIndex);
    std::vector<FS_RECTF> lines;
    listLines(0, startIndex, endIndex, lines);
    for(const auto line : lines)
    {
        FS_RECTF_union(rect, line);
    }
    getBoundedText(text_page.get(), rect);
    printf("=====Annot rect(%f, %f, %f, %f)=====\n", rect.left, rect.top, rect.right, rect.bottom);
    //set rect
    std::vector<FS_QUADPOINTSF> points;
    for(const auto line : lines)
    {
        FS_QUADPOINTSF p;
        rect2quadpoint(line, p);
        points.push_back(p);
    }
    for(const auto p : points)
    {
        FPDFAnnot_AppendAttachmentPoints(annot.get(), &p);
    }
    FPDFAnnot_SetRect(annot.get(), &rect);

    if(msg.length() > 0) {
        setAnnotStringValue(annot.get(), kContents, msg);
        setAnnotStringValue(annot.get(), kIReaderNoteID, "ADFALSDJFLASJDFAS");
        std::string timestamp = string_format("%ld", 123456780988L);
        setAnnotStringValue(annot.get(), kIReaderNoteLastModifiedTime, timestamp);
        std::string range = string_format("%d_%d", startIndex, endIndex);
//        sscanf(range.c_str(), "%d_%d", &start, &end);
        setAnnotStringValue(annot.get(), kIReaderNoteRange, range);
    }

    //RED
    FPDFAnnot_SetColor(annot.get(), FPDFANNOT_COLORTYPE_Color, 239,40,40,255);
    if(FPDFPage_GenerateContent(page.get())) {
        printf("generate content\n");
    }

    if(mSavePath) {
        FileWriterImpl* writer = new FileWriterImpl;
        writer->OpenPDFFileForWrite(mSavePath);
        if(FPDF_SaveAsCopy(mDoc, writer, 0)) {
            printf("SaveAsCopy\n");
        }
        writer->ClosePDFFileForWrite();
        delete writer;
    }

}

void PDFDocument::listLines(int pageIndex, int startIndex, int endIndex, std::vector<FS_RECTF> &lines)
{
    ScopedFPDFPage page = ScopedFPDFPage(FPDF_LoadPage(mDoc, pageIndex));
    ScopedFPDFTextPage text_page = ScopedFPDFTextPage(FPDFText_LoadPage(page.get()));
    int count = FPDFText_CountChars(text_page.get());
    endIndex = std::min(endIndex, count -1);
    int len = endIndex - startIndex + 1;
//    int len = FPDFText_GetText(text_page.get(), startIndex, endIndex - startIndex + 1, nullptr);
    std::vector<FPDF_WCHAR> buf = GetFPDFWideStringBuffer(len + 1);
    FPDFText_GetText(text_page.get(), startIndex, len, buf.data());
    FPDFText_GetText(text_page.get(), startIndex, endIndex - startIndex + 1, buf.data());
    std::string text;
    GetPlatformString(buf.data(), text);
    printf("lineLines text :%s\n", text.c_str());
    for(int i=startIndex; i<=endIndex; i++) {
            FS_RECTF rect;
            FS_RECTF_init(rect);
            double l,r,b,t;
            FPDFText_GetCharBox(text_page.get(), i, &l, &r, &b, &t);
            FS_RECTF_set(rect, l, r, b, t);
            FS_RECTF_lines(rect, lines);
    }
}

void PDFDocument::listIReaderAnnotations(int pageIndex, std::vector<const IReaderNote*> &notes)
{
    ScopedFPDFPage page = ScopedFPDFPage(FPDF_LoadPage(mDoc, pageIndex));
//    std::vector<const IReaderNote*> notes;
    int size = FPDFPage_GetAnnotCount(page.get());
    for(int i=0;i<size;i++) {
        ScopedFPDFAnnotation annot = ScopedFPDFAnnotation(FPDFPage_GetAnnot(page.get(), i));
        if(FPDFAnnot_HasKey(annot.get(), kIReaderNoteID)) {
            IReaderNote *note = new IReaderNote(i);
            note->loadData(page.get());
            notes.push_back(note);
        }
    }
}

void PDFDocument::setSavePath(const char *newSavePath)
{
    mSavePath = newSavePath;
}

//void PDFDocument::listLines(int pageIndex, int startIndex, int endIndex, std::vector<FS_RECTF &> lines)
//{
//    ScopedFPDFPage page = ScopedFPDFPage(FPDF_LoadPage(mDoc, pageIndex));
//    ScopedFPDFTextPage text_page = ScopedFPDFTextPage(FPDFText_LoadPage(page.get()));
//    int count = FPDFText_CountChars(text_page.get());
//    endIndex = std::min(endIndex, count -1);
//    for(int i=startIndex; i<=endIndex; i++) {
//        FS_RECTF rect;
//        FS_RECTF_init(rect);
//        double l,r,b,t;
//        FPDFText_GetCharBox(text_page.get(), i, &l, &r, &b, &t);
//        FS_RECTF_set(rect, l, r, b, t);
//    }
//}

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



static void convertQuadPoinTORect(const FS_QUADPOINTSF & point, FS_RECTF & rect)
{
    rect.left = point.x1;
    rect.bottom = point.y1;
    rect.right = point.x4;
    rect.top = point.y4;
}

static void getBoundedText(FPDF_TEXTPAGE text_page, const FS_RECTF &rect)
{
    int len = FPDFText_GetBoundedText(text_page, rect.left, rect.top, rect.right, rect.bottom, nullptr, 0);
    if(len > 0) {
        std::vector<FPDF_WCHAR> buf = GetFPDFWideStringBuffer(len);
        FPDFText_GetBoundedText(text_page, rect.left, rect.top, rect.right, rect.bottom, buf.data(), len);
        std::string text;
        GetPlatformString(buf.data(), text);
        printf("bounded text:%s\n", text.c_str());
    }
}

static void printAnnotationStringValue(FPDF_ANNOTATION &annot, const char* &key)
{
     FPDF_OBJECT_TYPE type = FPDFAnnot_GetValueType(annot, key);
     std::cout<<"key:"<<key<<", valueType:" <<getTypeStr(type);
     std::string stringstr;
     if(type == FPDF_OBJECT_STRING) {
         unsigned long len = FPDFAnnot_GetStringValue(annot, key, nullptr, 0);
         std::vector<FPDF_WCHAR> buf = GetFPDFWideStringBuffer(len);
         FPDFAnnot_GetStringValue(annot, key, buf.data(), len);
         GetPlatformString(buf.data(), stringstr);
         std::cout<<", value:"<<stringstr;
     }
     std::cout<<std::endl;
}

void PDFDocument::analyzeAnnotations(int index, FPDF_PAGE page, FPDF_TEXTPAGE text_page)
{    
    printf("========begin analyzeAnnotations on page %d==========\n", index);
    printf("=========page size(%f, %f)=========\n", FPDF_GetPageWidthF(page), FPDF_GetPageHeightF(page));
    int count = FPDFPage_GetAnnotCount(page);
    printf("==============annotation count(%d)==========\n", count);
    for(int i=0;i<count;i++) {
        printf("=======analyze annotation :%d======\n", i);
        FPDF_ANNOTATION annot = FPDFPage_GetAnnot(page, i);
        FPDF_ANNOTATION_SUBTYPE subtype = FPDFAnnot_GetSubtype(annot);
        unsigned long aplen = FPDFAnnot_GetAP(annot, FPDF_ANNOT_APPEARANCEMODE_NORMAL, nullptr, 0);
        std::vector<FPDF_WCHAR> apbuf = GetFPDFWideStringBuffer(aplen);
        FPDFAnnot_GetAP(annot, FPDF_ANNOT_APPEARANCEMODE_NORMAL, apbuf.data(), aplen);
        std::string ap;
        GetPlatformString(apbuf.data(), ap);
        int flags = FPDFAnnot_GetFlags(annot);
        FS_RECTF rect;
        FS_RECTF_init(rect);
//        if(FPDFAnnot_GetRect(annot, &rect)) {
//            getBoundedText(text_page, rect);
//        }

        unsigned int r,g,b,a;
        FPDFAnnot_GetColor(annot, FPDFANNOT_COLORTYPE_Color, &r, &g, &b, &a);

        printAnnotationStringValue(annot, kContents);
        printAnnotationStringValue(annot, kIReaderNoteID);
        printAnnotationStringValue(annot, kIReaderNoteLastModifiedTime);
        printAnnotationStringValue(annot, kIReaderNoteRange);
//        unsigned long len = FPDFAnnot_GetStringValue(annot, kContents, nullptr, 0);
//        std::vector<FPDF_WCHAR> buf = GetFPDFWideStringBuffer(len);
//        FPDFAnnot_GetStringValue(annot, kContents, buf.data(), len);
//        std::string text;
//        GetPlatformString(buf.data(), text);

//        FPDF_OBJECT_TYPE uuidType = FPDFAnnot_GetValueType(annot, kIReaderNoteID);
//        printf("uuid value type: %s\n", getTypeStr(uuidType).c_str());
//        FPDF_OBJECT_TYPE missingType = FPDFAnnot_GetValueType(annot, "test");
//        printf("test value type: %s\n", getTypeStr(missingType).c_str());
//        FPDF_OBJECT_TYPE timestampType = FPDFAnnot_GetValueType(annot, kIReaderNoteLastModifiedTime);
//        printf("lastmodifiedtime type: %s\n", getTypeStr(timestampType).c_str());
//        FPDF_OBJECT_TYPE rangeType = FPDFAnnot_GetValueType(annot, kIReaderNoteRange);
//        printf("range type:%s\n", getTypeStr(rangeType).c_str());
//        if(FPDFAnnot_HasKey(annot, kIReaderNoteLastModifiedTime) && timestampType == FPDF_OBJECT_STRING) {
//            len = FPDFAnnot_GetStringValue(annot, kIReaderNoteLastModifiedTime, nullptr, 0);
//            std::vector<FPDF_WCHAR> buf1 = GetFPDFWideStringBuffer(len);
//            FPDFAnnot_GetStringValue(annot, kIReaderNoteLastModifiedTime, buf1.data(), len);
//            std::wstring timestamp = GetPlatformWString(buf1.data());
//            printf("lastmodifiedtime equals:%d\n", wcscmp(timestamp.c_str(), kTestTimeStamp));
//        }


        int objCount = FPDFAnnot_GetObjectCount(annot);
        printf("annot color: %d,%d,%d,%d\n", r, g,b,a);
        printf("annot %d, subtype(%s), flag:%d, ap:%s, rect(%f, %f, %f, %f), objCount:%d\n", i, AnnotSubtypeToCString(subtype), flags, ap.c_str(), rect.left, rect.top, rect.right, rect.bottom, objCount);
        size_t quadpointsCount = FPDFAnnot_CountAttachmentPoints(annot);
        if(quadpointsCount > 0) {
            for(int m=0;m<quadpointsCount;m++) {
                FS_QUADPOINTSF quadpoint;
                if(FPDFAnnot_GetAttachmentPoints(annot, m, &quadpoint)) {
                    FS_RECTF tmpR;
                    FS_RECTF_init(tmpR);
                    convertQuadPoinTORect(quadpoint, tmpR);
                    printf("analyze quadpoint : %d\n", m);
                    printf("quadpoint :%d (%f,%f,%f,%f,%f,%f,%f,%f)\n", m, quadpoint.x1,quadpoint.y1, quadpoint.x2, quadpoint.y2, quadpoint.x3, quadpoint.y3, quadpoint.x4, quadpoint.y4);
                    printf("convert quadpoint 2 rectf:(%f, %f, %f, %f)\n", tmpR.left, tmpR.top, tmpR.right, tmpR.bottom);
                    getBoundedText(text_page, tmpR);
                }
            }
        }
        printf("\n\n");

    }

}

void PDFDocument::analyzeTextObject(int index, FPDF_PAGE page)
{
    ScopedFPDFTextPage text_page = ScopedFPDFTextPage(FPDFText_LoadPage(page));
    int charSize = FPDFText_CountChars(text_page.get());
    for(int i=0;i<charSize;i++) {
        unsigned short buf[2];
        FPDFText_GetText(text_page.get(),i, 1, buf);
        std::string text;
        GetPlatformString(buf, text);
        double l, t, r, b;
        FPDFText_GetCharBox(text_page.get(), i, &l, &r, &b, &t);
        printf("char at %d: content :%s, rect(%f,%f,%f,%f)\n", i, text.c_str(), l, t, r, b);
    }
}

void PDFDocument::removeAllAnnotationsByPage(FPDF_PAGE page)
{
    //todo
}
