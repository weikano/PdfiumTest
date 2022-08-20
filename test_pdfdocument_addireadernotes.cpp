#include "test_pdfdocument_addireadernotes.h"

#include <memory>
#include "pdfdocument.h"

class Test
{
public:
    Test() {
        std::cout<<__func__<<std::endl;
    }
    ~Test() {
        std::cout<<__func__<<std::endl;
    }
    void helloexcept() const {
        std::cout<<__func__<<std::endl;
    }
    void hello() const noexcept {
        std::cout<<__func__<<std::endl;
    }
};
using PDFDocumentPtr = std::unique_ptr<std::remove_pointer<PDFDocument>::type>;

static void clear(std::vector<IReaderNote*> &notes)
{
    for(auto* note : notes)
    {
        delete note;
    }
    notes.clear();
}

static void clear(std::vector<const IReaderNote*> &notes)
{
    for(auto* note : notes)
    {
        delete note;
    }
    notes.clear();
}


void testAddIReaderNotes()
{

//    std::unique_ptr<Test> ptr(new Test);
//    auto tp = ptr.get();
//    tp->hello();

//    const char* path = "/home/roman/git/pdfium/testing/resources/hello_world.pdf";
    const char* path = "/home/roman/文档/PDF32000_2008.pdf";
    const char* output = "/home/roman/文档/pdf_test.pdf";
    PDFDocumentPtr pdoc(new PDFDocument(path, nullptr));
    pdoc.get()->setSavePath(output);
//    pdoc.get()->addAnnotations(2,5,"hahaha");

    std::vector<IReaderNote*> notes;
    notes.push_back(new IReaderNote(0,1, "uuid_0_1", "content_0_1", 123456L));
    notes.push_back(new IReaderNote(5,6, "uuid_5_6", "content_5_6", 123457L));
    pdoc.get()->addIReaderNotes(0, notes);


    clear(notes);

    PDFDocumentPtr ptest(new PDFDocument(output, nullptr));
    std::vector<const IReaderNote*> exists;
    std::cout<<std::endl;
    std::cout<<"pdf_test.pdf listIReaderAnnotations\n"<<std::endl;
    ptest.get()->listIReaderAnnotations(0, exists);
    clear(exists);
}
