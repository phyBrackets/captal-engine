#ifndef CAPTAL_FOUNDATION_ENCODING_HPP_INCLUDED
#define CAPTAL_FOUNDATION_ENCODING_HPP_INCLUDED

#include <string>
#include <array>
#include <iterator>
#include <concepts>
#include <ranges>

namespace cpt
{

namespace impl
{

inline constexpr std::array<char32_t, 666> lowers
{
    0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069,
    0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072,
    0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00E0,
    0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9,
    0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 0x00F0, 0x00F1, 0x00F2,
    0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC,
    0x00FD, 0x00FE, 0x00FF, 0x0101, 0x0103, 0x0105, 0x0107, 0x0109, 0x010B,
    0x010D, 0x010F, 0x0111, 0x0113, 0x0115, 0x0117, 0x0119, 0x011B, 0x011D,
    0x011F, 0x0121, 0x0123, 0x0125, 0x0127, 0x0129, 0x012B, 0x012D, 0x012F,
    0x0131, 0x0133, 0x0135, 0x0137, 0x013A, 0x013C, 0x013E, 0x0140, 0x0142,
    0x0144, 0x0146, 0x0148, 0x014B, 0x014D, 0x014F, 0x0151, 0x0153, 0x0155,
    0x0157, 0x0159, 0x015B, 0x015D, 0x015F, 0x0161, 0x0163, 0x0165, 0x0167,
    0x0169, 0x016B, 0x016D, 0x016F, 0x0171, 0x0173, 0x0175, 0x0177, 0x017A,
    0x017C, 0x017E, 0x0183, 0x0185, 0x0188, 0x018C, 0x0192, 0x0199, 0x01A1,
    0x01A3, 0x01A5, 0x01A8, 0x01AD, 0x01B0, 0x01B4, 0x01B6, 0x01B9, 0x01BD,
    0x01C6, 0x01C9, 0x01CC, 0x01CE, 0x01D0, 0x01D2, 0x01D4, 0x01D6, 0x01D8,
    0x01DA, 0x01DC, 0x01DF, 0x01E1, 0x01E3, 0x01E5, 0x01E7, 0x01E9, 0x01EB,
    0x01ED, 0x01EF, 0x01F3, 0x01F5, 0x01FB, 0x01FD, 0x01FF, 0x0201, 0x0203,
    0x0205, 0x0207, 0x0209, 0x020B, 0x020D, 0x020F, 0x0211, 0x0213, 0x0215,
    0x0217, 0x0253, 0x0254, 0x0257, 0x0258, 0x0259, 0x025B, 0x0260, 0x0263,
    0x0268, 0x0269, 0x026F, 0x0272, 0x0275, 0x0283, 0x0288, 0x028A, 0x028B,
    0x0292, 0x03AC, 0x03AD, 0x03AE, 0x03AF, 0x03B1, 0x03B2, 0x03B3, 0x03B4,
    0x03B5, 0x03B6, 0x03B7, 0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD,
    0x03BE, 0x03BF, 0x03C0, 0x03C1, 0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7,
    0x03C8, 0x03C9, 0x03CA, 0x03CB, 0x03CC, 0x03CD, 0x03CE, 0x03E3, 0x03E5,
    0x03E7, 0x03E9, 0x03EB, 0x03ED, 0x03EF, 0x0430, 0x0431, 0x0432, 0x0433,
    0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C,
    0x043D, 0x043E, 0x043F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445,
    0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E,
    0x044F, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457, 0x0458,
    0x0459, 0x045A, 0x045B, 0x045C, 0x045E, 0x045F, 0x0461, 0x0463, 0x0465,
    0x0467, 0x0469, 0x046B, 0x046D, 0x046F, 0x0471, 0x0473, 0x0475, 0x0477,
    0x0479, 0x047B, 0x047D, 0x047F, 0x0481, 0x0491, 0x0493, 0x0495, 0x0497,
    0x0499, 0x049B, 0x049D, 0x049F, 0x04A1, 0x04A3, 0x04A5, 0x04A7, 0x04A9,
    0x04AB, 0x04AD, 0x04AF, 0x04B1, 0x04B3, 0x04B5, 0x04B7, 0x04B9, 0x04BB,
    0x04BD, 0x04BF, 0x04C2, 0x04C4, 0x04C8, 0x04CC, 0x04D1, 0x04D3, 0x04D5,
    0x04D7, 0x04D9, 0x04DB, 0x04DD, 0x04DF, 0x04E1, 0x04E3, 0x04E5, 0x04E7,
    0x04E9, 0x04EB, 0x04EF, 0x04F1, 0x04F3, 0x04F5, 0x04F9, 0x0561, 0x0562,
    0x0563, 0x0564, 0x0565, 0x0566, 0x0567, 0x0568, 0x0569, 0x056A, 0x056B,
    0x056C, 0x056D, 0x056E, 0x056F, 0x0570, 0x0571, 0x0572, 0x0573, 0x0574,
    0x0575, 0x0576, 0x0577, 0x0578, 0x0579, 0x057A, 0x057B, 0x057C, 0x057D,
    0x057E, 0x057F, 0x0580, 0x0581, 0x0582, 0x0583, 0x0584, 0x0585, 0x0586,
    0x10D0, 0x10D1, 0x10D2, 0x10D3, 0x10D4, 0x10D5, 0x10D6, 0x10D7, 0x10D8,
    0x10D9, 0x10DA, 0x10DB, 0x10DC, 0x10DD, 0x10DE, 0x10DF, 0x10E0, 0x10E1,
    0x10E2, 0x10E3, 0x10E4, 0x10E5, 0x10E6, 0x10E7, 0x10E8, 0x10E9, 0x10EA,
    0x10EB, 0x10EC, 0x10ED, 0x10EE, 0x10EF, 0x10F0, 0x10F1, 0x10F2, 0x10F3,
    0x10F4, 0x10F5, 0x1E01, 0x1E03, 0x1E05, 0x1E07, 0x1E09, 0x1E0B, 0x1E0D,
    0x1E0F, 0x1E11, 0x1E13, 0x1E15, 0x1E17, 0x1E19, 0x1E1B, 0x1E1D, 0x1E1F,
    0x1E21, 0x1E23, 0x1E25, 0x1E27, 0x1E29, 0x1E2B, 0x1E2D, 0x1E2F, 0x1E31,
    0x1E33, 0x1E35, 0x1E37, 0x1E39, 0x1E3B, 0x1E3D, 0x1E3F, 0x1E41, 0x1E43,
    0x1E45, 0x1E47, 0x1E49, 0x1E4B, 0x1E4D, 0x1E4F, 0x1E51, 0x1E53, 0x1E55,
    0x1E57, 0x1E59, 0x1E5B, 0x1E5D, 0x1E5F, 0x1E61, 0x1E63, 0x1E65, 0x1E67,
    0x1E69, 0x1E6B, 0x1E6D, 0x1E6F, 0x1E71, 0x1E73, 0x1E75, 0x1E77, 0x1E79,
    0x1E7B, 0x1E7D, 0x1E7F, 0x1E81, 0x1E83, 0x1E85, 0x1E87, 0x1E89, 0x1E8B,
    0x1E8D, 0x1E8F, 0x1E91, 0x1E93, 0x1E95, 0x1EA1, 0x1EA3, 0x1EA5, 0x1EA7,
    0x1EA9, 0x1EAB, 0x1EAD, 0x1EAF, 0x1EB1, 0x1EB3, 0x1EB5, 0x1EB7, 0x1EB9,
    0x1EBB, 0x1EBD, 0x1EBF, 0x1EC1, 0x1EC3, 0x1EC5, 0x1EC7, 0x1EC9, 0x1ECB,
    0x1ECD, 0x1ECF, 0x1ED1, 0x1ED3, 0x1ED5, 0x1ED7, 0x1ED9, 0x1EDB, 0x1EDD,
    0x1EDF, 0x1EE1, 0x1EE3, 0x1EE5, 0x1EE7, 0x1EE9, 0x1EEB, 0x1EED, 0x1EEF,
    0x1EF1, 0x1EF3, 0x1EF5, 0x1EF7, 0x1EF9, 0x1F00, 0x1F01, 0x1F02, 0x1F03,
    0x1F04, 0x1F05, 0x1F06, 0x1F07, 0x1F10, 0x1F11, 0x1F12, 0x1F13, 0x1F14,
    0x1F15, 0x1F20, 0x1F21, 0x1F22, 0x1F23, 0x1F24, 0x1F25, 0x1F26, 0x1F27,
    0x1F30, 0x1F31, 0x1F32, 0x1F33, 0x1F34, 0x1F35, 0x1F36, 0x1F37, 0x1F40,
    0x1F41, 0x1F42, 0x1F43, 0x1F44, 0x1F45, 0x1F51, 0x1F53, 0x1F55, 0x1F57,
    0x1F60, 0x1F61, 0x1F62, 0x1F63, 0x1F64, 0x1F65, 0x1F66, 0x1F67, 0x1F80,
    0x1F81, 0x1F82, 0x1F83, 0x1F84, 0x1F85, 0x1F86, 0x1F87, 0x1F90, 0x1F91,
    0x1F92, 0x1F93, 0x1F94, 0x1F95, 0x1F96, 0x1F97, 0x1FA0, 0x1FA1, 0x1FA2,
    0x1FA3, 0x1FA4, 0x1FA5, 0x1FA6, 0x1FA7, 0x1FB0, 0x1FB1, 0x1FD0, 0x1FD1,
    0x1FE0, 0x1FE1, 0x24D0, 0x24D1, 0x24D2, 0x24D3, 0x24D4, 0x24D5, 0x24D6,
    0x24D7, 0x24D8, 0x24D9, 0x24DA, 0x24DB, 0x24DC, 0x24DD, 0x24DE, 0x24DF,
    0x24E0, 0x24E1, 0x24E2, 0x24E3, 0x24E4, 0x24E5, 0x24E6, 0x24E7, 0x24E8,
    0x24E9, 0xFF41, 0xFF42, 0xFF43, 0xFF44, 0xFF45, 0xFF46, 0xFF47, 0xFF48,
    0xFF49, 0xFF4A, 0xFF4B, 0xFF4C, 0xFF4D, 0xFF4E, 0xFF4F, 0xFF50, 0xFF51,
    0xFF52, 0xFF53, 0xFF54, 0xFF55, 0xFF56, 0xFF57, 0xFF58, 0xFF59, 0xFF5A
};

inline constexpr std::array<char32_t, 666> uppers
{
    0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049,
    0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00C0,
    0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9,
    0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2,
    0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC,
    0x00DD, 0x00DE, 0x0178, 0x0100, 0x0102, 0x0104, 0x0106, 0x0108, 0x010A,
    0x010C, 0x010E, 0x0110, 0x0112, 0x0114, 0x0116, 0x0118, 0x011A, 0x011C,
    0x011E, 0x0120, 0x0122, 0x0124, 0x0126, 0x0128, 0x012A, 0x012C, 0x012E,
    0x0049, 0x0132, 0x0134, 0x0136, 0x0139, 0x013B, 0x013D, 0x013F, 0x0141,
    0x0143, 0x0145, 0x0147, 0x014A, 0x014C, 0x014E, 0x0150, 0x0152, 0x0154,
    0x0156, 0x0158, 0x015A, 0x015C, 0x015E, 0x0160, 0x0162, 0x0164, 0x0166,
    0x0168, 0x016A, 0x016C, 0x016E, 0x0170, 0x0172, 0x0174, 0x0176, 0x0179,
    0x017B, 0x017D, 0x0182, 0x0184, 0x0187, 0x018B, 0x0191, 0x0198, 0x01A0,
    0x01A2, 0x01A4, 0x01A7, 0x01AC, 0x01AF, 0x01B3, 0x01B5, 0x01B8, 0x01BC,
    0x01C4, 0x01C7, 0x01CA, 0x01CD, 0x01CF, 0x01D1, 0x01D3, 0x01D5, 0x01D7,
    0x01D9, 0x01DB, 0x01DE, 0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8, 0x01EA,
    0x01EC, 0x01EE, 0x01F1, 0x01F4, 0x01FA, 0x01FC, 0x01FE, 0x0200, 0x0202,
    0x0204, 0x0206, 0x0208, 0x020A, 0x020C, 0x020E, 0x0210, 0x0212, 0x0214,
    0x0216, 0x0181, 0x0186, 0x018A, 0x018E, 0x018F, 0x0190, 0x0193, 0x0194,
    0x0197, 0x0196, 0x019C, 0x019D, 0x019F, 0x01A9, 0x01AE, 0x01B1, 0x01B2,
    0x01B7, 0x0386, 0x0388, 0x0389, 0x038A, 0x0391, 0x0392, 0x0393, 0x0394,
    0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D,
    0x039E, 0x039F, 0x03A0, 0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7,
    0x03A8, 0x03A9, 0x03AA, 0x03AB, 0x038C, 0x038E, 0x038F, 0x03E2, 0x03E4,
    0x03E6, 0x03E8, 0x03EA, 0x03EC, 0x03EE, 0x0410, 0x0411, 0x0412, 0x0413,
    0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C,
    0x041D, 0x041E, 0x041F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425,
    0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E,
    0x042F, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408,
    0x0409, 0x040A, 0x040B, 0x040C, 0x040E, 0x040F, 0x0460, 0x0462, 0x0464,
    0x0466, 0x0468, 0x046A, 0x046C, 0x046E, 0x0470, 0x0472, 0x0474, 0x0476,
    0x0478, 0x047A, 0x047C, 0x047E, 0x0480, 0x0490, 0x0492, 0x0494, 0x0496,
    0x0498, 0x049A, 0x049C, 0x049E, 0x04A0, 0x04A2, 0x04A4, 0x04A6, 0x04A8,
    0x04AA, 0x04AC, 0x04AE, 0x04B0, 0x04B2, 0x04B4, 0x04B6, 0x04B8, 0x04BA,
    0x04BC, 0x04BE, 0x04C1, 0x04C3, 0x04C7, 0x04CB, 0x04D0, 0x04D2, 0x04D4,
    0x04D6, 0x04D8, 0x04DA, 0x04DC, 0x04DE, 0x04E0, 0x04E2, 0x04E4, 0x04E6,
    0x04E8, 0x04EA, 0x04EE, 0x04F0, 0x04F2, 0x04F4, 0x04F8, 0x0531, 0x0532,
    0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 0x0538, 0x0539, 0x053A, 0x053B,
    0x053C, 0x053D, 0x053E, 0x053F, 0x0540, 0x0541, 0x0542, 0x0543, 0x0544,
    0x0545, 0x0546, 0x0547, 0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D,
    0x054E, 0x054F, 0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556,
    0x10A0, 0x10A1, 0x10A2, 0x10A3, 0x10A4, 0x10A5, 0x10A6, 0x10A7, 0x10A8,
    0x10A9, 0x10AA, 0x10AB, 0x10AC, 0x10AD, 0x10AE, 0x10AF, 0x10B0, 0x10B1,
    0x10B2, 0x10B3, 0x10B4, 0x10B5, 0x10B6, 0x10B7, 0x10B8, 0x10B9, 0x10BA,
    0x10BB, 0x10BC, 0x10BD, 0x10BE, 0x10BF, 0x10C0, 0x10C1, 0x10C2, 0x10C3,
    0x10C4, 0x10C5, 0x1E00, 0x1E02, 0x1E04, 0x1E06, 0x1E08, 0x1E0A, 0x1E0C,
    0x1E0E, 0x1E10, 0x1E12, 0x1E14, 0x1E16, 0x1E18, 0x1E1A, 0x1E1C, 0x1E1E,
    0x1E20, 0x1E22, 0x1E24, 0x1E26, 0x1E28, 0x1E2A, 0x1E2C, 0x1E2E, 0x1E30,
    0x1E32, 0x1E34, 0x1E36, 0x1E38, 0x1E3A, 0x1E3C, 0x1E3E, 0x1E40, 0x1E42,
    0x1E44, 0x1E46, 0x1E48, 0x1E4A, 0x1E4C, 0x1E4E, 0x1E50, 0x1E52, 0x1E54,
    0x1E56, 0x1E58, 0x1E5A, 0x1E5C, 0x1E5E, 0x1E60, 0x1E62, 0x1E64, 0x1E66,
    0x1E68, 0x1E6A, 0x1E6C, 0x1E6E, 0x1E70, 0x1E72, 0x1E74, 0x1E76, 0x1E78,
    0x1E7A, 0x1E7C, 0x1E7E, 0x1E80, 0x1E82, 0x1E84, 0x1E86, 0x1E88, 0x1E8A,
    0x1E8C, 0x1E8E, 0x1E90, 0x1E92, 0x1E94, 0x1EA0, 0x1EA2, 0x1EA4, 0x1EA6,
    0x1EA8, 0x1EAA, 0x1EAC, 0x1EAE, 0x1EB0, 0x1EB2, 0x1EB4, 0x1EB6, 0x1EB8,
    0x1EBA, 0x1EBC, 0x1EBE, 0x1EC0, 0x1EC2, 0x1EC4, 0x1EC6, 0x1EC8, 0x1ECA,
    0x1ECC, 0x1ECE, 0x1ED0, 0x1ED2, 0x1ED4, 0x1ED6, 0x1ED8, 0x1EDA, 0x1EDC,
    0x1EDE, 0x1EE0, 0x1EE2, 0x1EE4, 0x1EE6, 0x1EE8, 0x1EEA, 0x1EEC, 0x1EEE,
    0x1EF0, 0x1EF2, 0x1EF4, 0x1EF6, 0x1EF8, 0x1F08, 0x1F09, 0x1F0A, 0x1F0B,
    0x1F0C, 0x1F0D, 0x1F0E, 0x1F0F, 0x1F18, 0x1F19, 0x1F1A, 0x1F1B, 0x1F1C,
    0x1F1D, 0x1F28, 0x1F29, 0x1F2A, 0x1F2B, 0x1F2C, 0x1F2D, 0x1F2E, 0x1F2F,
    0x1F38, 0x1F39, 0x1F3A, 0x1F3B, 0x1F3C, 0x1F3D, 0x1F3E, 0x1F3F, 0x1F48,
    0x1F49, 0x1F4A, 0x1F4B, 0x1F4C, 0x1F4D, 0x1F59, 0x1F5B, 0x1F5D, 0x1F5F,
    0x1F68, 0x1F69, 0x1F6A, 0x1F6B, 0x1F6C, 0x1F6D, 0x1F6E, 0x1F6F, 0x1F88,
    0x1F89, 0x1F8A, 0x1F8B, 0x1F8C, 0x1F8D, 0x1F8E, 0x1F8F, 0x1F98, 0x1F99,
    0x1F9A, 0x1F9B, 0x1F9C, 0x1F9D, 0x1F9E, 0x1F9F, 0x1FA8, 0x1FA9, 0x1FAA,
    0x1FAB, 0x1FAC, 0x1FAD, 0x1FAE, 0x1FAF, 0x1FB8, 0x1FB9, 0x1FD8, 0x1FD9,
    0x1FE8, 0x1FE9, 0x24B6, 0x24B7, 0x24B8, 0x24B9, 0x24BA, 0x24BB, 0x24BC,
    0x24BD, 0x24BE, 0x24BF, 0x24C0, 0x24C1, 0x24C2, 0x24C3, 0x24C4, 0x24C5,
    0x24C6, 0x24C7, 0x24C8, 0x24C9, 0x24CA, 0x24CB, 0x24CC, 0x24CD, 0x24CE,
    0x24CF, 0xFF21, 0xFF22, 0xFF23, 0xFF24, 0xFF25, 0xFF26, 0xFF27, 0xFF28,
    0xFF29, 0xFF2A, 0xFF2B, 0xFF2C, 0xFF2D, 0xFF2E, 0xFF2F, 0xFF30, 0xFF31,
    0xFF32, 0xFF33, 0xFF34, 0xFF35, 0xFF36, 0xFF37, 0xFF38, 0xFF39, 0xFF3A
};

}

inline namespace foundation
{

/*
struct some_encoding
{
    using char_type = xxx; //Replace "xxx" by the integer type used to represent the encoding.

    template<std::input_iterator InputIt>
    static constexpr InputIt decode(InputIt begin, InputIt end, codepoint_t& output) [noexcept]
    {
        //Convert the first character of the sequence [begin, end[ to the UTF-32 codepoint.
        //Write it in "output" then return the iterator pointing on the next character of the decoded sequence.

        return begin;
    }

    template<std::output_iterator<char_type> OutputIt>
    static constexpr OutputIt encode(codepoint_t code, OutputIt output) [noexcept]
    {
        //Convert the UTF-32 codepoint to the sequence representing the character in the encoding.
        //Write the sequence in "output" then return the iterator pointing on the next character of the output sequence.

        return output;
    }

    template<std::input_iterator InputIt>
    static constexpr InputIt next(InputIt begin, InputIt end) [noexcept]
    {
        //Advance "begin" to the next character in the sequence [begin, end[.
        //Return the iterator pointing on the next character of the sequence.

        return begin;
    }

    template<std::input_iterator InputIt>
    static constexpr std::size_t count(InputIt begin, InputIt end) [noexcept]
    {
        //Return the count of "real" characters in the sequence [begin, end[. (For multi-bytes encoding)

        std::size_t count{};
        //...
        return count;
    }

    static constexpr std::size_t max_char_length() noexcept
    {
        //Return the highest count of values to represent a single character.
        //e.g. UTF-8 encoding can use 6 values to represent one character.

        return x;
    }
};
*/

using codepoint_t = char32_t;

template<typename T>
concept encoding = requires(typename T::char_type* it, char32_t code)
{
    typename T::char_type;
    {T::decode(it, it, code)} -> std::convertible_to<typename T::char_type*>;
    {T::encode(code, it)} -> std::convertible_to<typename T::char_type*>;
    {T::next(it, it)} -> std::convertible_to<typename T::char_type*>;
    {T::count(it, it)} -> std::convertible_to<std::size_t>;
    {T::max_char_length()} noexcept -> std::convertible_to<std::size_t>;
};

struct utf8
{
private:
    static constexpr std::array<std::uint8_t, 256> trailing
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5  //UTF-8 can theoretically use up to 6 bytes
    };

    static constexpr std::array<std::uint32_t, 4> offsets{0x00000000, 0x00003080, 0x000E2080, 0x03C82080};
    static constexpr std::array<std::uint8_t, 4> first_bytes{0x00, 0xC0, 0xE0, 0xF0};

/*
    static constexpr std::array<std::uint32_t, 6> offsets{0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080}; //6 bytes version
    static constexpr std::array<std::uint8_t, 6> first_bytes{0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
*/

public:
    using char_type = char8_t;

    template<std::input_iterator InputIt>
    static constexpr InputIt decode(InputIt begin, InputIt end, codepoint_t& output) noexcept
    {
        const std::size_t trailing_bytes{static_cast<std::size_t>(trailing[static_cast<std::uint8_t>(*begin)])};

        if(std::next(begin, trailing_bytes) < end && trailing_bytes < 4)
        {
            output = 0;

            switch(trailing_bytes)
            {
                case 3: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
                case 2: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
                case 1: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
                case 0: output += static_cast<std::uint8_t>(*begin++);
            }

            output -= offsets[trailing_bytes];
        }
        else
        {
            begin = end;
            output = 0;
        }

        return begin;
    }

    template<std::output_iterator<char_type> OutputIt>
    static constexpr OutputIt encode(codepoint_t code, OutputIt output) noexcept
    {
        if((code > 0x0010FFFF) || ((code >= 0xD800) && (code <= 0xDBFF))) //Invalid value
        {
            *output++ = 0;
        }
        else
        {
            std::size_t count{};

            if(code < 0x80)
            {
                count = 0;
            }
            else if(code < 0x800)
            {
                count = 1;
            }
            else if(code < 0x10000)
            {
                count = 2;
            }
            else
            {
                count = 3;
            }

            std::array<std::uint8_t, 4> bytes{};
            switch(count)
            {
                case 3: bytes[3] = static_cast<std::uint8_t>((code | 0x80) & 0xBF); code >>= 6; [[fallthrough]];
                case 2: bytes[2] = static_cast<std::uint8_t>((code | 0x80) & 0xBF); code >>= 6; [[fallthrough]];
                case 1: bytes[1] = static_cast<std::uint8_t>((code | 0x80) & 0xBF); code >>= 6; [[fallthrough]];
                case 0: bytes[0] = static_cast<std::uint8_t> (code | first_bytes[count]);
            }

            for(std::size_t i{}; i < count + 1; ++i)
            {
                *output++ = bytes[i];
            }
        }

        return output;
    }

    template<std::input_iterator InputIt>
    static constexpr InputIt next(InputIt begin, InputIt end) noexcept
    {
        if(begin == end)
        {
            return end;
        }

        while((*++begin & 0xc0) != 0x80 && begin != end) //Advance as long as it is not the begin of a character code.
        {

        }

        return begin;
    }

    template<std::input_iterator InputIt>
    static constexpr std::size_t count(InputIt begin, InputIt end) noexcept
    {
        std::size_t count{};

        while(begin != end)
        {
            if((*begin & 0xc0) != 0x80) //Check if it is the begin
            {
                ++count;
            }

            ++begin;
        }

        return count;
    }

    static constexpr std::size_t max_char_length() noexcept
    {
        return 4;
    }
};

struct utf16
{
    using char_type = char16_t;

