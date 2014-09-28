#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "./monster_data.h"
#include "./skill_data.h"
#include "./bin_decode.h"

class PadDb {
public:
    void LoadMonsterData(const std::string& path) {
        ifstream fin(path);
        fin >> noskipws;
        vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
        BinDecode(v);

        uint32_t monster_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));
        cout << "monster count: " << monster_count << endl;

        assert(v.size() == 32 + monster_count * sizeof(MonsterData));

        for (size_t i = 0; i < monster_count; i++) {
            monster_data_.emplace_back(v.data() + 32 + i * sizeof(MonsterData));
        }
    }

    void LoadSkillData(const std::string& path) {
        ifstream fin(path);
        fin >> noskipws;
        vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
        BinDecode(v);
        
        uint32_t skill_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));

        size_t string_table_offset = 32 + sizeof(SkillRawData) * skill_count + 8;
        const char* string_table_base = reinterpret_cast<char*>(v.data()) + string_table_offset;

        cout << "skill count: " << skill_count << endl;

        for (size_t i = 0; i < skill_count; i++) {
            skill_data_.emplace_back(v.data() + 32 + i * sizeof(SkillRawData), string_table_base);
        }
    }

    const vector<MonsterData>& monster_data() const {
        return monster_data_;
    }

    const MonsterData& monster(size_t no) const {
        return monster_data_.at(no);
    }

    const SkillData& skill(size_t no) const {
        return skill_data_.at(no);
    }

private:

    vector<MonsterData> monster_data_;
    vector<SkillData> skill_data_;
};
