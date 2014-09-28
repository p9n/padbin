#include <algorithm>
#include <regex>

#include "./util.h"
#include "./skill_data.h"

SkillRawData::SkillRawData(const uint8_t* raw) {
    std::copy(raw, raw + sizeof(*this), reinterpret_cast<char*>(this));
    ReverseAll(name_offset, description_offset, type, max_lv, turn, is_leader);
    ReverseAll(param1, param2, param3, param4, param5, param6, param7, param8);
}

SkillData::SkillData(const uint8_t* raw, const char* string_table_base) : SkillRawData(raw) {
    name = std::string(string_table_base + name_offset);
    description = std::string(string_table_base + description_offset);
    description = std::regex_replace(description, std::regex("\n"), "");
}