    template<std::input_iterator InputIt>
    static constexpr InputIt decode(InputIt begin, InputIt end, codepoint_t& output) noexcept
    {
        const std::uint16_t first{static_cast<std::uint16_t>(*begin++)};

        if(first >= 0xD800 && first <= 0xDBFF)
        {
            if(begin < end)
            {
                const std::uint16_t second{static_cast<std::uint16_t>(*begin++)};

                if(second >= 0xDC00 && second <= 0xDFFF)
                {
                    output = static_cast<codepoint_t>(((first - 0xD800) << 10) + (second - 0xDC00) + 0x10000);
                }
                else
                {
                    output = 0;
                }
            }
            else
            {
                begin = end;
                output = 0;
            }
        }
        else
        {
            output = first;
        }

        return begin;
    }

    template<std::output_iterator<char_type> OutputIt>
    static constexpr OutputIt encode(codepoint_t code, OutputIt output) noexcept
    {
        if(code <= 0xFFFF)
        {
            if(code >= 0xD800 && code <= 0xDFFF)
            {
                *output++ = 0;
            }
            else
            {
                *output++ = static_cast<std::uint16_t>(code);
            }
        }
        else if(code > 0x0010FFFF)
        {
            *output++ = 0;
        }
        else
        {
            code -= 0x0010000;
            *output++ = static_cast<std::uint16_t>((code >> 10)     + 0xD800);
            *output++ = static_cast<std::uint16_t>((code & 0x3FFUL) + 0xDC00);
        }

        return output;
    }

