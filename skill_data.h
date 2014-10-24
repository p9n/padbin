#include <type_traits>
#include <string>

struct SkillRawData {
    uint32_t name_offset;
    uint32_t description_offset;
    uint16_t type;
    uint16_t max_lv;
    uint16_t turn;
    uint16_t is_active_skill;  // 0: leader skill, 65535: active skill?
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;
    uint32_t param6;
    uint32_t param7;
    uint32_t param8;

    SkillRawData(const uint8_t* raw);
    SkillRawData() = default;
};

static_assert(std::is_pod<SkillRawData>::value, "SkillRawData is not pod");
static_assert(sizeof(SkillRawData) == 48, "Incorrect SkillRawData struct size");

struct SkillData : public SkillRawData {
    std::string name;
    std::string description;

    SkillData(const uint8_t* raw, const char* string_table_base);
    SkillData() = delete;
};
