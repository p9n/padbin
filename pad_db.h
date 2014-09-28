#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "./monster_data.h"
#include "./bin_decode.h"

class PadDb {
public:
    void LoadMonsterData(std::string path) {
        ifstream fin("f/data021.bin");
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

    const vector<MonsterData>& monster_data() const {
        return monster_data_;
    }

    const MonsterData& monster(size_t no) const {
        return monster_data_.at(no);
    }

private:

    vector<MonsterData> monster_data_;
};
