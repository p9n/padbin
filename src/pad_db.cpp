#include <boost/filesystem/fstream.hpp>

#include "./pad_db.h"
#include "./bin_decode.h"

using namespace std;

PadDb::PadDb(const boost::filesystem::path& folder) {
    const string DATA021 = "data021.bin";
    const string DATA036 = "data036.bin";
    const string DATA056 = "data056.bin";

    load_monster_data(folder / DATA021);
    load_skill_data(folder / DATA036);
    load_box_data(folder / DATA056);
}

void PadDb::load_monster_data(const boost::filesystem::path& filename) {
    boost::filesystem::ifstream fin(filename);
    fin >> noskipws;
    vector<unsigned char> v{istream_iterator<unsigned char>(fin), istream_iterator<unsigned char>()};
    BinDecode(v);

    uint32_t monster_count = Reverse(reinterpret_cast<uint32_t&>(v[24]));

    assert(v.size() == 32 + monster_count * sizeof(MonsterData));

    for (size_t i = 0; i < monster_count; i++) {
        monster_data_.emplace_back(v.data() + 32 + i * sizeof(MonsterData));
    }
}

void PadDb::load_skill_data(const boost::filesystem::path& filename) {
    boost::filesystem::ifstream fin(filename);
    fin >> noskipws;
    vector<unsigned char> v{istream_iterator<unsigned char>(fin), istream_iterator<unsigned char>()};
    BinDecode(v);

    uint32_t skill_count = Reverse(reinterpret_cast<uint32_t&>(v[24]));
    uint32_t string_table_size = Reverse(reinterpret_cast<uint32_t&>(v[32 + sizeof(SkillRawData) * skill_count + 4]));

    assert(v.size() == 32 + sizeof(SkillRawData) * skill_count + 8 + string_table_size);

    size_t string_table_offset = 32 + sizeof(SkillRawData) * skill_count + 8;
    const char* string_table_base = reinterpret_cast<char*>(v.data()) + string_table_offset;

    for (size_t i = 0; i < skill_count; i++) {
        skill_data_.emplace_back(v.data() + 32 + i * sizeof(SkillRawData), string_table_base);
    }
}

void PadDb::load_box_data(const boost::filesystem::path& filename) {
    boost::filesystem::ifstream fin(filename);
    fin >> noskipws;
    vector<unsigned char> v{istream_iterator<unsigned char>(fin), istream_iterator<unsigned char>()};
    BinDecode(v);

    // TODO: need a better way to locate these offset
    // uint16_t box_size = Reverse(reinterpret_cast<uint16_t&>(&v[278]));
    uint16_t box_count = Reverse(reinterpret_cast<uint16_t&>(v[280]));
    constexpr auto BOX_OFFSET = 282;

    for (size_t i = 0; i < box_count; i++) {
        box_data_.emplace_back(v.data() + BOX_OFFSET + i * sizeof(BoxData));
    }
}

const vector<MonsterData>& PadDb::monster_data() const {
    return monster_data_;
}

const MonsterData& PadDb::monster(size_t no) const {
    return monster_data_.at(no);
}

const vector<SkillData>& PadDb::skill_data() const {
    return skill_data_;
}

const SkillData& PadDb::skill(size_t no) const {
    return skill_data_.at(no);
}

const vector<BoxData>& PadDb::box_data() const {
    return box_data_;
}

vector<uint16_t> PadDb::evolution_of(size_t no) const {
    vector<uint16_t> result;
    for (auto& m : monster_data_) {
        if (m.base_monster == no) result.push_back(m.no);
    }
    return result;
}
