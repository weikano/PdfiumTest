#include "ireadernote.h"
#include "public/cpp/fpdf_scopers.h"

#include "annot_const.h"
IReaderNote::IReaderNote()
{
    std::cout<<__func__<<std::endl;
}

IReaderNote::~IReaderNote()
{
    std::cout<<__func__<<this<<std::endl;
}

IReaderNote::IReaderNote(int annotIndex):_annotIndex(annotIndex)
{
    _inited.store(false);
}

void IReaderNote::loadData(FPDF_PAGE page)
{    
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
    if(_startIndex > other.endIndex() || _endIndex < other.startIndex()) {
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


IReaderNote::IReaderNote(int startIndex, int endIndex, const std::string &uuid, const std::string &content, long timestamp) : _startIndex(startIndex),
    _endIndex(endIndex),
    _uuid(uuid),
    _content(content),
    _timestamp(timestamp)
{}

IReaderNote::IReaderNote(const IReaderNote &note):
    _annotIndex(note.annotIndex()),
    _startIndex(note.startIndex()),
    _endIndex(note.endIndex()),
    _uuid(note.uuid()),
    _content(note.content()),
    _timestamp(note.timestamp())
{
    std::cout<<"copy " <<&note<<" to "<<this<<std::endl;
}

IReaderNote::IReaderNote(const IReaderNote &&note) noexcept:
_annotIndex(note.annotIndex()),
_startIndex(note.startIndex()),
_endIndex(note.endIndex()),
_uuid(note.uuid()),
_content(note.content()),
_timestamp(note.timestamp())
{
std::cout<<"move:"<<&note<<" to " <<this<<std::endl;
}
