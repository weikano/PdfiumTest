
#include "test_ireadnote.h"
#include "test_pdfdocument.h"
//#include "test_pdfdocument_addireadernotes.h"
//#include "test_dir.h"
#include "test_pdfdocument_removeannotations.h"

int main(int argc, char *argv[])
{

    FPDF_InitLibrary();
    testRemoveIreaderNote();
//    test_dir();
//    test_ireader_ctor();
//    test_scopeireadernotelist();
//    testAddIReaderNotes();
//    testMergeIReaderNotes();
//    test_ireadernote_merge();

    FPDF_DestroyLibrary();
    return 0;
}
