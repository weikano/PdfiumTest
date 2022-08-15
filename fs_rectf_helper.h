#ifndef FS_RECTF_HELPER_H
#define FS_RECTF_HELPER_H


#include <vector>

#include "public/fpdfview.h"

bool FS_RECTF_unset(const FS_RECTF &in);
void FS_RECTF_union(FS_RECTF &in, FS_RECTF *target);
void FS_RECTF_init(FS_RECTF &rect);
void FS_RECTF_union(FS_RECTF &in, double l, double t, double r, double b);
void FS_RECTF_by_line(FS_RECTF &charbox, std::vector<FS_RECTF> &lines);
void FS_RECTF_set(FS_RECTF &target, double l, double r, double b, double t);

#endif // FS_RECTF_HELPER_H
