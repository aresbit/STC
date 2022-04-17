#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stc/utf8.h>
#include <stdbool.h>

static struct CaseFold { uint16_t c0, c1, m1; } casefold[] = {
    {65, 90, 122}, {181, 181, 956}, {192, 214, 246}, {216, 222, 254},
    {256, 302, 303}, {306, 310, 311}, {313, 327, 328}, {330, 374, 375}, {376, 376, 255},
    {377, 381, 382}, {383, 383, 115}, {385, 385, 595}, {386, 388, 389}, {390, 390, 596},
    {391, 391, 392}, {393, 394, 599}, {395, 395, 396}, {398, 398, 477}, {399, 399, 601},
    {400, 400, 603}, {401, 401, 402}, {403, 403, 608}, {404, 404, 611}, {406, 406, 617},
    {407, 407, 616}, {408, 408, 409}, {412, 412, 623}, {413, 413, 626}, {415, 415, 629},
    {416, 420, 421}, {422, 422, 640}, {423, 423, 424}, {425, 425, 643}, {428, 428, 429},
    {430, 430, 648}, {431, 431, 432}, {433, 434, 651}, {435, 437, 438}, {439, 439, 658},
    {440, 442, 443}, {452, 452, 454}, {453, 453, 454}, {455, 455, 457}, {456, 456, 457},
    {458, 458, 460}, {459, 475, 476}, {478, 494, 495}, {497, 497, 499}, {498, 500, 501},
    {502, 502, 405}, {503, 503, 447}, {504, 542, 543}, {544, 544, 414}, {546, 562, 563},
    {570, 570, 11365}, {571, 571, 572}, {573, 573, 410}, {574, 574, 11366}, {577, 577, 578},
    {579, 579, 384}, {580, 580, 649}, {581, 581, 652}, {582, 590, 591}, {837, 837, 953},
    {880, 882, 883}, {886, 886, 887}, {895, 895, 1011}, {902, 902, 940}, {904, 906, 943},
    {908, 908, 972}, {910, 911, 974}, {913, 929, 961}, {931, 939, 971}, {962, 962, 963},
    {975, 975, 983}, {976, 976, 946}, {977, 977, 952}, {981, 981, 966}, {982, 982, 960},
    {984, 1006, 1007}, {1008, 1008, 954}, {1009, 1009, 961}, {1012, 1012, 952}, {1013, 1013, 949},
    {1015, 1015, 1016}, {1017, 1017, 1010}, {1018, 1018, 1019}, {1021, 1023, 893},
    {1024, 1039, 1119}, {1040, 1071, 1103}, {1120, 1152, 1153}, {1162, 1214, 1215},
    {1216, 1216, 1231}, {1217, 1229, 1230}, {1232, 1326, 1327}, {1329, 1366, 1414},
    {4256, 4293, 11557}, {4295, 4296, 11560}, {5112, 5117, 5109}, {7296, 7296, 1074},
    {7297, 7297, 1076}, {7298, 7298, 1086}, {7299, 7300, 1090}, {7301, 7301, 1090},
    {7302, 7302, 1098}, {7303, 7303, 1123}, {7304, 7304, 42571}, {7312, 7354, 4346},
    {7357, 7359, 4351}, {7680, 7828, 7829}, {7835, 7835, 7777}, {7838, 7838, 223},
    {7840, 7934, 7935}, {7944, 7951, 7943}, {7960, 7965, 7957}, {7976, 7983, 7975},
    {7992, 7999, 7991}, {8008, 8013, 8005}, {8025, 8028, 8020}, {8040, 8047, 8039},
    {8072, 8079, 8071}, {8088, 8095, 8087}, {8104, 8111, 8103}, {8120, 8121, 8113},
    {8122, 8123, 8049}, {8124, 8124, 8115}, {8126, 8126, 953}, {8136, 8139, 8053},
    {8140, 8140, 8131}, {8152, 8153, 8145}, {8154, 8155, 8055}, {8168, 8169, 8161},
    {8170, 8171, 8059}, {8172, 8172, 8165}, {8184, 8185, 8057}, {8186, 8187, 8061},
    {8188, 8188, 8179}, {8486, 8486, 969}, {8490, 8490, 107}, {8491, 8491, 229},
    {8498, 8498, 8526}, {8544, 8559, 8575}, {8579, 8579, 8580}, {9398, 9423, 9449},
    {11264, 11311, 11359}, {11360, 11360, 11361}, {11362, 11362, 619}, {11363, 11363, 7549},
    {11364, 11364, 637}, {11367, 11371, 11372}, {11373, 11373, 593}, {11374, 11374, 625},
    {11375, 11375, 592}, {11376, 11376, 594}, {11378, 11380, 11381}, {11390, 11391, 576},
    {11392, 11490, 11491}, {11499, 11501, 11502}, {11506, 11508, 11509}, {42562, 42604, 42605},
    {42624, 42650, 42651}, {42786, 42798, 42799}, {42802, 42862, 42863}, {42873, 42875, 42876},
    {42877, 42877, 7545}, {42878, 42886, 42887}, {42891, 42891, 42892}, {42893, 42893, 613},
    {42896, 42898, 42899}, {42902, 42920, 42921}, {42922, 42922, 614}, {42923, 42923, 604},
    {42924, 42924, 609}, {42925, 42925, 620}, {42926, 42926, 618}, {42928, 42928, 670},
    {42929, 42929, 647}, {42930, 42930, 669}, {42931, 42931, 43859}, {42932, 42946, 42947},
    {42948, 42948, 42900}, {42949, 42949, 642}, {42950, 42950, 7566}, {42951, 42953, 42954},
    {42960, 42962, 42963}, {42968, 42970, 42971}, {43888, 43913, 5049}, {65313, 65338, 65370},
}; // 188
static uint8_t cfold_low[] = {
    0, 138, 10, 111, 2, 139, 3, 8, 4, 5, 6, 7, 9, 59, 12, 14, 16, 20, 49, 25,
    56, 52, 29, 31, 33, 35, 37, 39, 50, 40, 41, 42, 43, 44, 45, 17, 46, 47, 48, 51,
    53, 55, 155, 58, 62, 152, 150, 153, 11, 13, 15, 18, 19, 171, 21, 172, 22, 167, 170, 24,
    23, 174, 146, 173, 26, 151, 27, 28, 148, 30, 181, 32, 176, 34, 60, 36, 61, 38, 177, 175,
    64, 65, 87, 67, 68, 71, 75, 83, 76, 82, 63, 126, 80, 1, 78, 81, 72, 73, 77, 137,
    69, 70, 74, 79, 85, 66, 84, 86, 89, 99, 100, 101, 102, 103, 104, 88, 90, 105, 91, 93,
    92, 94, 95, 107, 108, 186, 98, 164, 147, 182, 109, 110, 112, 113, 114, 115, 116, 117, 118, 119,
    124, 127, 130, 134, 132, 135, 120, 121, 122, 123, 125, 128, 129, 131, 133, 136, 140, 141, 142, 143,
    144, 145, 54, 57, 149, 154, 156, 157, 158, 96, 97, 159, 106, 160, 161, 162, 163, 165, 166, 168,
    180, 169, 179, 183, 184, 185, 178, 187,
};

