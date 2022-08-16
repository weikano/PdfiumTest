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

void testAddIReaderNotes()
{

//    std::unique_ptr<Test> ptr(new Test);
//    auto tp = ptr.get();
//    tp->hello();

    const char* path = "/home/weikano/文档/PDF32000_2008.pdf";
    PDFDocument doc(path, "");
//    std::unique_ptr<PDFDocument> mDoc(new PDFDocument(doc, nullptr));
//    mDoc->walkAnnotations();
}
