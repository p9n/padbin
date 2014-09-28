#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "./monster_data.h"
#include "./skill_data.h"
#include "./box_data.h"
#include "./bin_decode.h"

class PadDb {
public:
    void load_monster_data(const std::string& path);

    void load_skill_data(const std::string& path);

    void load_box_data(const std::string& path);

    const vector<MonsterData>& monster_data() const;

    const MonsterData& monster(size_t no) const;

    const SkillData& skill(size_t no) const;

    const vector<BoxData>& box_data() const;

private:

    vector<MonsterData> monster_data_;
    vector<SkillData> skill_data_;
    vector<BoxData> box_data_;
};
