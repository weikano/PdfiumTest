#include <string>

#include "public/fpdf_text.h"
using std::string;
namespace PDFUtil {
    constexpr float kDefaultMatrix[6] = {1, 0, 0, 1, 0, 0};
    int ucs2_char_to_utf8_len(unsigned short ucs2);
    int ucs2_char_to_utf8(char *utf8, unsigned short ucs2);
    void escapeHtml(const string &input, string &out);

    void ucs2_chars_to_string(std::string& out, int len, unsigned short *inputs);

    void getPdfDebugText(FPDF_TEXTPAGE text_page,int start_index,int count, std::string &outUtfStr, bool printHex=true);

    bool matrixShouldIgnore(const float *matrix);

    bool matrixEquals(const float *m1, const float *m2);

    int utf8_length(const char* str, int len);
    /**
     * utf8字符串中第几个字符
     * @param str
     * @param index
     * @param out
     * @return
     */
    int utf8_char_at(const char* str, int len, int index, char * out);
    /**
     * utf8字符串转unicode字符串
     * @param dst
     * @param dstLen
     * @param src
     * @param srcLen
     * @return
     */
    int utf8_to_ucs2(unsigned short *dst, int dstLen, const char *src, int srcLen);

    int utf8_char_to_ucs2_ex(unsigned short *ucs2, const char *utf8,int utf8Len);
};
