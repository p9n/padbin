#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>
#include <map>
#include <type_traits>
#include <boost/range.hpp>

#include "./bin_decode.h"
#include "./enum.h"

using namespace std;

template<class T>
T Reverse(T& t) {
    char *p = reinterpret_cast<char*>(&t);
    reverse(p, p + sizeof(T));
    return t;
}

void ReverseAll() {
}

template<class T, class... Targs>
void ReverseAll(T& t, Targs& ...args) {
    Reverse(t);
    ReverseAll(args...);
}

#pragma pack(1)
struct MonsterData {
    MonsterData(const uint8_t* raw) {
        copy(raw, raw + sizeof(MonsterData), reinterpret_cast<char*>(this));
        // reverse multibyte values
        ReverseAll(no, cost);
        ReverseAll(hp_1, hp_max, hp_grow);
        ReverseAll(atk_1, atk_max, atk_grow);
        ReverseAll(heal_1, heal_max, heal_grow);
        ReverseAll(exp_type, skill, leader_skill);
        for (int i = 0; i < 9; i++) {
            Reverse(kakusei[i]);
        }
    }

    MonsterData() = default;

    char name[97];
    Element element;
    uint8_t rare;
    uint16_t no;
    uint16_t cost;
    uint8_t unknown1[2];
    uint8_t max_lv;
    uint8_t ignore1[12];  // uint32_t ComposeExp Unknown02 Sell;

    float hp_1;
    float hp_max;
    float hp_grow;
    uint8_t unknown2[4];
    float atk_1;
    float atk_max;
    float atk_grow;
    uint8_t unknown3[4];
    float heal_1;
    float heal_max;
    float heal_grow;
    uint8_t unknown4[4];

    float exp_type;
    uint8_t unknown5[8];
    Type type;
    Type sub_type;

    uint16_t skill;
    uint8_t unknown6[4];
    uint16_t leader_skill;

    uint8_t ignore2[54];  // enemy stat
    uint8_t unknown7[160];  // lots of zeros?

    Element sub_element;

    uint8_t unknown8[11];  // 究極退化?
    uint16_t kakusei[9];
    uint8_t unknownN[6];

    float plus() const {
        return hp_max / 10 + atk_max / 5 + heal_max / 3;
    }
};


static_assert(is_pod<MonsterData>::value, "MonsterData is not pod");
static_assert(sizeof(MonsterData) == 438, "Incorrect MonsterData struct size");

template<class T>
void DumpHex(const T& t, ostream& os) {
    const char *p = reinterpret_cast<const char*>(&t);
    for (size_t i = 0; i < sizeof(T); i++) {
        os << setw(2) << hex << setfill('0') << (int)(unsigned char)p[i];
        if (i % 10 == 9) os << endl;
        else os << ' ';
    }
    os << dec << endl;
}

ostream& operator<<(ostream& os, const MonsterData& m) {
    os << m.no << ' ';
    os << m.name << ' ';
    os << m.element << '/' << m.sub_element << ' ';
    os << m.type << '/' << m.sub_type << ' ';
    os << m.hp_max << ' ' << m.atk_max << ' ' << m.heal_max << ' ' << m.plus();
    return os;
}

void DumpFloat(initializer_list<float>&& list) {
    for (auto f: list) DumpHex(f, cout);
}

int main() {
    ifstream fin("data021.bin");
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
   
    BinDecode(v);

    uint32_t monster_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));
    cout << "monster count: " << monster_count << endl;

    assert(v.size() == 32 + monster_count * sizeof(MonsterData));

    vector<MonsterData> m;
    for (size_t i = 0; i < monster_count; i++) {
        m.emplace_back(v.data() + 32 + i * sizeof(MonsterData));
    }

    for (size_t i = 0; i < monster_count; i++) {
        if (m[i].element == Element::LIGHT and (m[i].type == Type::ATTACK or m[i].sub_type == Type::ATTACK)) {
            cout << m[i] << endl;
        }
    }
}