    template<std::input_iterator InputIt>
    static constexpr InputIt next(InputIt begin, InputIt end) noexcept
    {
        if(begin == end)
        {
            return end;
        }

        if((*begin <= 0xD7FF) || (*begin >= 0xE000 && *begin <= 0xFFFD))
        {
            std::advance(begin, 1);
        }
        else
        {
            std::advance(begin, 2);
        }

        return begin;
    }

    template<std::input_iterator InputIt>
    static constexpr std::size_t count(InputIt begin, InputIt end) noexcept
    {
        std::size_t count{};

        while(begin < end)
        {
            if((*begin <= 0xD7FF) || (*begin >= 0xE000 && *begin <= 0xFFFD))
            {
                std::advance(begin, 1);
            }
            else
            {
                std::advance(begin, 2);
            }

            ++count;
        }

        return count;
    }

    static constexpr std::size_t max_char_length() noexcept
    {
        return 2;
    }
};

struct utf32
{
    using char_type = char32_t;

    template<std::input_iterator InputIt>
    static constexpr InputIt decode(InputIt begin, InputIt, codepoint_t& output) noexcept
    {
        output = *begin++;
        return begin;
    }

    template<std::output_iterator<char_type> OutputIt>
    static constexpr OutputIt encode(codepoint_t code, OutputIt output) noexcept
    {
        *output++ = code;
        return output;
    }

