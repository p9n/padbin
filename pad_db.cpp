#include "./pad_db.h"

void PadDb::load_monster_data(const std::string& path) {
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

void PadDb::load_skill_data(const std::string& path) {
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

void PadDb::load_box_data(const std::string& path) {
    ifstream fin(path);
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
    BinDecode(v);

    // TODO: need a better way to locate these offset
    uint16_t box_size = Reverse(*reinterpret_cast<uint16_t*>(&v[0x116]));
    uint16_t box_count = Reverse(*reinterpret_cast<uint16_t*>(&v[0x118]));

    cout << "box_count: " << box_count << "/" << box_size << endl;

    for (size_t i = 0; i < box_count; i++) {
        box_data_.emplace_back(v.data() + 0x11A + i * sizeof(BoxData));
    }
}

const vector<MonsterData>& PadDb::monster_data() const {
    return monster_data_;
}

const MonsterData& PadDb::monster(size_t no) const {
    return monster_data_.at(no);
}

const SkillData& PadDb::skill(size_t no) const {
    return skill_data_.at(no);
}

const vector<BoxData>& PadDb::box_data() const {
    return box_data_;
}
