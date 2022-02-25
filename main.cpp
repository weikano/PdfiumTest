
#include <codecvt>
#include <locale>
#include <iostream>
#include <string.h>

#include "public/fpdf_doc.h"
#include "public/fpdfview.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_text.h"

#include "pdfdocument.h"
#include "fpdf_stringhelper.h"

int main(int argc, char *argv[])
{
    const char *path = "/home/weikano/文档/需要的文件/书籍/TaggedPDFBestPracticeGuideSyntax.pdf";
    FPDF_InitLibrary();

    PDFDocument *doc = new PDFDocument(path, NULL);
//    printf("pageCount(%d)\n", doc->getPageCount());
//    FPDF_PAGE page = doc->loadPage(0);
//    float width = FPDF_GetPageWidthF(page);
//    float height = FPDF_GetPageHeightF(page);
//    printf("width(%f), height(%f)\n", width, height);
//    FPDF_TEXTPAGE textpage = FPDFText_LoadPage(page);
//    int count = FPDFText_CountChars(textpage);
//    printf("char count(%d)\n", count)           ;
//    unsigned short buf[count+1]; //+1 for terminator
//    memset(buf, 0, sizeof(buf));
//    FPDFText_GetText(textpage, 0, count, buf);
//    std::wstring wstr = GetPlatformWString(buf);
//    std::wcout<<"content:"<<wstr<<std::endl;
//    FPDFText_ClosePage(textpage);
//    doc->ClosePage(page);
//    doc->walkStructtree();
    doc->walkMarkObject();
    delete doc;

    FPDF_DestroyLibrary();
//    std::wstring wstr = L"/home/weikano/文档/需要的文件/书籍/TaggedPDFBestPracticeGuideSyntax.pdf";
//    using convert_type = std::codecvt_utf8<wchar_t>;
//    std::wstring_convert<convert_type, wchar_t> converter;
//    std::string converted = converter.to_bytes(wstr.c_str());
//    qDebug("converted %s", converted.c_str());

//    std::wstring wstr2 = L"ħëłlö";
//    std::string conv2 = converter.to_bytes(wstr2.c_str());
//    qDebug("conv2 %s", conv2.c_str());

//    std::string conv2(wstr.begin(), wstr.end()); //chinese not work
//    qDebug("converted2 %s", conv2.c_str());
//    auto qstr = QString::fromWCharArray(wstr.c_str());
//    auto str = qstr.toStdString();
//    qDebug().nospace().noquote()<<qstr;
//    qDebug(str.c_str());
    return 0;
}