    template<std::input_iterator InputIt>
    static constexpr InputIt next(InputIt begin, InputIt) noexcept
    {
        return ++begin;
    }

    template<std::input_iterator InputIt>
    static constexpr std::size_t count(InputIt begin, InputIt end) noexcept
    {
        return static_cast<std::size_t>(std::distance(begin, end));
    }

    static constexpr std::size_t max_char_length() noexcept
    {
        return 1;
    }
};

#ifdef _WIN32

struct wide : public utf16
{
    using char_type = wchar_t;
};

#else //On most systems wchar_t is UCS-4 or UTF-32

struct wide : public utf32
{
    using char_type = wchar_t;
};

#endif

struct narrow : public utf8
{
    using char_type = char;
};

struct latin_1
{
    using char_type = char;

    template<std::input_iterator InputIt>
    static constexpr InputIt decode(InputIt begin, InputIt, codepoint_t& output) noexcept
    {
        const std::uint8_t value{static_cast<std::uint8_t>(*begin++)};

        if(value >= 0x80 && value < 0xA0) //in latin_1 this range is unused, but there are unicode code points for this range
        {
            output = 0;
        }
        else
        {
            output = static_cast<codepoint_t>(value);
        }

        return begin;
    }

    template<std::output_iterator<char_type> OutputIt>
    static constexpr OutputIt encode(codepoint_t code, OutputIt output) noexcept
    {
        if(code > 0xFF)
        {
            *output++ = 0;
        }
        else
        {
            *output++ = static_cast<std::uint8_t>(code);
        }

        return output;
    }

