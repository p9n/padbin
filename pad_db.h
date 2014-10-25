#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "./monster_data.h"
#include "./skill_data.h"
#include "./box_data.h"

class PadDb {
public:
    PadDb(std::string folder);
    const std::vector<MonsterData>& monster_data() const;
    const MonsterData& monster(size_t no) const;
    const std::vector<SkillData>& skill_data() const;
    const SkillData& skill(size_t no) const;
    const std::vector<BoxData>& box_data() const;
    std::vector<uint16_t> evolution_of(size_t no) const;

private:
    void load_monster_data(const std::string& path);
    void load_skill_data(const std::string& path);
    void load_box_data(const std::string& path);

    std::vector<MonsterData> monster_data_;
    std::vector<SkillData> skill_data_;
    std::vector<BoxData> box_data_;
};
