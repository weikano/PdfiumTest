#ifndef SCOPERS_H
#define SCOPERS_H
#include <memory>
#include <type_traits>

#include "fpdf_structtree.h"
#include "fpdf_deleters.h"
using ScopedFPDFStructTree =
    std::unique_ptr<std::remove_pointer<FPDF_STRUCTTREE>::type,
                    FPDFStructTreeDeleter>;
using ScopedFPDFPage =
    std::unique_ptr<std::remove_pointer<FPDF_PAGE>::type, FPDFPageDeleter>;

#endif // SCOPERS_H
