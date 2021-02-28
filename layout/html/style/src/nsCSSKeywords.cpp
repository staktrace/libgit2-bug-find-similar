
/*
** This is a generated file, do not edit it. This file is created by
** genhash.pl
*/

#include "plstr.h"
#include "nsCSSKeywords.h"
#define TOTAL_KEYWORDS 86
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 8
#define MAX_HASH_VALUE 322
/* maximum key range = 315, duplicates = 0 */


struct StaticNameTable {
  char* tag;
  PRInt32 id;
};

static const unsigned char kLowerLookup[256] = {
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
  32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
  48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
  64,
    97,98,99,100,101,102,103,104,105,106,107,108,109,
    110,111,112,113,114,115,116,117,118,119,120,121,122,

   91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
  112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,

  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
  160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
  176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
  192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
  208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
  224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
  240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};

#define MYLOWER(x) kLowerLookup[((x) & 0x7f)]

/**
 * Map a name to an ID or -1
 */
PRInt32 nsCSSKeywords::LookupName(const char* str)
{
  static unsigned short asso_values[] =
    {
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323,  15, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323, 323, 323, 323,
     323, 323, 323, 323, 323, 323, 323,   5, 125, 115,
      35,   0,   5,  55,  60, 110,   0,   5,   0,   5,
      75,   0,  90,   5,   0,  25,   0,  27,   0,  90,
      10,   0, 323, 323, 323, 323, 323, 323,
    };
  static unsigned char lengthtable[] =
    {
      0,  0,  0,  0,  0,  0,  0,  0,  8,  4,  5,  6,  2,  0,
      0,  0,  0,  2,  0,  0,  0, 11,  2,  0,  0,  0, 11,  0,
      0,  0,  0,  0,  7,  6,  7,  0,  4,  0,  0,  0,  5,  0,
      7,  8,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,
      0,  7,  0,  7,  0,  6,  0,  6,  0,  5,  0,  0,  6,  0,
      0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  6,  0,  8,
      0,  0,  0,  0,  0,  0,  0,  0,  2,  3,  0,  0,  6,  0,
      8,  9,  0,  0,  0,  0,  9,  0,  6,  7,  8,  0,  0, 11,
      2,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,
      0,  2,  0,  0,  0,  6,  7,  0,  0,  5,  6,  0,  0,  0,
      0,  0,  7,  0,  0,  0,  6,  5,  0,  9,  5,  6,  0,  0,
      4,  0,  6,  7,  0,  0,  0,  0,  0,  8,  4,  5,  0,  0,
      0,  9,  5,  0,  7,  0,  0,  0, 11,  0,  0,  0,  5,  0,
      0,  3,  0, 10,  0,  0,  0,  4,  0,  6,  0,  0,  0,  0,
      6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  5,  6,  0,  0,  0, 10,  9,  0,  0,
      0,  0,  0,  0, 11,  0,  0,  6,  0,  0,  0,  0,  6,  0,
      8,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  4,  0,  0,
      7,  0,  0,  0,  9,  0,  0,  0,  0,  6,  2,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  6,  0,  0,  4,  0,  0,  0, 11,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,
      2,
    };
  static struct StaticNameTable wordlist[] =
    {
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"relative",  56},
      {"left",  32},
      {"large",  30},
      {"larger",  31},
      {"em",  18},
      {"",}, {"",}, {"",}, {"",}, 
      {"mm",  41},
      {"",}, {"",}, {"",}, 
      {"transparent",  76},
      {"ex",  19},
      {"",}, {"",}, {"",}, 
      {"text-bottom",  71},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"x-large",  82},
      {"outset",  48},
      {"outside",  49},
      {"",}, 
      {"auto",  1},
      {"",}, {"",}, {"",}, 
      {"small",  63},
      {"",}, 
      {"smaller",  65},
      {"xx-small",  85},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"medium",  39},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"x-small",  83},
      {"",}, 
      {"justify",  29},
      {"",}, 
      {"groove",  21},
      {"",}, 
      {"square",  67},
      {"",}, 
      {"solid",  66},
      {"",}, {"",}, 
      {"double",  17},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"dotted",  16},
      {"",}, {"",}, {"",}, {"",}, 
      {"normal",  44},
      {"",}, 
      {"overline",  50},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"pt",  54},
      {"pre",  53},
      {"",}, {"",}, 
      {"repeat",  57},
      {"",}, 
      {"xx-large",  84},
      {"no-repeat",  42},
      {"",}, {"",}, {"",}, {"",}, 
      {"lowercase",  38},
      {"",}, 
      {"dashed",  13},
      {"noshade",  45},
      {"text-top",  72},
      {"",}, {"",}, 
      {"lower-alpha",  36},
      {"px",  55},
      {"repeat-y",  59},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"repeat-x",  58},
      {"",}, {"",}, {"",}, 
      {"cm",  12},
      {"",}, {"",}, {"",}, 
      {"bolder",  6},
      {"oblique",  47},
      {"",}, {"",}, 
      {"block",  4},
      {"bottom",  8},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"visible",  81},
      {"",}, {"",}, {"",}, 
      {"scroll",  62},
      {"super",  70},
      {"",}, 
      {"list-item",  35},
      {"ridge",  60},
      {"static",  68},
      {"",}, {"",}, 
      {"none",  43},
      {"",}, 
      {"middle",  40},
      {"decimal",  14},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"absolute",  0},
      {"bold",  5},
      {"fixed",  20},
      {"",}, {"",}, {"",}, 
      {"paragraph",  51},
      {"right",  61},
      {"",}, 
      {"lighter",  33},
      {"",}, {"",}, {"",}, 
      {"lower-roman",  37},
      {"",}, {"",}, {"",}, 
      {"thick",  73},
      {"",}, {"",}, 
      {"top",  75},
      {"",}, 
      {"small-caps",  64},
      {"",}, {"",}, {"",}, 
      {"both",  7},
      {"",}, 
      {"inline",  25},
      {"",}, {"",}, {"",}, {"",}, 
      {"center",  10},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      
      {"inset",  26},
      {"inside",  27},
      {"",}, {"",}, {"",}, 
      {"capitalize",  9},
      {"uppercase",  80},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"upper-alpha",  78},
      {"",}, {"",}, 
      {"circle",  11},
      {"",}, {"",}, {"",}, {"",}, 
      {"italic",  28},
      {"",}, 
      {"baseline",  2},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"blink",  3},
      {"",}, {"",}, {"",}, 
      {"thin",  74},
      {"",}, {"",}, 
      {"inherit",  24},
      {"",}, {"",}, {"",}, 
      {"underline",  77},
      {"",}, {"",}, {"",}, {"",}, 
      {"nowrap",  46},
      {"in",  23},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"hidden",  22},
      {"",}, {"",}, 
      {"disc",  15},
      {"",}, {"",}, {"",}, 
      {"upper-roman",  79},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, 
      {"sub",  69},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, {"",}, 
      {"line-through",  34},
      {"",}, {"",}, {"",}, {"",}, 
      {"pc",  52},
    };

  if (str != NULL) {
    int len = PL_strlen(str);
    if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH) {
  register int hval = len;

  switch (hval)
    {
      default:
      case 7:
        hval += asso_values[MYLOWER(str[6])];
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[MYLOWER(str[2])];
      case 2:
        hval += asso_values[MYLOWER(str[1])];
      case 1:
        hval += asso_values[MYLOWER(str[0])];
        break;
    }
  hval += asso_values[MYLOWER(str[len - 1])];
      if (hval <= MAX_HASH_VALUE && hval >= MIN_HASH_VALUE) {
        if (len == lengthtable[hval]) {
          register const char *tag = wordlist[hval].tag;

          /*
          ** While not at the end of the string, if they ever differ
          ** they are not equal.  We know "tag" is already lower case.
          */
          while ((*tag != '\0')&&(*str != '\0')) {
            if (*tag != (char) MYLOWER(*str)) {
              return -1;
            }
            tag++;
            str++;
          }

          /*
          ** One of the strings has ended, if they are both ended, then they
          ** are equal, otherwise not.
          */
          if ((*tag == '\0')&&(*str == '\0')) {
            return wordlist[hval].id;
          }
        }
      }
    }
  }
  return -1;
}

