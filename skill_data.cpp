#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>
#include <regex>

#include "./bin_decode.h"

using namespace std;

struct SkillRawData {
    uint32_t name;
    uint32_t description;
    uint16_t type;
    uint16_t maxLv;
    uint16_t turn;
    uint16_t isLeader;  // 0: leader skill, 65536: normal skill?
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;
    uint32_t param6;
    uint32_t param7;
    uint32_t param8;
};

static_assert(sizeof(SkillRawData) == 48, "Incorrect SkillRawData struct size");

template<class T>
T Reverse(T t) {
    char *p = reinterpret_cast<char*>(&t);
    reverse(p, p + sizeof(T));
    return t;
}


int main() {
    ifstream fin("f/data036.bin");
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
   
    BinDecode(v);

    SkillRawData *s = reinterpret_cast<SkillRawData*>(&v[32]);
    uint32_t skill_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));

    size_t string_table_offset = 32 + sizeof(SkillRawData) * skill_count + 8;

    cout << "skill count: " << skill_count << endl;

    for (uint32_t x = 0; x < skill_count; ++x) {
        string name = (char*)&v[Reverse(s[x].name) + string_table_offset];
        string description = (char*)&v[Reverse(s[x].description) + string_table_offset];
        description = regex_replace(description, regex("\n"), "");
        bool is_leader = not Reverse(s[x].isLeader);
        if (is_leader and regex_search(description, regex(u8"少し上がり|1.5倍"))) {
            cout << name << " - " << description << endl;
        }
    }
}
