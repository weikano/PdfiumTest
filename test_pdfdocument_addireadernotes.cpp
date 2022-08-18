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

static void freeIReaderNoteVector(std::vector<IReaderNote*> &notes)
{
    for(auto* note: notes)
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

//    const char* path = "/home/weikano/git/pdfium/testing/resources/hello_world.pdf";
        //ADD NOTES TO PDF32000_2000.pdf and save to pdf_test.pdf
    const char* path = "/home/weikano/文档/PDF32000_2008.pdf";
    const char* output = "/home/weikano/文档/pdf_test.pdf";
    const char* output2 = "/home/weikano/文档/pdf_test1.pdf";
//    PDFDocumentPtr pdoc(new PDFDocument(path, nullptr));
//    pdoc.get()->setSavePath(output);
////    pdoc.get()->addAnnotations(2,5,"hahaha");

//    std::vector<IReaderNote*> notes;
//    notes.push_back(new IReaderNote(0,1, "uuid_0_1", "content_0_1", 123456L));
//    notes.push_back(new IReaderNote(5,6, "uuid_5_6", "content_5_6", 123457L));
//    pdoc.get()->addIReaderNotes(5, notes);

//    freeIReaderNoteVector(notes);

    //try replace notes in pdf_test.pdf and save to pdf_test1.pdf
    std::vector<IReaderNote*> newNotes;
    newNotes.push_back(new IReaderNote(0,3, "uuid_0_3", "content_0_3", 77777777L));
    newNotes.push_back(new IReaderNote(75,85, "uuid_30_37", "content_30_37", 888888888L));
//    PDFDocumentPtr ptestdoc(new PDFDocument(output, nullptr));
    PDFDocument *ptestdoc = new PDFDocument(output, nullptr);
    ptestdoc->setSavePath(output2);
//    ptestdoc->addIReaderNotes(5, newNotes);


    std::string text;
    ptestdoc->getText(1, 85,120, text);
//    std::cout<<"75-85:"<<text<<std::endl;

    freeIReaderNoteVector(newNotes);
    delete ptestdoc;

//    PDFDocumentPtr ptest(new PDFDocument(output2, nullptr));
//    std::vector<const IReaderNote*> exists;
//    std::cout<<std::endl;
//    std::cout<<"pdf_test1.pdf listIReaderAnnotations\n"<<std::endl;
//    ptest.get()->listIReaderAnnotations(0, exists);
//    for(const auto* exist : exists)
//    {
//        std::cout<<*exist<<std::endl;
//        delete exist;
//    }
//    exists.clear();
}