const nsCSSKeywords::NameTableEntry nsCSSKeywords::kNameTable[] = {
  { "absolute", 0 }, 
  { "auto", 1 }, 
  { "baseline", 2 }, 
  { "blink", 3 }, 
  { "block", 4 }, 
  { "bold", 5 }, 
  { "bolder", 6 }, 
  { "both", 7 }, 
  { "bottom", 8 }, 
  { "capitalize", 9 }, 
  { "center", 10 }, 
  { "circle", 11 }, 
  { "cm", 12 }, 
  { "dashed", 13 }, 
  { "decimal", 14 }, 
  { "disc", 15 }, 
  { "dotted", 16 }, 
  { "double", 17 }, 
  { "em", 18 }, 
  { "ex", 19 }, 
  { "fixed", 20 }, 
  { "groove", 21 }, 
  { "hidden", 22 }, 
  { "in", 23 }, 
  { "inherit", 24 }, 
  { "inline", 25 }, 
  { "inset", 26 }, 
  { "inside", 27 }, 
  { "italic", 28 }, 
  { "justify", 29 }, 
  { "large", 30 }, 
  { "larger", 31 }, 
  { "left", 32 }, 
  { "lighter", 33 }, 
  { "line-through", 34 }, 
  { "list-item", 35 }, 
  { "lower-alpha", 36 }, 
  { "lower-roman", 37 }, 
  { "lowercase", 38 }, 
  { "medium", 39 }, 
  { "middle", 40 }, 
  { "mm", 41 }, 
  { "no-repeat", 42 }, 
  { "none", 43 }, 
  { "normal", 44 }, 
  { "noshade", 45 }, 
  { "nowrap", 46 }, 
  { "oblique", 47 }, 
  { "outset", 48 }, 
  { "outside", 49 }, 
  { "overline", 50 }, 
  { "paragraph", 51 }, 
  { "pc", 52 }, 
  { "pre", 53 }, 
  { "pt", 54 }, 
  { "px", 55 }, 
  { "relative", 56 }, 
  { "repeat", 57 }, 
  { "repeat-x", 58 }, 
  { "repeat-y", 59 }, 
  { "ridge", 60 }, 
  { "right", 61 }, 
  { "scroll", 62 }, 
  { "small", 63 }, 
  { "small-caps", 64 }, 
  { "smaller", 65 }, 
  { "solid", 66 }, 
  { "square", 67 }, 
  { "static", 68 }, 
  { "sub", 69 }, 
  { "super", 70 }, 
  { "text-bottom", 71 }, 
  { "text-top", 72 }, 
  { "thick", 73 }, 
  { "thin", 74 }, 
  { "top", 75 }, 
  { "transparent", 76 }, 
  { "underline", 77 }, 
  { "upper-alpha", 78 }, 
  { "upper-roman", 79 }, 
  { "uppercase", 80 }, 
  { "visible", 81 }, 
  { "x-large", 82 }, 
  { "x-small", 83 }, 
  { "xx-large", 84 }, 
  { "xx-small", 85 }, 
};