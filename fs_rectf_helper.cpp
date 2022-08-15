#include "fs_rectf_helper.h"

#include <algorithm>

bool FS_RECTF_unset(const FS_RECTF &in) {
    return in.left == 0 && in.top == 0 && in.bottom == 0 && in.right == 0;
}

void FS_RECTF_union(FS_RECTF &in, FS_RECTF *target) {
    if(FS_RECTF_unset(in)) {
        in.left = target->left;
        in.top = target->top;
        in.right = target->right;
        in.bottom = target->bottom;
    }else {
        in.left = std::min(in.left, target->left);
        in.right = std::max(in.right, target->right);
        in.top = std::max(in.top, target->top);
        in.bottom = std::max(in.bottom, target->bottom);
    }
}

void FS_RECTF_init(FS_RECTF &rect)
{
    rect.left = 0;
    rect.top = 0;
    rect.right = 0;
    rect.bottom = 0;
}

void FS_RECTF_union(FS_RECTF &in, double l, double t, double r, double b) {
    if(l == r || t == b) {
        return;
    }
    if(FS_RECTF_unset(in)) {
        in.left = l;
        in.top = t;
        in.right = r;
        in.bottom = b;
    }else {
        in.left = std::min(in.left, (float)l);
        in.right = std::max(in.right, (float)r);
        in.top = std::max(in.top, (float)t);
        in.bottom = std::min(in.bottom, (float)b);
    }
}

static bool FS_RECT_insect_vertical(const FS_RECTF &charbox, const FS_RECTF &linebox)
{
    return !(charbox.bottom > linebox.top || charbox.top < linebox.bottom);
}

void FS_RECTF_by_line(FS_RECTF &charbox, std::vector<FS_RECTF> &lines)
{
//    auto target = std::find_if(lines.begin(), lines.end(), [charbox](const FS_RECTF& line) {
//        return FS_RECT_insect_vertical(charbox, line);
//    });
//    if(target != lines.end()) {
//        FS_RECTF_union(*target, charbox);
//    }else {
//        lines.push_back(charbox);
//    }
}

void FS_RECTF_set(FS_RECTF &target, double l, double r, double b, double t)
{
    target.left = l;
    target.right =r;
    target.bottom = b;
    target.top = t;
}
