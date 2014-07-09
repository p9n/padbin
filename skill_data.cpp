#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>

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
    ifstream fin("data036.bin");
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
    assert(v.size() == 204794);
   
    BinDecode(v);

    for (int i = 0; i < 32; i++) {
        cout << setw(2) << setfill('0') << hex << (int)v[i] << ' ';
    }
    cout << endl << dec;

    SkillRawData *s = reinterpret_cast<SkillRawData*>(&v[32]);
    uint32_t skill_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));

    size_t string_table_offset = 32 + sizeof(SkillRawData) * skill_count + 8;

    for (uint32_t x = 0; x < skill_count; ++x) {
        auto l = Reverse(s[x].isLeader);
        if (l == 0 or l == 65535) continue;
        cout << Reverse(s[x].name) << ' ' << &v[Reverse(s[x].name) + string_table_offset] << endl;
        cout << Reverse(s[x].description) << ' ' << &v[Reverse(s[x].description) + string_table_offset] << endl;
        cout << Reverse(s[x].isLeader) << endl;
    }
}
