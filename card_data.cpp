#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>
#include <map>

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

enum class Type : uint8_t {
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

ostream& operator<<(ostream& os, const Element& e) {
    static const array<const char*, 5> names = {
        u8"火", u8"水", u8"木", u8"光", u8"闇",
    };
    if ((uint8_t)e <= 4) {
        return os << names[(uint8_t)e];
    } else {
        return os << u8"なし";
    }
}

ostream& operator<<(ostream& os, const Type& t) {
    static const map<uint8_t, const char*> names = {
        {0, u8"進化用"},
        {1, u8"バランス"},
        {2, u8"体力"},
        {3, u8"回復"},
        {4, u8"ドラゴン"},
        {5, u8"神"},
        {6, u8"攻撃"},
        {7, u8"悪魔"},
        {12, u8"覚醒用"},
        {13, u8"特別保護"},
        {14, u8"強化合成用"},
    };
    auto it = names.find((uint8_t)t);
    if (it != names.end()) {
        return os << it->second;
    } else {
        return os << u8"なし";
    }
}

#pragma pack(1)
struct MonsterData {

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
    os << Reverse(m.no) << ' ';
    os << m.name << ' ';
    os << m.element << '/' << m.sub_element << ' ';
    os << m.type << '/' << m.sub_type << ' ';
    os << Reverse(m.hp_max) << ' ' << Reverse(m.atk_max) << ' ' << Reverse(m.heal_max);
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
    
    MonsterData *m = reinterpret_cast<MonsterData*>(&v[32]);

    for (size_t i = 0; i < monster_count; i++) {
        if (m[i].element == Element::LIGHT and (m[i].type == Type::ATTACK or m[i].sub_type == Type::ATTACK)) {
            cout << m[i] << endl;
        }
    }
}
