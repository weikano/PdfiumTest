#include "test_ireadnote.h"

#include <cassert>
void test_ireadernote_merge()
{
    IReaderNote *note = new IReaderNote(2,5, "uuid_2_5", "content_2_5", 123456L);
    IReaderNote *ignore = new IReaderNote(0,1, "uud_0_1", "content_o_1", 1111);
    IReaderNote *merged = new IReaderNote(3,10, "uuid_3_5", "content_3_5", 333355555);

    assert (note->merge(*ignore) == IReaderNote::IReaderNoteMergeResult::Ignore);
    assert (note->merge(*merged) == IReaderNote::IReaderNoteMergeResult::Merged);

    assert (note->startIndex() == 2);
    assert (note->endIndex() == 10);
    assert (note->uuid() == "uuid_2_5");
    assert (note->content() == "content_2_5\ncontent_3_5");
    assert (note->timestamp() == std::max(123456L, merged->timestamp()));
}

void test_scopeireadernotelist()
{
    IReaderNote::ScopedIReaderNoteList list(new std::vector<IReaderNote*>);
    list.get()->push_back(new IReaderNote);
    list.get()->push_back(new IReaderNote);
}
