#include <algorithm>
#include <regex>

#include "./util.h"
#include "./skill_data.h"

SkillRawData::SkillRawData(const unsigned char* raw) {
    std::copy(raw, raw + sizeof(*this), reinterpret_cast<unsigned char*>(this));
    ReverseAll(name_offset, description_offset, type, max_lv, turn, is_active_skill);
    ReverseAll(param1, param2, param3, param4, param5, param6, param7, param8);
}

SkillData::SkillData(const unsigned char* raw, const char* string_table_base) : SkillRawData(raw) {
    name = std::string(string_table_base + name_offset);
    description = std::string(string_table_base + description_offset);
    description = std::regex_replace(description, std::regex("\n"), std::string());
}
