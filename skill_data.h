#include <string>
#include <algorithm>
#include <regex>

#include "./util.h"

struct SkillRawData {
    uint32_t name_offset;
    uint32_t description_offset;
    uint16_t type;
    uint16_t max_lv;
    uint16_t turn;
    uint16_t is_leader;  // 0: leader skill, 65535: normal skill?
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;
    uint32_t param6;
    uint32_t param7;
    uint32_t param8;

    SkillRawData(const uint8_t* raw) {
        std::copy(raw, raw + sizeof(SkillRawData), reinterpret_cast<char*>(this));
        ReverseAll(name_offset, description_offset, type, max_lv, turn, is_leader);
        ReverseAll(param1, param2, param3, param4, param5, param6, param7, param8);
    }
};

static_assert(sizeof(SkillRawData) == 48, "Incorrect SkillRawData struct size");

struct SkillData : public SkillRawData {
    std::string name;
    std::string description;

    SkillData(const uint8_t* raw, const char* string_table_base) : SkillRawData(raw) {
        name = std::string(string_table_base + name_offset);
        description = std::string(string_table_base + description_offset);
        description = regex_replace(description, regex("\n"), "");
    }
};
