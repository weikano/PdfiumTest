#include "ireadernote.h"
#include "public/cpp/fpdf_scopers.h"

#include "annot_const.h"
IReaderNote::IReaderNote(int annotIndex):_annotIndex(annotIndex)
{
    _inited.store(false);
}

void IReaderNote::loadData(FPDF_PAGE page)
{
    std::cout<<__func__<<std::endl;
    if(_inited.compare_exchange_strong(_status, true)) {
        ScopedFPDFAnnotation annot = ScopedFPDFAnnotation(FPDFPage_GetAnnot(page, _annotIndex));
        //get ireadernote id
        _uuid.clear();
        FPDFAnnotation_GetStringValueImpl(annot.get(), kIReaderNoteID, _uuid);

        std::string range;
        FPDFAnnotation_GetStringValueImpl(annot.get(), kIReaderNoteRange, range);
        if(!range.empty()) {
            sscanf(range.c_str(), "%d_%d", &_startIndex, &_endIndex);
        }
        std::string timestampstr;
        FPDFAnnotation_GetStringValueImpl(annot.get(), kIReaderNoteLastModifiedTime, timestampstr);
        if(!timestampstr.empty()) {
            sscanf(timestampstr.c_str(), "%ld", &_timestamp);
        }
        _content.clear();
        FPDFAnnotation_GetStringValueImpl(annot.get(), kContents, _content);
    }

    //    if(_inited.compare_exchange_strong(false, true))
}

int IReaderNote::startIndex() const
{
    return _startIndex;
}


int IReaderNote::endIndex() const
{
    return _endIndex;
}


const std::string &IReaderNote::uuid() const
{
    return _uuid;
}

const std::string &IReaderNote::content() const
{
    return _content;
}

long IReaderNote::timestamp() const
{
    return _timestamp;
}

int IReaderNote::annotIndex() const
{
    return _annotIndex;
}

IReaderNote::IReaderNoteMergeResult IReaderNote::merge(const IReaderNote &other)
{
    if(_startIndex < other.startIndex() && _endIndex > other.endIndex()) {
        return Ignore;
    }
    _startIndex = std::min(_startIndex, other.startIndex());
    _endIndex = std::max(_endIndex, other.endIndex());
    if(!other.content().empty()) {
        if(!_content.empty()) {
            _content.append("\n");
        }
        _content.append(other.content());
    }
    _timestamp = std::max(_timestamp, other.timestamp());
    return Merged;
}
