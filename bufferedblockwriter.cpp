#include "bufferedblockwriter.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>

bool writeAllBytes(const int fd, const void *buffer, const size_t byteCount)
{
    char *writeBuffer = static_cast<char*>(const_cast<void *>(buffer));
    size_t remainingBytes = byteCount;
    while(remainingBytes > 0) {
        size_t writtenByteCount = write(fd, writeBuffer, remainingBytes);
        if(writtenByteCount == -1) {
            if(errno == EINTR) {

            }
        }
    }
}
