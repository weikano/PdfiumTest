#ifndef IREADERNOTE_H
#define IREADERNOTE_H

#include <string>
#include <atomic>
#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

#include "public/fpdf_annot.h"
class IReaderNote
{
public:
    enum IReaderNoteMergeResult {
        Merged, Ignore
    };
    IReaderNote();
    ~IReaderNote();
    IReaderNote(int annotIndex);
    IReaderNote(int startIndex, int endIndex, const std::string &uuid, const std::string &content, long timestamp);
    void loadData(FPDF_PAGE page);
    int startIndex() const;

    int endIndex() const;

    const std::string &uuid() const;
    const std::string &content() const;

    long timestamp() const;

    int annotIndex() const;

    IReaderNoteMergeResult merge(const IReaderNote &other);

    friend std::ostream& operator<<(std::ostream &out, const IReaderNote & note)
    {
        out<<"start:"<<note._startIndex<<", end:"<<note._endIndex<<", timestamp:"<<note._timestamp<<", uuid:"<<note._uuid<<std::endl;
        return out;
    }

    using IReaderNoteList = std::vector<IReaderNote*>;
    struct IReaderNoteListDeleter {
        inline void operator()(IReaderNoteList* list) {
            for(const auto* item:*list)
            {
                delete item;
            }
            list->clear();
        }
    };
    using ScopedIReaderNoteList = std::unique_ptr<std::remove_pointer<IReaderNoteList>::type, IReaderNoteListDeleter>;

private:
    bool _status = false;
    std::atomic_bool _inited;
    int _annotIndex = 0;
    int _startIndex = 0;
    int _endIndex = 0;
    std::string _uuid = "";
    std::string _content = "";
    long _timestamp = 0L;
};

#endif // IREADERNOTE_H
