#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>

#include "./bin_decode.h"

using namespace std;

enum class Element : uint8_t {
    FIRE = 0,
    WATER = 1,
    WOOD = 2,
    LIGHT = 3,
    DARK = 4,
    NONE = 255
};

enum class Type {
    EVOLUTION = 0,
    BALANCE = 1,
    VITALITY = 2,
    HEAL = 3,
    DRAGON = 4,
    GOD = 5,
    ATTACK = 6,
    DEMON = 7,
    AWAKEN = 12,
    PROTECT = 13,
    STRONG = 14,
    NONE = 255
};

#pragma pack(1)
struct MonsterData {

    char name[97];
    uint8_t element;
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
    uint8_t type;
    uint8_t sub_type;

    uint16_t skill;
    uint8_t unknown6[4];
    uint16_t leader_skill;

    uint8_t ignore2[54];  // enemy stat
    uint8_t unknown7[160];  // lots of zeros?

    uint8_t sub_element;

    uint8_t unknown8[11];  // 究極退化?
    uint16_t kakusei[9];
    uint8_t unknownN[6];
};

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

template<class T>
T Reverse(T t) {
    char *p = reinterpret_cast<char*>(&t);
    reverse(p, p + sizeof(T));
    return t;
}

ostream& operator<<(ostream& os, const MonsterData& m) {
    os << "name: " << m.name << endl;
    os << "element: " << (int)m.element << ' ' << (int)m.sub_element << endl;
    os << "rare: " << (int)m.rare << endl;
    os << "no.: " << Reverse(m.no) << endl;
    os << "cost: " << Reverse(m.cost) << endl;
    os << "max_lv: " << (int)m.max_lv << endl;
    os << "hp: " << Reverse(m.hp_1) << ' ' << Reverse(m.hp_max) << ' ' << Reverse(m.hp_grow) << endl;
    os << "atk: " << Reverse(m.atk_1) << ' ' << Reverse(m.atk_max) << ' ' << Reverse(m.atk_grow) << endl;
    os << "heal: " << Reverse(m.heal_1) << ' ' << Reverse(m.heal_max) << ' ' << Reverse(m.heal_grow) << endl;
    os << "exp_type: " << (int)Reverse(m.exp_type) << endl;
    os << "type: " << (int)m.type << ", " << (int)m.sub_type << endl;
    os << "skill: " << Reverse(m.skill) << ", " << Reverse(m.leader_skill) << endl;

    os << "kakusei: ";
    for (auto x: m.kakusei) os << Reverse(x) << ' ';
    os << endl;
    return os;
}


void DumpFloat(initializer_list<float>&& list) {
    for (auto f: list) DumpHex(f, cout);
}

int main() {
    ifstream fin("data021.bin");
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
    assert(v.size() == 640388);
   
    BinDecode(v);

    uint32_t monster_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));
    cout << "monster count: " << monster_count << endl;
    
    MonsterData *m = reinterpret_cast<MonsterData*>(&v[32]);

    int x;
    cin >> x;
    cout << m[x] << endl;
}
