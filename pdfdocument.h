#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H
#include <vector>
#include "public/fpdf_doc.h"
#include "public/fpdf_structtree.h"
class PDFDocument
{
public:
    PDFDocument(const char *path, const char *pwd);
    ~PDFDocument();
    int getPageCount();
    void walkStructtree();
    FPDF_PAGE loadPage(int index);
    void ClosePage(FPDF_PAGE page);
    void walkMarkObject();
private:
    FPDF_DOCUMENT mDoc = nullptr;
    void treeWalking(FPDF_STRUCTTREE root);
    void getStructElements(FPDF_PAGE page, std::vector<FPDF_STRUCTELEMENT> &out);
    void recursiveTreeElement(FPDF_STRUCTELEMENT element, std::vector<FPDF_STRUCTELEMENT>& elements);
    void printStructElement(FPDF_STRUCTELEMENT elem);
    void showPageObjectMark(FPDF_PAGEOBJECTMARK mark);

    void printFunction(const char* func, bool begin=true);
    /**
     * @brief analyzeFPDFTextObj
     * @param page
     * @param text_obj
     */
    void analyzeFPDFTextObj(FPDF_PAGE page, FPDF_PAGEOBJECT text_obj);
};

#endif // PDFDOCUMENT_H
