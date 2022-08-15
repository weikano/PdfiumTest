#ifndef FILEWRITERIMPL_H
#define FILEWRITERIMPL_H

#include "public/fpdf_save.h"

#include <fstream>
#include <string>

class FileWriterImpl : public FPDF_FILEWRITE
{
public:
    FileWriterImpl();
    ~FileWriterImpl();
    void OpenPDFFileForWrite(const std::string &filename);
    void ClosePDFFileForWrite();
private:
    static int WriteBLockCallback(FPDF_FILEWRITE *pFileWriter, const void*data, unsigned long size);
private:
    std::ofstream filestream_;
};

#endif // FILEWRITERIMPL_H
