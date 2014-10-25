#include <type_traits>

#pragma pack(1)
struct BoxData {
    uint32_t exp;
    uint32_t unknown1;
    uint32_t counter;
    uint32_t unknown2;
    uint16_t no;
    uint16_t unknown3;
    uint16_t skill_lv;
    uint16_t unknown4;
    uint16_t flag;  // 8 = 気に入り, 1 = new
    uint8_t hp_plus;
    uint8_t atk_plus;
    uint8_t heal_plus;
    uint8_t awoken_count;

    BoxData(const unsigned char* raw);
    BoxData() = default;
};

static_assert(std::is_pod<BoxData>::value, "BoxData is not pod");
static_assert(sizeof(BoxData) == 30, "Incorrect BoxData struct size");
