
#include "test_ireadnote.h"
#include "test_pdfdocument.h"
#include "test_pdfdocument_addireadernotes.h"
int main(int argc, char *argv[])
{
    FPDF_InitLibrary();
//    test_scopeireadernotelist();
    testAddIReaderNotes();
//    test_ireadernote_merge();
    FPDF_DestroyLibrary();
    return 0;
}