    template<std::input_iterator InputIt>
    static constexpr InputIt next(InputIt begin, InputIt) noexcept
    {
        return ++begin;
    }

    template<std::input_iterator InputIt>
    static constexpr std::size_t count(InputIt begin, InputIt end) noexcept
    {
        return static_cast<std::size_t>(std::distance(begin, end));
    }

    static constexpr std::size_t max_char_length() noexcept
    {
        return 1;
    }
};


template<encoding Input, encoding Output, std::input_iterator InputIt, std::output_iterator<typename Output::char_type> OutputIt>
constexpr OutputIt convert(InputIt begin, InputIt end, OutputIt output)
{
    //A -> A is a no-op
    //A -> B and B -> A is also a no-op if their are based on the same type (I do this to prevent from code duplication)
    //Ex: on Windows cpt::wide and cpt::utf16 would be the same (because wide is just a public inheritance of utf16)
    if constexpr(std::is_same_v<Input, Output> || std::is_base_of_v<Input, Output> || std::is_base_of_v<Output, Input>)
    {
        return std::copy(begin, end, output);
    }
    else
    {
        while(begin < end)
        {
            codepoint_t code{};
            begin  = Input::decode(begin, end, code);
            output = Output::encode(code, output);
        }

        return output;
    }
}

template<encoding Input, encoding Output, typename StringIn, typename StringOut = std::basic_string<typename Output::char_type>>
constexpr StringOut convert(const StringIn& str)
{
    StringOut output{};
    convert<Input, Output>(std::begin(str), std::end(str), std::back_inserter(output));

    return output;
}

template<encoding Encoding, std::input_iterator InputIt, std::output_iterator<typename Encoding::char_type> OutputIt>
constexpr OutputIt to_lower(InputIt begin, InputIt end, OutputIt output)
{
    while(begin < end)
    {
        codepoint_t code{};
        begin = Encoding::decode(begin, end, code);

        const auto it{std::lower_bound(std::begin(impl::uppers), std::end(impl::uppers), code)};
        if(it != std::end(impl::uppers) && *it == code)
        {
            code = impl::lowers[std::distance(std::begin(impl::uppers), it)];
        }

        output = Encoding::encode(code, output);
    }

    return output;
}

template<encoding Encoding, typename String = std::basic_string<typename Encoding::char_type>>
constexpr String to_lower(const String& str)
{
    String output{};
    output.resize(std::size(str));

    to_lower<Encoding>(std::begin(str), std::end(str), std::begin(output));

    return output;
}

template<encoding Encoding, std::input_iterator InputIt, std::output_iterator<typename Encoding::char_type> OutputIt>
constexpr OutputIt to_upper(InputIt begin, InputIt end, OutputIt output)
{
    while(begin < end)
    {
        codepoint_t code{};
        begin = Encoding::decode(begin, end, code);

        const auto it{std::lower_bound(std::begin(impl::lowers), std::end(impl::lowers), code)};
        if(it != std::end(impl::lowers) && *it == code)
        {
            code = impl::uppers[std::distance(std::begin(impl::lowers), it)];
        }

        output = Encoding::encode(code, output);
    }

    return output;
}

template<encoding Encoding, typename String = std::basic_string<typename Encoding::char_type>>
constexpr String to_upper(const String& str)
{
    String output{};
    to_upper<Encoding>(std::begin(str), std::end(str), std::back_inserter(output));

    return output;
}

template<encoding Encoding, std::input_iterator InputIt>
class decoder_iterator
{
    static constexpr codepoint_t end_of_stream{std::numeric_limits<codepoint_t>::max()};

public:
    using encoding = Encoding;
    using iterator_type = InputIt;
    using iterator_category = std::input_iterator_tag;
    using value_type = codepoint_t;
    using char_type = codepoint_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const codepoint_t*;
    using reference = codepoint_t;

public:
    constexpr decoder_iterator() = default;

