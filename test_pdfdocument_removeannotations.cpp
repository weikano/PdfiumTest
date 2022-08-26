#include "test_pdfdocument_removeannotations.h"
#include "pdfdocument.h"
void testRemoveIreaderNote()
{
//    const char* source= "/home/weikano/下载/Listening and Speaking Textbook 2021-划线自动变长.pdf";
    const char* source = "/home/weikano/下载/Listening and Speaking Textbook 2021-划线自动变长_20220826 (1).pdf";
    const char* output = "/home/weikano/下载/remove_annot.pdf";
    const char* output1 = "/home/weikano/下载/remove_annot1.pdf";

    PDFDocument doc(source, nullptr);
    doc.setSavePath(output);
    doc.walkAnnotationsByPageIndex(2);
//    IReaderNote note(805, /*982*/810, "751-1-3-131.267990-359.8299871-1-3-200.184006-313.041992", "假期我而与iokjgfdsse", -667274741);
//    std::vector<IReaderNote*> notes;
//    notes.push_back(&note);
//    doc.addIReaderNotes(3,  notes);
    doc.removeAnnot(2, 0);

    std::cout<<"========out_doc============="<<std::endl;

    PDFDocument out_doc(output, nullptr);
    out_doc.setSavePath(output1);
    out_doc.walkAnnotationsByPageIndex(3);
//    out_doc.removeAnnot(3, 0);

//     std::cout<<"========out_doc1============="<<std::endl;
//    PDFDocument out_doc1(output1, nullptr);
//    out_doc1.walkAnnotationsByPageIndex(3);

}
