#include "filewriterimpl.h"

FileWriterImpl::FileWriterImpl()
{
    FPDF_FILEWRITE::version = 1;
    FPDF_FILEWRITE::WriteBlock = WriteBLockCallback;
}

FileWriterImpl::~FileWriterImpl() = default;

void FileWriterImpl::OpenPDFFileForWrite(const std::string &filename)
{
    filestream_.open(filename, std::ios_base::binary);
}

void FileWriterImpl::ClosePDFFileForWrite()
{
    filestream_.close();
}

int FileWriterImpl::WriteBLockCallback(FPDF_FILEWRITE *pFileWriter, const void *data, unsigned long size)
{
    FileWriterImpl* pThis = static_cast<FileWriterImpl *>(pFileWriter);
    if(pThis->filestream_.is_open()) {
        pThis->filestream_.write(static_cast<const char*>(data), size);
    }
    return 1;
}