    constexpr decoder_iterator(iterator_type begin, iterator_type end)
    :m_begin{std::move(begin)}
    ,m_end{std::move(end)}
    {
        operator++();
    }

    ~decoder_iterator() = default;
    decoder_iterator(const decoder_iterator&) = default;
    decoder_iterator& operator=(const decoder_iterator&) = default;
    decoder_iterator(decoder_iterator&& other) noexcept = default;
    decoder_iterator& operator=(decoder_iterator&& other) noexcept = default;

    constexpr reference operator*() const noexcept
    {
        return m_codepoint;
    }

    constexpr pointer operator->() const noexcept
    {
        return &m_codepoint;
    }

    constexpr decoder_iterator& operator++()
    {
        if(m_begin != m_end)
        {
            m_begin = encoding::decode(m_begin, m_end, m_codepoint);
        }
        else
        {
            m_codepoint = end_of_stream;
        }

        return *this;
    }

    constexpr decoder_iterator operator++(int)
    {
        decoder_iterator temp{*this};
        operator++();

        return temp;
    }

    constexpr decoder_iterator begin() const
    {
        return *this;
    }

    constexpr decoder_iterator end() const
    {
        return decoder_iterator{};
    }

    constexpr bool operator!=(const decoder_iterator& other) const noexcept
    {
        return m_codepoint != other.m_codepoint;
    }

private:
    iterator_type m_begin{};
    iterator_type m_end{};
    value_type m_codepoint{end_of_stream};
};

template<encoding Encoding, std::input_iterator InputIt>
constexpr decoder_iterator<Encoding, InputIt> decode(InputIt&& begin, InputIt&& end)
{
    return decoder_iterator<Encoding, InputIt>{std::forward(begin), std::forward(end)};
}

template<encoding Encoding, std::ranges::range Range>
constexpr decoder_iterator<Encoding, std::ranges::iterator_t<Range>> decode(const Range& container)
{
    return decoder_iterator<Encoding, std::ranges::iterator_t<Range>>{std::begin(container), std::end(container)};
}

template<encoding Encoding, typename T, std::size_t N>
constexpr decoder_iterator<Encoding, const T*> decode(const T (&array)[N])
{
    return decoder_iterator<Encoding, const T*>{std::begin(array), std::end(array)};
}

inline std::string_view to_narrow(const std::u8string_view& str) noexcept
{
    return std::string_view{reinterpret_cast<const char*>(std::data(str)), std::size(str)};
}

}

}

#endif