uint32_t utf8_tolower(uint32_t c) {
    for (size_t i=0; i < sizeof casefold/sizeof *casefold; ++i) {
        if (c <= casefold[i].c1) {
            if (c < casefold[i].c0) return c;
            int d = casefold[i].m1 - casefold[i].c1;
            if (d == 1) return c + ((casefold[i].c1 & 1) == (c & 1));
            return c + d;
        }
    }
    return c;
}

uint32_t utf8_toupper(uint32_t c) {
    for (size_t i=0; i < sizeof cfold_low/sizeof *cfold_low; ++i) {
        struct CaseFold cfold = casefold[cfold_low[i]];
        if (c <= cfold.m1) {
            int d = cfold.m1 - cfold.c1;
            if (c < (uint32_t)(cfold.c0 + d)) return c;
            if (d == 1) return c - ((cfold.m1 & 1) == (c & 1));
            return c - d;
        }
    }
    return c;
}

bool utf8_isupper(uint32_t c) {
    return utf8_tolower(c) != c;
}

bool utf8_islower(uint32_t c) {
    return utf8_toupper(c) != c;
}

bool utf8_isspace(uint32_t c) {
    static uint16_t t[] = {0x20, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x85, 0xA0,
                           0x1680, 0x2028, 0x2029, 0x202F, 0x205F, 0x3000};
    for (size_t i=0; i<sizeof t/sizeof *t; ++i)
        if (c == t[i]) return true;
    return (c >= 0x2000) & (c <= 0x200A);
}

bool utf8_isdigit(uint32_t c) {
    return ((c >= '0') & (c <= '9')) || 
           ((c >= 0xFF10) & (c <= 0xFF19));
}

bool utf8_isxdigit(uint32_t c) {
    static uint16_t t[] = {0x30, 0x39, 0x41, 0x46, 0x61, 0x66, 0xFF10, 
                           0xFF19, 0xFF21, 0xFF26, 0xFF41, 0xFF46};
    for (size_t i=1; i<sizeof t/sizeof *t; i += 2)
        if (c <= t[i]) return c >= t[i - 1];
    return false;
}

bool utf8_isalnum(uint32_t c) {
    if (c < 128) return isalnum(c) != 0;
    if ((c >= 0xFF10) & (c <= 0xFF19)) return true;
    return utf8_islower(c) || utf8_isupper(c);
}

bool utf8_isalpha(uint32_t c) {
    if (c < 128) return isalpha(c) != 0;
    return utf8_islower(c) || utf8_isupper(c);
}


#ifdef TEST
int main()
{
    for (size_t i=0; i < sizeof cfold_low/sizeof *cfold_low; ++i)
    {
        char x[3][5]={0};
        uint32_t a = casefold[i].c0;
        uint32_t b = utf8_tolower(a);
        uint32_t c = utf8_toupper(b);

        utf8_encode(x[0], a);
        utf8_encode(x[1], b);
        utf8_encode(x[2], c);
        printf("%s %s %s - %u %u %u\n", x[0], x[1], x[2], a, b, c);
    }
}
#endif

