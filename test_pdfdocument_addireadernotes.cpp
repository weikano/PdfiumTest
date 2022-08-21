#include "test_pdfdocument_addireadernotes.h"

#include <memory>
#include "pdfdocument.h"

#include "dir.h"

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
    std::string path = homedir;
    path.append("/文档/PDF32000_2008.pdf");
    std::string output = homedir;
    output.append("/文档/pdf_test.pdf");
//    const char* path = homedir"/home/roman/文档/PDF32000_2008.pdf";
//    const char* output = "/home/roman/文档/pdf_test.pdf";
    PDFDocumentPtr pdoc(new PDFDocument(path.c_str(), nullptr));
    pdoc.get()->setSavePath(output.c_str());
//    pdoc.get()->addAnnotations(2,5,"hahaha");

    std::vector<IReaderNote*> notes;
    notes.push_back(new IReaderNote(0,1, "uuid_0_1", "content_0_1", 123456L));
    notes.push_back(new IReaderNote(5,6, "uuid_5_6", "content_5_6", 123457L));
    pdoc.get()->addIReaderNotes(0, notes);


    clear(notes);   
}

void testMergeIReaderNotes()
{
    std::string save = homedir;
    save.append("/文档/pdf_test1.pdf");
    std::string output = homedir;
    output.append("/文档/pdf_test.pdf");
    PDFDocumentPtr doc(new PDFDocument(output.c_str(), nullptr));
    doc->setSavePath(save.c_str());
    std::vector<IReaderNote> exists;
    doc.get()->listIReaderAnnotations(0, exists);
    std::cout<<"==========already exists ireadernotes=========="<<std::endl;
    for(const auto &note:exists)
    {
        std::cout<<note;
    }
    exists.clear();
    std::cout<<"==========try override ireadernotes=========="<<std::endl;
    std::vector<IReaderNote*> override;

    override.push_back(new IReaderNote(0,3,"uuid_0_3","content_0_3", 123457));
    override.push_back(new IReaderNote(4,10,"uuid_4_10","content_4_10", 1234578));
    override.push_back(new IReaderNote(12,20,"uuid_12_20","content_12_20", 1234578));
    override.push_back(new IReaderNote(23,30,"uuid_30_50","content_30_50", 1234578));
    override.push_back(new IReaderNote(40,45,"uuid_30_50","content_30_50", 1234578));
    override.push_back(new IReaderNote(60,65,"uuid_30_50","content_30_50", 1234578));

    override.push_back(new IReaderNote(80,95,"uuid_30_50","content_30_50", 1234578));
    doc->addIReaderNotes(0, override);

    for(const auto* note:override)
    {
        delete note;
    }
    override.clear();

    std::cout<<"==========after override=========="<<std::endl;
    PDFDocument testdoc(save.c_str(), nullptr);
//    PDFDocumentPtr test(new PDFDocument(save.c_str(), nullptr));
    testdoc.listIReaderAnnotations(0, exists);
    for(const auto &note:exists)
    {
        std::cout<<note;
    }
    exists.clear();

}
