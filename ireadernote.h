#ifndef IREADERNOTE_H
#define IREADERNOTE_H

#include <string>
#include <atomic>
#include <iostream>
#include "public/fpdf_annot.h"
class IReaderNote
{
public:
    enum IReaderNoteMergeResult {
        Merged, Ignore
    };

    IReaderNote(int annotIndex);
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

private:
    bool _status = false;
    std::atomic_bool _inited;
    int _annotIndex;
    int _startIndex;
    int _endIndex;
    std::string _uuid;
    std::string _content;
    long _timestamp;
};

#endif // IREADERNOTE_H
