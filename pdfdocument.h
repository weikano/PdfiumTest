#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H
#include <vector>
#include <string>
#include "public/fpdf_doc.h"
#include "public/fpdf_structtree.h"
#include "public/fpdf_annot.h"
#include "public/fpdf_save.h"

#include "ireadernote.h"

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
    void walkAnnotations();
    void walkTextObject();
    void removeAllAnnotations();
    void addAnnotations(int startIndex, int endIndex, const std::string& message);  
    void listLines(int pageIndex, int startIndex, int endIndex, std::vector<FS_RECTF> &lines);
    void listIReaderAnnotations(int pageIndex,  std::vector<const IReaderNote*> &notes);

    //    void listLines(int pageIndex, int startIndex, int endIndex, std::vector<FS_RECTF&> lines);
    void setSavePath(const char *newSavePath);

private:
    const char* mSavePath;

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

    void analyzeAnnotations(int index, FPDF_PAGE page, FPDF_TEXTPAGE text_page);
    /**
     * analyze text object
     * @brief analyzeTextObject
     * @param index
     * @param page
     */
    void analyzeTextObject(int index, FPDF_PAGE page);
    void removeAllAnnotationsByPage(FPDF_PAGE page);   
    static const char* AnnotSubtypeToCString(FPDF_ANNOTATION_SUBTYPE subtype) {
      if (subtype == FPDF_ANNOT_TEXT)
        return "Text";
      if (subtype == FPDF_ANNOT_LINK)
        return "Link";
      if (subtype == FPDF_ANNOT_FREETEXT)
        return "FreeText";
      if (subtype == FPDF_ANNOT_LINE)
        return "Line";
      if (subtype == FPDF_ANNOT_SQUARE)
        return "Square";
      if (subtype == FPDF_ANNOT_CIRCLE)
        return "Circle";
      if (subtype == FPDF_ANNOT_POLYGON)
        return "Polygon";
      if (subtype == FPDF_ANNOT_POLYLINE)
        return "PolyLine";
      if (subtype == FPDF_ANNOT_HIGHLIGHT)
        return "Highlight";
      if (subtype == FPDF_ANNOT_UNDERLINE)
        return "Underline";
      if (subtype == FPDF_ANNOT_SQUIGGLY)
        return "Squiggly";
      if (subtype == FPDF_ANNOT_STRIKEOUT)
        return "StrikeOut";
      if (subtype == FPDF_ANNOT_STAMP)
        return "Stamp";
      if (subtype == FPDF_ANNOT_CARET)
        return "Caret";
      if (subtype == FPDF_ANNOT_INK)
        return "Ink";
      if (subtype == FPDF_ANNOT_POPUP)
        return "Popup";
      if (subtype == FPDF_ANNOT_FILEATTACHMENT)
        return "FileAttachment";
      if (subtype == FPDF_ANNOT_SOUND)
        return "Sound";
      if (subtype == FPDF_ANNOT_MOVIE)
        return "Movie";
      if (subtype == FPDF_ANNOT_WIDGET)
        return "Widget";
      if (subtype == FPDF_ANNOT_SCREEN)
        return "Screen";
      if (subtype == FPDF_ANNOT_PRINTERMARK)
        return "PrinterMark";
      if (subtype == FPDF_ANNOT_TRAPNET)
        return "TrapNet";
      if (subtype == FPDF_ANNOT_WATERMARK)
        return "Watermark";
      if (subtype == FPDF_ANNOT_THREED)
        return "3D";
      if (subtype == FPDF_ANNOT_RICHMEDIA)
        return "RichMedia";
      if (subtype == FPDF_ANNOT_XFAWIDGET)
        return "XFAWidget";
      return "";
    }
};

#endif // PDFDOCUMENT_H
