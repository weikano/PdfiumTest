#ifndef BUFFEREDBLOCKWRITER_H
#define BUFFEREDBLOCKWRITER_H

#include "public/fpdf_save.h"

struct PdfToFdWRiter: FPDF_FILEWRITE {
    int dstFd;
};

void flushBuffer(int fd);
bool writeAllBytes(const int fd, const void*buffer, const size_t byteCount);
int writeBlock(FPDF_FILEWRITE *owner, const void* buffer, unsigned long size);
int writeBlockBuffered(FPDF_FILEWRITE *owner, const void* buffer, unsigned long size);
void startBufferedWriting(size_t buffer_size);

#endif // BUFFEREDBLOCKWRITER_H
