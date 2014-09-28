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

#pragma pack(1)
struct BoxData {
    uint32_t exp;
    uint32_t unknown1;
    uint32_t counter;
    uint32_t unknown2;
    uint16_t no;
    uint16_t unknown3;
    uint16_t skill_lv;
    uint16_t unknown4;
    uint16_t flag;  // 8 = 気に入り, 1 = new
    uint8_t hp_plus;
    uint8_t atk_plus;
    uint8_t heal_plus;
    uint8_t kakusei;
};

static_assert(sizeof(BoxData) == 30, "Incorrect BoxData struct size");

template<class T>
T Reverse(T t) {
    char *p = reinterpret_cast<char*>(&t);
    reverse(p, p + sizeof(T));
    return t;
}

int main() {
    ifstream fin("f/data056.bin");
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
   
    BinDecode(v);

    uint16_t box_size = Reverse(*reinterpret_cast<uint16_t*>(&v[0x116]));
    uint16_t monster_count = Reverse(*reinterpret_cast<uint16_t*>(&v[0x118]));

    cout << box_size << ' ' << monster_count << endl;

}
