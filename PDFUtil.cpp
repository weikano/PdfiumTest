#include "PDFUtil.h"

#include "string.h"
#include "public/fpdf_text.h"


using std::string;
namespace PDFUtil {
    int ucs2_char_to_utf8_len(unsigned short ucs2)
    {
        if (ucs2 < 0x80)
        {
            return 1;
        }
        else if (ucs2 < 0x800)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }

    int ucs2_char_to_utf8(char *utf8, unsigned short ucs2) {
        int count;

        if (ucs2 < 0x80)
        {
            count = 1;
        }
        else if (ucs2 < 0x800)
        {
            count = 2;
        }
        else
        {
            count = 3;
        }
        switch (count)
        {
            case 3:
                utf8[2] = 0x80 | (ucs2 & 0x3f);
                ucs2 = ucs2 >> 6;
                ucs2 |= 0x800;
            case 2:
                utf8[1] = 0x80 | (ucs2 & 0x3f);
                ucs2 = ucs2 >> 6;
                ucs2 |= 0xc0;
            case 1:
                utf8[0] = (char) ucs2;
        }
        return count;
    }

    void escapeHtml(const string &input, string &out) {
        out.clear();
        for(char c : input) {
            if (c == '<') {
                out.append("&lt;");
            } else if (c == '>') {
                out.append("&gt;");
            } else if (c == '&') {
                out.append("&amp;");
            } else if (c == '"') {
                out.append("&quot;");
            } else if (c == '\'') {
                out.append("&apos;");
            } else {
                out.append(1,c);
            }
        }
    }

    void ucs2_chars_to_string(std::string &out, int len, unsigned short *inputs) {
        for(int i=0;i<len;i++) {
            char utf[4];
            int utflen = ucs2_char_to_utf8(utf, inputs[i]);
            utf[utflen] = 0;
            std::string span;
            std::string input(utf);
            escapeHtml(input, span);
            out.append(span);
        }
    }

    void getPdfDebugText(FPDF_TEXTPAGE text_page,int start_index,int count, std::string &outUtfStr, bool printHex) {
        unsigned short *ch = new unsigned short[count + 1];
        FPDFText_GetText(text_page, start_index, count, ch);
        if (printHex) {
            for (int i = 0; i < count; i++) {
                char buff[8];
                sprintf(buff, "\\%02X", ch[0]);
                outUtfStr.append(buff);
                if (ch[i] == '\n') {
                    ch[i] = 'N';
                } else if (ch[i] == '\r') {
                    ch[i] = 'R';
                }
            }
            outUtfStr.append("[");
        }
        PDFUtil::ucs2_chars_to_string(outUtfStr, count, ch);
        if (printHex) {
            outUtfStr.append("]");
        }
        delete []ch;
    }

    bool matrixShouldIgnore(const float *matrix) {
        return matrixEquals(matrix, kDefaultMatrix);
    }

    bool matrixEquals(const float *m1, const float *m2) {
        for(int i=0 ;i<6;i++) {
            if(m1[i] != m2[i]) {
                return false;
            }
        }
        return true;
    }

    int utf8_length(const char *str, int len) {
        const char *last = str + len;
        int counter = 0;
        while (str < last)
        {
            if ((*str & 0x80) == 0)
            {
                ++str;
            }
            else if ((*str & 0x20) == 0)
            {
                str += 2;
            }
            else if ((*str & 0x10) == 0)
            {
                str += 3;
            }
            else
            {
                str += 4;
            }
            ++counter;
        }
        return counter;
    }

    static int utf8_char_len(const char str) {
        if ((str & 0x80) == 0)
        {
            return 1;
        }
        else if ((str & 0x20) == 0)
        {
            return 2;
        }
        else if ((str & 0x10) == 0)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }

    int utf8_char_at(const char *str, int strlen, int index, char *out) {
        const char* last = str+strlen;
        int counter = 0;
        while(counter != index && str<last) {
            str += utf8_char_len(*str);
            counter ++;
        }
        int len = utf8_char_len(*str);
        strncpy(out, str, len);
        return len;
    }

    int utf8_char_to_ucs2_ex(unsigned short *ucs2, const char *utf8, int utf8Len)
    {
        const unsigned char *ch = (const unsigned char *)utf8;
        if (ch[0] < 0x80)
        {
            *ucs2 = ch[0];
            if(utf8Len < 1)
            {
                return 0;
            }
            return 1;
        }
        else if (ch[0] < 0xe0)
        {
            if(utf8Len < 2)
            {
                return 0;
            }
            *ucs2 = ((unsigned short) (ch[0] & 0x1f) << 6) | (unsigned short) (ch[1] ^ 0x80);
            return 2;
        }
        else
        {
            if(utf8Len < 3)
            {
                return 0;
            }
            *ucs2 = ((unsigned short) (ch[0] & 0x0f) << 12) | ((unsigned short) (ch[1] ^ 0x80) << 6) | (unsigned short) (ch[2] ^ 0x80);
            return 3;
        }
    }

    int utf8_to_ucs2(unsigned short *dst, int dstLen, const char *src, int srcLen)
    {
        int i;
        int ucs2Count;
        int utf8Count;
        if(srcLen == -1)
        {
            srcLen = strlen(src);
        }
        ucs2Count = 0;
        i = 0;
        while(i < srcLen && ucs2Count + 1< dstLen)
        {
            utf8Count = utf8_char_to_ucs2_ex(&dst[ucs2Count],src + i, srcLen - i);
            if (utf8Count == 0) {
                break;
            }
            i += utf8Count;
            ++ucs2Count;
        }
        dst[ucs2Count] = 0;
        return ucs2Count;
    }
};
